#include "KernelFilterBuffer.h"

KernelFilterBuffer::KernelFilterBuffer(AOpenCLInterface* const opencl, cl::Program* const program)
    : KernelFilter(opencl, program)
{}

KernelFilterBuffer::~KernelFilterBuffer()
{}

std::string KernelFilterBuffer::kernelSource()
{
    return getKernelSource("kernels/filter_buffer.cl");
}

cl::Event KernelFilterBuffer::runSingle(cl::Buffer& img, cl::Buffer& imgDst, cl::Buffer& locationLookup, int octave, const std::vector<Lookup>& lookup)
{
    const size_t depth = 4;
    const int base = octave * depth;
    const size_t rows = lookup[base].imgHeight;
    const size_t cols = lookup[base].imgWidth;

    ASSERT(rows > 0 && cols > 0, "The image object seems to be invalid, no rows/cols set");
    ASSERT(border == cv::BORDER_REPLICATE || border == cv::BORDER_REFLECT101, "Unsupported border type");

    cl::Kernel kernel(*program, "filter_single");
    kernel.setArg(0, img);
    kernel.setArg(1, imgDst);
    kernel.setArg(2, locationLookup);
    kernel.setArg(3, bufferKernel1);
    kernel.setArg(4, kernel1.rows);
    kernel.setArg(5, kernel1.rows / 2);
    kernel.setArg(6, kernel1.cols);
    kernel.setArg(7, kernel1.cols / 2);
    kernel.setArg(8, border);

    cl::Event eventFilter;
    const cl::NDRange offset(0, 0, base);
    const cl::NDRange global(cols, rows, depth);
    queue->enqueueNDRangeKernel(kernel, offset, global, cl::NullRange, &events, &eventFilter);

    // Clear for next call. From the OpenCL API side this is no problem since the event list can be freed after the enqueue function returns
    events.clear();

    return eventFilter;
}

cl::Event KernelFilterBuffer::runSingleLocal(cl::Buffer& img, cl::Buffer& imgDst, cl::Buffer& locationLookup, int octave, const std::vector<Lookup>& lookup)
{
    const size_t depth = 4;
    const int base = octave * depth;
    const size_t rows = lookup[base].imgHeight;
    const size_t cols = lookup[base].imgWidth;

    ASSERT(rows > 0 && cols > 0, "The image object seems to be invalid, no rows/cols set");
    ASSERT(border == cv::BORDER_REPLICATE || border == cv::BORDER_REFLECT101, "Unsupported border type");

    cl::Kernel kernel(*program, "filter_single_local");
    kernel.setArg(0, img);
    kernel.setArg(1, imgDst);
    kernel.setArg(2, locationLookup);
    kernel.setArg(3, bufferKernel1);
    kernel.setArg(4, kernel1.rows);
    kernel.setArg(5, kernel1.rows / 2);
    kernel.setArg(6, kernel1.cols);
    kernel.setArg(7, kernel1.cols / 2);
    kernel.setArg(8, border);

    cl::Event eventFilter;
    const cl::NDRange offset(0, 0, base);
    const cl::NDRange global(cols, rows, depth);
    const cl::NDRange local(16, 16);
    queue->enqueueNDRangeKernel(kernel, offset, global, local, &events, &eventFilter);

    // Clear for next call. From the OpenCL API side this is no problem since the event list can be freed after the enqueue function returns
    events.clear();

    return eventFilter;
}

cl::Event KernelFilterBuffer::runSingleSeparationLocal(cl::Buffer& img, cl::Buffer& imgDst, cl::Buffer& locationLookup, int octave, const std::vector<Lookup>& lookup)
{
    const size_t depth = 4;
    const int base = octave * depth;
    const size_t rows = lookup[base].imgHeight;
    const size_t cols = lookup[base].imgWidth;

    ASSERT(rows > 0 && cols > 0, "The image object seems to be invalid, no rows/cols set");
    ASSERT(border == cv::BORDER_REPLICATE || border == cv::BORDER_REFLECT101, "Unsupported border type");

    if (!bufferSet)
    {
        const int totalPixels = lookup.back().previousPixels + lookup.back().imgHeight * lookup.back().imgWidth;
        imgTmp = cl::Buffer(*context, CL_MEM_READ_WRITE, sizeof(float) * totalPixels);
        bufferSet = true;
    }

    cl::Kernel kernelX(*program, "filter_single_local");
    kernelX.setArg(0, img);
    kernelX.setArg(1, imgTmp);
    kernelX.setArg(2, locationLookup);
    kernelX.setArg(3, bufferKernelSeparation1A);
    kernelX.setArg(4, kernelSeparation1A.rows);
    kernelX.setArg(5, kernelSeparation1A.rows / 2);
    kernelX.setArg(6, kernelSeparation1A.cols);
    kernelX.setArg(7, kernelSeparation1A.cols / 2);
    kernelX.setArg(8, border);

    cl::Kernel kernelY(*program, "filter_single_local");
    kernelY.setArg(0, imgTmp);
    kernelY.setArg(1, imgDst);
    kernelY.setArg(2, locationLookup);
    kernelY.setArg(3, bufferKernelSeparation1B);
    kernelY.setArg(4, kernelSeparation1B.rows);
    kernelY.setArg(5, kernelSeparation1B.rows / 2);
    kernelY.setArg(6, kernelSeparation1B.cols);
    kernelY.setArg(7, kernelSeparation1B.cols / 2);
    kernelY.setArg(8, border);

    cl::Event eventFilter;
    const cl::NDRange offset(0, 0, base);
    const cl::NDRange global(cols, rows, depth);
    const cl::NDRange local(16, 16);
    queue->enqueueNDRangeKernel(kernelX, offset, global, local, &events);
    queue->enqueueNDRangeKernel(kernelY, offset, global, local, nullptr, &eventFilter);

    // Clear for next call. From the OpenCL API side this is no problem since the event list can be freed after the enqueue function returns
    events.clear();

    return eventFilter;
}

cl::Event KernelFilterBuffer::runHalfsampleImage(cl::Buffer& img, cl::Buffer& locationLookup, int octave, const std::vector<Lookup>& lookup)
{
    const size_t depth = 4;
    const int lastInOctave = (octave + 1) * depth - 1;
    const size_t rows = lookup[lastInOctave].imgHeight;
    const size_t cols = lookup[lastInOctave].imgWidth;

    ASSERT(rows > 0 && cols > 0, "The image object seems to be invalid, no rows/cols set");

    cl::Kernel kernelConductivty(*program, "fed_resize");
    kernelConductivty.setArg(0, img);
    kernelConductivty.setArg(1, locationLookup);
    kernelConductivty.setArg(2, lastInOctave);

    cl::Event event;
    const cl::NDRange global(cols / 2, rows / 2);
    queue->enqueueNDRangeKernel(kernelConductivty, cl::NullRange, global, cl::NullRange, &events, &event);

    events.clear();

    return event;
}

cl::Event KernelFilterBuffer::runCopyInsideCube(cl::Buffer& img, cl::Buffer& locationLookup, int octave, const std::vector<Lookup>& lookup)
{
    const size_t depth = 4;
    const int base = octave * depth;
    const size_t rows = lookup[base].imgHeight;
    const size_t cols = lookup[base].imgWidth;

    ASSERT(rows > 0 && cols > 0, "The image object seems to be invalid, no rows/cols set");

    cl::Kernel kernelConductivty(*program, "copy_inside_cube");
    kernelConductivty.setArg(0, img);
    kernelConductivty.setArg(1, locationLookup);
    kernelConductivty.setArg(2, base);

    cl::Event event;
    const cl::NDRange offset(0, 0, base + 1);
    const cl::NDRange global(cols, rows, depth - 1);
    queue->enqueueNDRangeKernel(kernelConductivty, offset, global, cl::NullRange, &events, &event);

    events.clear();

    return event;
}
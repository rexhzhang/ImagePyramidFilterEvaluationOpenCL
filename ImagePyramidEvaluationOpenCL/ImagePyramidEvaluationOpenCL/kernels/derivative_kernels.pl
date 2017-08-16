our %kernels;
$kernels{'Gx_3x3'} = [-3, 0, 3, -10, 0, 10, -3, 0, 3];
$kernels{'Gy_3x3'} = [-3, -10, -3, 0, 0, 0, 3, 10, 3];
$kernels{'Gx_5x5'} = [-0.0468750037, 0, 0, 0, 0.0468750037, -0, 0, 0, 0, 0, -0.156250015, 0, 0, 0, 0.156250015, -0, 0, 0, 0, 0, -0.0468750037, 0, 0, 0, 0.0468750037];
$kernels{'Gy_5x5'} = [-0.0468750037, -0, -0.156250015, -0, -0.0468750037, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0468750037, 0, 0.156250015, 0, 0.0468750037];
$kernels{'Gx_7x7'} = [-0.0312500037, 0, 0, 0, 0, 0, 0.0312500037, -0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, -0.104166679, 0, 0, 0, 0, 0, 0.104166679, -0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, -0.0312500037, 0, 0, 0, 0, 0, 0.0312500037];
$kernels{'Gy_7x7'} = [-0.0312500037, -0, -0, -0.104166679, -0, -0, -0.0312500037, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0312500037, 0, 0, 0.104166679, 0, 0, 0.0312500037];
$kernels{'Gx_9x9'} = [-0.0234375019, 0, 0, 0, 0, 0, 0, 0, 0.0234375019, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0.0781250075, 0, 0, 0, 0, 0, 0, 0, 0.0781250075, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0, 0, 0, 0, 0, 0, 0, 0, 0, -0.0234375019, 0, 0, 0, 0, 0, 0, 0, 0.0234375019];
$kernels{'Gy_9x9'} = [-0.0234375019, -0, -0, -0, -0.0781250075, -0, -0, -0, -0.0234375019, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0234375019, 0, 0, 0, 0.0781250075, 0, 0, 0, 0.0234375019];
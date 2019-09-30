static float3	int_color(int col)
{
	float3	v;

	v.x = (float)((col >> 16) & 0xFF) / 255;
	v.y = (float)((col >> 8) & 0xFF) / 255;
	v.z = (float)((col) & 0xFF) / 255;
	return (v);
}

static float	gauss_coeff_x(int x, float sigma)
{
	return(1000.f / (sqrt(2 * M_PI_F) * sigma) * exp(-(float)(x * x) / (2 * sigma * sigma)));
}

__kernel void	gauss_blur_y(
							const __global int	*input_data,
							__global int		*output_data,
							__global t_screen	*screen)
{
	int tx = get_global_id(0);
	int ty = get_global_id(1);
	int index = ty * WIDTH + tx;

	float3	color;
	int		a = 3 * (int)SIGMA;
	float	sum = 0;
	int		coef;

	color = (float3) 0;
	for (int j = -a; j <= a; j++)
	{
		if (ty + j >= 0 && ty + j < HEIGHT)
		{
			coef = gauss_coeff_x(j, SIGMA);
			color += int_color(input_data[WIDTH * (ty + j) + tx]) * coef;
			sum += coef;
		}
	}
	output_data[index] = get_color(color / sum, screen->effects);
}

__kernel void	gauss_blur_x(
							const __global int	*input_data,
							__global int		*output_data,
							__global t_screen	*screen)
{
	int tx = get_global_id(0);
	int ty = get_global_id(1);
	int index = ty * WIDTH + tx;

	float3	color;
	int		a = 3 * (int)SIGMA;
	float	sum = 0;
	int		coef;

	color = (float3) 0;
	for (int i = -a; i <= a; i++)
	{
		if (tx + i >= 0 && tx + i < WIDTH)
		{
			coef = gauss_coeff_x(i, SIGMA);
			color += int_color(input_data[WIDTH * ty + (tx + i)]) * coef;
			sum += coef;
		}
	}
	output_data[index] = get_color(color / sum, screen->effects);
}

__kernel void	post_processing(
								const __global int	*input_data,
								__global int		*output_data,
								__global t_screen	*screen)
{
	int tx = get_global_id(0);
	int ty = get_global_id(1);
	int index = ty * WIDTH + tx;


	if (screen->effects.y == 1)
	{
		float3	l_color;
		float3	r_color;
		float3	anaglyph_color;
		int		sdvig = 8;

		l_color = int_color(input_data[ty * WIDTH + (tx - sdvig)]);
		r_color = int_color(input_data[ty * WIDTH + (tx + sdvig)]);

		anaglyph_color.x = l_color.x;
		anaglyph_color.y = r_color.y;
		anaglyph_color.z = r_color.z;

		output_data[index] = get_color(anaglyph_color, screen->effects);
	}
	else if (screen->effects.w == 1)
	{
		float16 bayer = {0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5};
		bayer *= 1.f / 16;
		int col = (int)(bayer[((tx % 4) * 4 + ty % 4)] * 255);

		int red = (input_data[index] >> 16) & 0xFF;
		int green = (input_data[index] >> 8) & 0xFF;
		int blue = input_data[index] & 0xFF;

		red = col > red ? 0 : 255;
		green = col > green ? 0 : 255;
		blue = col > blue ? 0 : 255;

		output_data[index] = ((red << 16) | (green << 8) | blue);
	}
	else
		output_data[index] = input_data[index];
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_key_0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 18:33:57 by broggo            #+#    #+#             */
/*   Updated: 2019/08/02 18:33:57 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int	ft_esc(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img.img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win);
	exit(0);
	return (0);
}

int	ft_aa(int keycode, t_mlx *mlx)
{
	mlx->screen.fsaa_n = 2 * (keycode - 18);
	return (0);
}

int ft_effects(int keycode, t_mlx *mlx)
{
	if (keycode == 6)
		mlx->screen.effects.x = (mlx->screen.effects.x + 1) % 2;
	else if (keycode == 7)
	{
		mlx->screen.effects.w = 0;
		mlx->screen.effects.y = (mlx->screen.effects.y + 1) % 2;
	}
	else if (keycode == 8)
	{
		mlx->screen.effects.z = (mlx->screen.effects.z + 1) % 2;
	}
	else if (keycode == 9)
	{
		mlx->screen.effects.y = 0;
		mlx->screen.effects.w = (mlx->screen.effects.w + 1) % 2;
	}
	return (0);
}

cl_int3	int_color(int col)
{
	cl_int3	v;
	
	v.x = (col >> 16) & 0xFF;
	v.y = (col >> 8) & 0xFF;
	v.z = (col) & 0xFF;
	return (v);
}

char * settime(struct tm *u)
{
	char s[40];
	char *tmp;

	for (int i = 0; i < 40; i++)
		s[i] = 0;
	strftime(s, 40, "%d.%m.%Y %H.%M.%S.png", u);
	tmp = (char*)malloc(sizeof(s));
	ft_strcpy(tmp, s);
	return(tmp);
}

int	ft_save(t_mlx *mlx)
{
	uint8_t* rgb_image;
	rgb_image = malloc(WIDTH * HEIGHT * CHANNEL_NUM);
	
	cl_int3	color;
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			color = int_color(mlx->img.data[j * WIDTH + i]);
			
			rgb_image[3 * (j * WIDTH + i)] = (uint8_t)color.x;
			rgb_image[3 * (j * WIDTH + i) + 1] = (uint8_t)color.y;
			rgb_image[3 * (j * WIDTH + i) + 2] = (uint8_t)color.z;
		}
	}
	struct tm		*u;
	char			*name;
	const time_t	timer = time(NULL);
	u = localtime(&timer);
	name = settime(u);
	if (!stbi_write_png(ft_strjoin("images/", name), WIDTH, HEIGHT, CHANNEL_NUM, rgb_image, WIDTH * CHANNEL_NUM))
		stbi_write_png(name, WIDTH, HEIGHT, CHANNEL_NUM, rgb_image, WIDTH * CHANNEL_NUM);
	free(name);

	return (0);
}

int	check_key(int keycode, t_mlx *mlx)
{
	if (mlx->mouse.r == 1 || mlx->mouse.l == 1)
		return(0);
	
	if (keycode == 123 || keycode == 124)
		ft_move_alpha(keycode, mlx);
	else if (keycode == 125 || keycode == 126)
		ft_move_betta(keycode, mlx);
	else if (keycode == 13 || keycode == 1)
		ft_move_x(keycode, mlx);
	else if (keycode == 0 || keycode == 2)
		ft_move_y(keycode, mlx);
	else if (keycode == 15 || keycode == 3)
		ft_move_z(keycode, mlx);
	else if ((keycode > 17) && (keycode < 22))
		ft_aa(keycode, mlx);
	else if ((keycode > 5) && (keycode < 10))
		ft_effects(keycode, mlx);
	else if (keycode == 29)
		return (ft_save(mlx));
	else if (keycode == 53)
		return (ft_esc(mlx));
	else
		return (0);
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	draw_picture(mlx);
	return (0);
}
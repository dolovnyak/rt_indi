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

cl_int3	int_color(int col)
{
	cl_int3	v;

	v.x = (col >> 16) & 0xFF;
	v.y = (col >> 8) & 0xFF;
	v.z = (col) & 0xFF;
	return (v);
}

char	*settime(struct tm *u)
{
	char	s[40];
	char	*tmp;
	int		i;

	i = -1;
	while (++i < 40)
		s[i] = 0;
	strftime(s, 40, "%d.%m.%Y %H.%M.%S.png", u);
	tmp = (char*)malloc(sizeof(s));
	ft_strcpy(tmp, s);
	return (tmp);
}

void	save_helper(t_rt *rt, uint8_t *rgb_image)
{
	cl_int3	color;
	int		i;
	int		j;

	i = -1;
	while (++i < WIDTH)
	{
		j = -1;
		while (++j < HEIGHT)
		{
			color = int_color(rt->img.data[j * WIDTH + i]);
			rgb_image[3 * (j * WIDTH + i)] = (uint8_t)color.x;
			rgb_image[3 * (j * WIDTH + i) + 1] = (uint8_t)color.y;
			rgb_image[3 * (j * WIDTH + i) + 2] = (uint8_t)color.z;
		}
	}
}

int		ft_save(t_rt *rt)
{
	struct tm		*u;
	char			*name;
	const time_t	timer = time(NULL);
	uint8_t			*rgb_image;

	rgb_image = ft_memalloc(WIDTH * HEIGHT * CHANNEL_NUM);
	save_helper(rt, rgb_image);
	u = localtime(&timer);
	name = settime(u);
	if (!stbi_write_png(ft_strjoin("images/", name), WIDTH, HEIGHT,
			CHANNEL_NUM, rgb_image, WIDTH * CHANNEL_NUM))
		stbi_write_png(name, WIDTH, HEIGHT, CHANNEL_NUM,
				rgb_image, WIDTH * CHANNEL_NUM);
	free(name);
	return (0);
}

int		check_key(int keycode, t_rt *rt)
{
	if (rt->mouse.r == 1 || rt->mouse.l == 1)
		return (0);
	if (keycode == 123 || keycode == 124)
		ft_move_alpha(keycode, rt);
	else if (keycode == 125 || keycode == 126)
		ft_move_betta(keycode, rt);
	else if (keycode == 13 || keycode == 1)
		ft_move_x(keycode, rt);
	else if (keycode == 0 || keycode == 2)
		ft_move_y(keycode, rt);
	else if (keycode == 15 || keycode == 3)
		ft_move_z(keycode, rt);
	else if ((keycode > 17) && (keycode < 22))
		ft_aa(keycode, rt);
	else if (((keycode > 5) && (keycode < 10)) || keycode == 37)
		ft_effects(keycode, rt);
	else if (keycode == 29)
		return (ft_save(rt));
	else if (keycode == 53)
		return (ft_esc(rt));
	else
		return (0);
	mlx_clear_window(rt->mlx_ptr, rt->win);
	draw_picture(rt);
	return (0);
}

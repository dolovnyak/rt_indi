/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_key_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:19:01 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:21 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ft_esc(t_rt *rt)
{
    release_gpu_mem(rt);
	mlx_destroy_image(rt->mlx_ptr, rt->img.img_ptr);
	mlx_destroy_window(rt->mlx_ptr, rt->win);
	exit(0);
	return (0);
}

int		ft_aa(int keycode, t_rt *rt)
{
	rt->screen.fsaa_n = 2 * (keycode - 18);
	return (0);
}

int		ft_effects(int keycode, t_rt *rt)
{
	if (keycode == 6)
		rt->screen.effects ^= GRAY;
	else if (keycode == 7)
	{
		rt->screen.effects &= ~(DITHERING);
		rt->screen.effects ^= THREE_D;
	}
	else if (keycode == 8)
		rt->screen.effects ^= GAYSS_BLURE;
	else if (keycode == 9)
	{
		rt->screen.effects &= ~(THREE_D);
		rt->screen.effects ^= DITHERING;
	}
	else if (keycode == 37)
	{
		rt->screen.params ^= PHONG;
		rt->screen.params ^= PATH_TRACE;
	}
	printf("%d\n", rt->screen.effects);
	return (0);
}

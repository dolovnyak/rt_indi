/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_key_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 16:19:19 by broggo            #+#    #+#             */
/*   Updated: 2019/08/08 16:19:19 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	ft_move_x(int keycode, t_rt *rt)
{
	cl_float3	v;

	v = cl_mult_n(rt->screen.center, SPEED1);
	if (keycode == 13)
		rt->cam.center = cl_minus(rt->cam.center, v);
	else
		rt->cam.center = cl_sum(rt->cam.center, v);
	return (0);
}

int	ft_move_y(int keycode, t_rt *rt)
{
	cl_float3	v;

	v = cl_mult_n(rt->screen.v1, SPEED1);
	if (keycode == 2)
		rt->cam.center = cl_sum(rt->cam.center, v);
	else
		rt->cam.center = cl_minus(rt->cam.center, v);
	return (0);
}

int	ft_move_alpha(int keycode, t_rt *rt)
{
	if (keycode == 124)
		rt->cam.alpha += SPEED2;
	else
		rt->cam.alpha -= SPEED2;
	
	if (rt->cam.alpha > (float)M_PI - 1e-5f)
		rt->cam.alpha -= 2 * (float)M_PI;
	if (rt->cam.alpha < -(float)M_PI + 1e-5f)
		rt->cam.alpha += - 2 * (float)M_PI;
	return (0);
}

int	ft_move_betta(int keycode, t_rt *rt)
{
	if (keycode == 126)
	{
		if ((rt->cam.betta - SPEED2) >= 1e-5f)
			rt->cam.betta -= SPEED2;
	}
	else
	{
		if ((rt->cam.betta + SPEED2) <= M_PI - 1e-5f)
			rt->cam.betta += SPEED2;
	}
	return (0);
}

int	ft_move_z(int keycode, t_rt *rt)
{
	if (keycode == 15)
		rt->cam.center.z = rt->cam.center.z + M_PI / 3.14;
	else
		rt->cam.center.z = rt->cam.center.z - M_PI / 3.14;
	return (0);
}

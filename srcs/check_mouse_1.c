/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_mouse_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 16:17:12 by broggo            #+#    #+#             */
/*   Updated: 2019/08/08 16:17:12 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	ft_move_mouse(t_rt *rt, int button, int x, int y)
{
	cl_float3	v;
	cl_float3	v1;
	cl_float3	v2;

	v1 = cl_mult_n(rt->screen.v1, 0.5f - (float)(x) / WIDTH);
	v2 = cl_mult_n(rt->screen.v2, (float)(y) / WIDTH - 0.5f);
	v = cl_sum(rt->screen.center, v1);
	v = cl_sum(v, v2);
	cl_to_norm(&v);
	v = cl_mult_n(v, (9 - 2 * button) * SPEED3);
	rt->cam.center = cl_sum(rt->cam.center, v);
	return (1);
}

int    ft_mouse_alpha_betta(t_rt *rt, int x, int y)
{
	cl_float3    v;
	cl_float3    v1;
	cl_float3    v2;

	v1 = cl_mult_n(rt->screen.v1, (float)(x - rt->mouse.x) / WIDTH);
	v2 = cl_mult_n(rt->screen.v2, (float)(rt->mouse.y - y) / WIDTH);
	v = cl_sum(rt->screen.center, v2);
	float angle = cl_angle(v, rt->screen.center);
	if (y - rt->mouse.y < 0.f)
	{
		if (rt->mouse.betta + angle <= M_PI - 1e-5f)
			rt->cam.betta = rt->mouse.betta + angle;
	}
	else
	{
		if ((rt->cam.betta - angle) >= 1e-5f)
			rt->cam.betta = rt->mouse.betta - angle;
	}
	v = cl_sum(rt->screen.center, v1);
	if (x - rt->mouse.x < 0.f)
		rt->cam.alpha = rt->mouse.alpha + cl_angle(v, rt->screen.center);
	else
		rt->cam.alpha = rt->mouse.alpha - cl_angle(v, rt->screen.center);
	if (rt->cam.alpha > (float)M_PI - 1e-5f)
		rt->cam.alpha -= 2 * (float)M_PI;
	if (rt->cam.alpha < -(float)M_PI + 1e-5f)
		rt->cam.alpha += - 2 * (float)M_PI;
	return (0);
}

int    ft_mouse_x_y(t_rt *rt, int x, int y)
{
	rt->cam.center = cl_minus(rt->mouse.center,
			cl_mult_n(rt->screen.v1, SPEED4 * (float)(x - rt->mouse.x) / 64));
	rt->cam.center = cl_minus(rt->cam.center,
			cl_mult_n(rt->screen.v2, SPEED4 * (float)(rt->mouse.y - y) / 64));
	return (0);
}

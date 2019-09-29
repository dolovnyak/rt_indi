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

int	ft_move_mouse(t_mlx *mlx, int button, int x, int y)
{
	cl_float3	v;
	cl_float3	v1;
	cl_float3	v2;

	v1 = cl_mult_n(mlx->screen.v1, 0.5f - (float)(x) / WIDTH);
	v2 = cl_mult_n(mlx->screen.v2, (float)(y) / WIDTH - 0.5f);
	v = cl_sum(mlx->screen.center, v1);
	v = cl_sum(v, v2);
	cl_to_norm(&v);
	v = cl_mult_n(v, (9 - 2 * button) * SPEED3);
	mlx->cam.center = cl_sum(mlx->cam.center, v);
	return (0);
}

int    ft_mouse_alpha_betta(t_mlx *mlx, int x, int y)
{
	cl_float3    v;
	cl_float3    v1;
	cl_float3    v2;

	v1 = cl_mult_n(mlx->screen.v1, (float)(x - mlx->mouse.x) / WIDTH);
	v2 = cl_mult_n(mlx->screen.v2, (float)(mlx->mouse.y - y) / WIDTH);
	v = cl_sum(mlx->screen.center, v2);
	
	float angle = cl_angle(v, mlx->screen.center);
	if (y - mlx->mouse.y < 0.f)
	{
		if (mlx->mouse.betta + angle <= M_PI - 1e-5f)
			mlx->cam.betta = mlx->mouse.betta + angle;
	}
	else
	{
		if ((mlx->cam.betta - angle) >= 1e-5f)
			mlx->cam.betta = mlx->mouse.betta - angle;
	}
	
	v = cl_sum(mlx->screen.center, v1);
	if (x - mlx->mouse.x < 0.f)
		mlx->cam.alpha = mlx->mouse.alpha + cl_angle(v, mlx->screen.center);
	else
		mlx->cam.alpha = mlx->mouse.alpha - cl_angle(v, mlx->screen.center);
	
	if (mlx->cam.alpha > (float)M_PI - 1e-5f)
		mlx->cam.alpha -= 2 * (float)M_PI;
	if (mlx->cam.alpha < -(float)M_PI + 1e-5f)
		mlx->cam.alpha += - 2 * (float)M_PI;
	return (0);
}

int    ft_mouse_x_y(t_mlx *mlx, int x, int y)
{
	mlx->cam.center = cl_minus(mlx->mouse.center, cl_mult_n(mlx->screen.v1, SPEED4 * (float)(x - mlx->mouse.x) / 64));
	mlx->cam.center = cl_minus(mlx->cam.center , cl_mult_n(mlx->screen.v2, SPEED4 * (float)(mlx->mouse.y - y) / 64));
	return (0);
}

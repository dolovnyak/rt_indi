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

int	ft_move_x(int keycode, t_mlx *mlx)
{
	cl_float3	v;

	v = cl_mult_n(mlx->screen.center, SPEED1);
	if (keycode == 13)
		mlx->cam.center = cl_minus(mlx->cam.center, v);
	else
		mlx->cam.center = cl_sum(mlx->cam.center, v);
	return (0);
}

int	ft_move_y(int keycode, t_mlx *mlx)
{
	cl_float3	v;

	v = cl_mult_n(mlx->screen.v1, SPEED1);
	if (keycode == 2)
		mlx->cam.center = cl_sum(mlx->cam.center, v);
	else
		mlx->cam.center = cl_minus(mlx->cam.center, v);
	return (0);
}

int	ft_move_alpha(int keycode, t_mlx *mlx)
{
	if (keycode == 124)
		mlx->cam.alpha += SPEED2;
	else
		mlx->cam.alpha -= SPEED2;
	
	if (mlx->cam.alpha > (float)M_PI - 1e-5f)
		mlx->cam.alpha -= 2 * (float)M_PI;
	if (mlx->cam.alpha < -(float)M_PI + 1e-5f)
		mlx->cam.alpha += - 2 * (float)M_PI;
	return (0);
}

int	ft_move_betta(int keycode, t_mlx *mlx)
{
	if (keycode == 126)
	{
		if ((mlx->cam.betta - SPEED2) >= 1e-5f)
			mlx->cam.betta -= SPEED2;
	}
	else
	{
		if ((mlx->cam.betta + SPEED2) <= M_PI - 1e-5f)
			mlx->cam.betta += SPEED2;
	}
	return (0);
}

int	ft_move_z(int keycode, t_mlx *mlx)
{
	if (keycode == 15)
		mlx->cam.center.z = mlx->cam.center.z + M_PI / 3.14;
	else
		mlx->cam.center.z = mlx->cam.center.z - M_PI / 3.14;
	return (0);
}

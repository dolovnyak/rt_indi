/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_mouse_0.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 16:15:49 by broggo            #+#    #+#             */
/*   Updated: 2019/08/08 16:15:49 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	mouse_press(int button, int x, int y, t_mlx *mlx)
{
	mlx->screen.fsaa_n = 0;
	if (((button == 4) || (button == 5)) && (mlx->mouse.r == 0 && mlx->mouse.l == 0))
	{
		ft_move_mouse(mlx, button, x, y);
		mlx_clear_window(mlx->mlx_ptr, mlx->win);
		draw_picture(mlx);
	}
	else if ((button == 1) && (mlx->mouse.l == 0) && (mlx->mouse.r == 0))
	{
		mlx->mouse.alpha = mlx->cam.alpha;
		mlx->mouse.betta = mlx->cam.betta;
		mlx->mouse.x = x;
		mlx->mouse.y = y;
		mlx->mouse.l = 1;
	}
	else if ((button == 2) && (mlx->mouse.l == 0) && (mlx->mouse.r == 0))
	{
		mlx->mouse.center = mlx->cam.center;
		mlx->mouse.x = x;
		mlx->mouse.y = y;
		mlx->mouse.r = 1;
	}
	else
		return (0);
	return (0);
}

int    mouse_move(int x, int y, t_mlx *mlx)
{
	if ((mlx->mouse.l == 1) && (mlx->mouse.r == 0))
		ft_mouse_alpha_betta(mlx, x, y);
	else if (mlx->mouse.r == 1)
		ft_mouse_x_y(mlx, x, y);
	else
		return (0);
	
	mlx_clear_window(mlx->mlx_ptr, mlx->win);
	draw_picture(mlx);
	return (0);
}

int		mouse_release(int button, int x, int y, t_mlx *mlx)
{
	x = 0;
	y = 0;
	if  (button == 1)
		mlx->mouse.l = 0;
	else if (button == 2)
		mlx->mouse.r = 0;
	return (0);
}

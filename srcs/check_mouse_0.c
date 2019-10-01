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

int	mouse_press(int button, int x, int y, t_rt *rt)
{
	rt->screen.fsaa_n = 0; //TODO WHAT THE FUCK?!
	if (((button == 4) || (button == 5)) && (rt->mouse.r == 0 && rt->mouse.l == 0))
	{
		ft_move_mouse(rt, button, x, y);
		mlx_clear_window(rt->mlx_ptr, rt->win);
		draw_picture(rt);
	}
	else if ((button == 1) && (rt->mouse.l == 0) && (rt->mouse.r == 0))
	{
		rt->mouse.alpha = rt->cam.alpha;
		rt->mouse.betta = rt->cam.betta;
		rt->mouse.x = x;
		rt->mouse.y = y;
		rt->mouse.l = 1;
	}
	else if ((button == 2) && (rt->mouse.l == 0) && (rt->mouse.r == 0))
	{
		rt->mouse.center = rt->cam.center;
		rt->mouse.x = x;
		rt->mouse.y = y;
		rt->mouse.r = 1;
	}
	else
		return (0);
	return (0);
}

int    mouse_move(int x, int y, t_rt *rt)
{
	if ((rt->mouse.l == 1) && (rt->mouse.r == 0))
		ft_mouse_alpha_betta(rt, x, y);
	else if (rt->mouse.r == 1)
		ft_mouse_x_y(rt, x, y);
	else
		return (0);
	
	mlx_clear_window(rt->mlx_ptr, rt->win);
	draw_picture(rt);
	return (0);
}

int		mouse_release(int button, int x, int y, t_rt *rt)
{
	x = 0;
	y = 0;
	if  (button == 1)
		rt->mouse.l = 0;
	else if (button == 2)
		rt->mouse.r = 0;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_jtoc_get_effects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:18:02 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:35 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

char	*g_err_str;

int		rt_jtoc_get_effects(t_rt *rt, t_jnode *n)
{
	t_jnode *tmp;

	g_err_str = "EFFECTS NAME ERROR OR EFFECTS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "gray"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (jtoc_get_int(tmp) == 1)
		rt->screen.effects |= GRAY;
	if (!(tmp = jtoc_node_get_by_path(n, "3d"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (jtoc_get_int(tmp) == 1)
		rt->screen.effects |= THREE_D;
	if (!(tmp = jtoc_node_get_by_path(n, "gauss_blure"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (jtoc_get_int(tmp) == 1)
		rt->screen.effects |= GAYSS_BLURE;
	if (!(tmp = jtoc_node_get_by_path(n, "dithering"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (jtoc_get_int(tmp) == 1)
		rt->screen.effects |= DITHERING;
	return (FUNCTION_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_jtoc_get_lights.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:18:11 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:31 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

int	rt_jtoc_get_light(t_light *light, t_jnode *n)
{
	t_jnode *tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "pos")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("POS ERROR OR POS MISSING", -1));
	if (rt_jtoc_get_float3(&light->center, tmp))
		return (rt_jtoc_sdl_log_error("POS PARAM ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "intens"))
												|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("INTENS ERROR OR INTENS MISSING", -1));
	light->intens = jtoc_get_float(tmp);
	if (light->intens < 0)
		return (rt_jtoc_sdl_log_error("INTENS ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int	rt_jtoc_get_lights(t_rt *rt, t_jnode *n)
{
	t_jnode			*tmp;
	t_light			*lights;
	int				i;
	unsigned int	g;

	g = 0;
	if (rt_jtoc_get_objects_num_in_arr(&g, n))
		return (FUNCTION_FAILURE);
	rt->counter.l = g;
	lights = ft_memalloc(sizeof(t_light) * rt->counter.l);
	i = 0;
	tmp = n->down;
	g_err_str = "LIGHTS IS NOT SET OR LIGHT TYPE ERROR";
	while (tmp)
	{
		if (tmp->type != object)
			return (rt_jtoc_sdl_log_error(g_err_str, i));
		if (rt_jtoc_get_light(&(lights[i]), tmp))
			return (rt_jtoc_sdl_log_error("LIGHT DATA ERROR", i));
		i++;
		tmp = tmp->right;
	}
	rt->light = lights;
	return (FUNCTION_SUCCESS);
}

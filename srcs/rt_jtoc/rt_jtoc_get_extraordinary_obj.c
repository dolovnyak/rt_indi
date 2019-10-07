/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_jtoc_get_extraordinary_obj.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:18:08 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:34 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

int			rt_jtoc_get_torus(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	g_err_str = "RADIUS TYPE ERROR OR RADIUS IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->radius = jtoc_get_float(tmp);
	if (obj->radius <= 0)
		return (rt_jtoc_sdl_log_error("RADIUS ERROR", -1));
	g_err_str = "INNER_RADIUS TYPE ERROR OR INNER_RADIUS IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "inner_radius"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->param = jtoc_get_float(tmp);
	if (obj->param <= 0)
		return (rt_jtoc_sdl_log_error("RADIUS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (FUNCTION_FAILURE);
	if (!(obj->vector.x == 0 && obj->vector.y == 0 && obj->vector.z == 0))
		obj->vector = cl_normalize(obj->vector);
	else
		return (rt_jtoc_sdl_log_error("VECTOR ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_hyper(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	g_err_str = "PARAM TYPE ERROR OR PARAM IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "param"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (FUNCTION_FAILURE);
	if (!(obj->vector.x == 0 && obj->vector.y == 0 && obj->vector.z == 0))
		obj->vector = cl_normalize(obj->vector);
	else
		return (rt_jtoc_sdl_log_error("VECTOR ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_sqr(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "first")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("A ERROR OR A IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->a, tmp))
		return (FUNCTION_FAILURE);
	if (!(tmp = jtoc_node_get_by_path(n, "second")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("B ERROR OR B IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->b, tmp))
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_mandelbulb(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	g_err_str = "POWER TYPE ERROR OR POWER IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "power"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->param = jtoc_get_float(tmp);
	g_err_str = "ITERTATION TYPE ERROR OR ITERTATION IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "iteration"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->radius = jtoc_get_float(tmp);
	if (obj->radius < 0.f || obj->radius > 200.f)
		obj->radius = 15.f;
	return (FUNCTION_SUCCESS);
}

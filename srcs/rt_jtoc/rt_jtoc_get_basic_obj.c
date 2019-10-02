#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

int			rt_jtoc_get_sphere(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	g_err_str = "RADIUS TYPE ERROR OR RADIUS IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->radius = jtoc_get_float(tmp);
	if (obj->radius <= 0)
		return (rt_jtoc_sdl_log_error("RADIUS ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_plane(t_object *obj, t_jnode *n)
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
		return (rt_jtoc_sdl_log_error("VEC TYPE ERROR OR VEC IS MISSING", -1));
	if (!(obj->vector.x == 0 && obj->vector.y == 0 && obj->vector.z == 0))
		obj->vector = cl_normalize(obj->vector);
	else
		return (rt_jtoc_sdl_log_error("VECTOR ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_cone(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "param"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("VEC TYPE ERROR OR VEC IS MISSING", -1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	g_err_str = "PARAMS TYPE ERROR OR PARAMS IS MISSING";
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (!(obj->vector.x == 0 && obj->vector.y == 0 && obj->vector.z == 0))
		obj->vector = cl_normalize(obj->vector);
	else
		return (rt_jtoc_sdl_log_error("VECTOR ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_cylinder(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	g_err_str = "PARAMS TYPE ERROR OR PARAMS IS MISSING";
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (!(obj->vector.x == 0 && obj->vector.y == 0 && obj->vector.z == 0))
		obj->vector = cl_normalize(obj->vector);
	else
		return (rt_jtoc_sdl_log_error("VECTOR ERROR", -1));
	g_err_str = "RADIUS TYPE ERROR OR RADIUS IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->radius = jtoc_get_float(tmp);
	if (obj->radius <= 0)
		return (rt_jtoc_sdl_log_error("RADIUS ERROR", -1));
	return (FUNCTION_SUCCESS);
}

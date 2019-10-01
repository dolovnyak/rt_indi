#include "rt.h"
#include "rt_jtoc.h"

int			rt_jtoc_get_sphere(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
											|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("RADIUS TYPE ERROR OR RADIUS IS MISSING",
				-1));
	obj->radius = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}


int			rt_jtoc_get_torus(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
												|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("RADIUS TYPE ERROR OR RADIUS IS MISSING",
				-1));
	obj->radius = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "inner_radius"))
								|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("INNER_RADIUS TYPE ERROR"
								"OR INNER_RADIUS IS MISSING", -1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_hyper(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "param"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PARAM TYPE ERROR OR PARAM IS MISSING",
									  -1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_sqr(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "first")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("A ERROR OR A IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->a, tmp))
	{
		printf(".!.\n");
		return (FUNCTION_FAILURE);
	}
	if (!(tmp = jtoc_node_get_by_path(n, "second")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("B ERROR OR B IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->b, tmp))
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_plane(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "param")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PARAM TYPE ERROR OR PARAM IS MISSING",
				-1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING",
				-1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (rt_jtoc_sdl_log_error("VEC TYPE ERROR OR VEC IS MISSING",
				-1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_cone(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "param"))
									|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("VEC TYPE ERROR OR VEC IS MISSING",
				-1));
	obj->param = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING",
				-1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (rt_jtoc_sdl_log_error("PARAMS TYPE ERROR OR PARAMS IS MISSING",
				-1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_cylinder(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;


	if (!(tmp = jtoc_node_get_by_path(n, "vec")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("VEC ERROR OR VEC IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->vector, tmp))
		return (rt_jtoc_sdl_log_error("PARAMS TYPE ERROR OR PARAMS IS MISSING",
				-1));
	if (!(tmp = jtoc_node_get_by_path(n, "radius"))
											|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("RADIUS TYPE ERROR OR RADIUS IS MISSING",
				-1));
	obj->radius = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}

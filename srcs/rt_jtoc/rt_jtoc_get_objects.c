/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_jtoc_get_objects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:18:20 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:29 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

int			rt_jtoc_phong_processing(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	g_err_str = "PHONG TYPE ERROR OR PHONG IS MISSING";
	if (!(n = jtoc_node_get_by_path(n, "phong")) || n->type != object)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (!(tmp = jtoc_node_get_by_path(n, "al1"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG al1 ERROR", -1));
	obj->mat.al.x = jtoc_get_float(tmp);
	if (obj->mat.al.x <= 0)
		return (rt_jtoc_sdl_log_error("PHONG al1 ERROR", -1));
	if (rt_jtoc_get_phong_param(obj, n))
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_obj_color_param(t_object *obj, t_jnode *n)
{
	t_jnode *tmp;

	if (rt_jtoc_phong_processing(obj, n))
		return (rt_jtoc_sdl_log_error("PHONG ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "emission"))
		|| tmp->type != object)
		return (rt_jtoc_sdl_log_error("EMISSION ERROR OR MISSING", -1));
	if (rt_jtoc_get_float3(&obj->mat.emission, tmp))
		return (rt_jtoc_sdl_log_error("EMISSION PARAM ERROR", -1));
	if ((obj->mat.emission.x < 0 || obj->mat.emission.y < 0
	|| obj->mat.emission.z < 0))
		return (rt_jtoc_sdl_log_error("EMISSION ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "reflection"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("REFLECTION ERROR OR MISSING", -1));
	obj->mat.reflection = jtoc_get_float(tmp);
	if (obj->mat.reflection > 1.f)
		return (rt_jtoc_sdl_log_error("REFLECTION ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "refraction"))
	|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("REFRACTION ERROR OR MISSING", -1));
	obj->mat.refraction = jtoc_get_float(tmp);
	if (obj->mat.refraction < 1.f || obj->mat.refraction > 1.3f)
		obj->mat.refraction = 0.f;
	if (!(tmp = jtoc_node_get_by_path(n ,"clouding"))
	|| tmp->type != fractional)
		return(rt_jtoc_sdl_log_error("CLOUDING ERROR OR MISSING", -1));
	obj->mat.clouding = jtoc_get_float(tmp);
	if (obj->mat.clouding > 1.f || obj->mat.clouding < 0.96)
		obj->mat.clouding = 1.f;
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_obj_param(t_object *obj, t_jnode *n,
				t_obj_texture *texture)
{
	t_jnode *tmp;

	if (rt_jtoc_get_object_type(obj, n))
		return (rt_jtoc_sdl_log_error("NOT VALID TYPE", -1));
	if (obj->e_type != o_plane)
		if ((!(tmp = jtoc_node_get_by_path(n, "pos")) || tmp->type != object))
			return (rt_jtoc_sdl_log_error("POS TYPE ERROR OR MISSING", -1));
	if (obj->e_type != o_plane)
		if (rt_jtoc_get_float3(&(obj->center), tmp))
			return (rt_jtoc_sdl_log_error("POS ERROR", -1));
	if (rt_jtoc_get_object_texture(obj, texture, n))
		return (rt_jtoc_sdl_log_error("NOT VALID TEXTURE", -1));
	g_err_str = "COLOR TYPE ERROR OR COLOR IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "color")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (rt_jtoc_get_float3(&obj->mat.diffuse_color, tmp))
		return (rt_jtoc_sdl_log_error("COLOR ERROR", -1));
	obj->mat.diffuse_color.x /= 255;
	obj->mat.diffuse_color.y /= 255;
	obj->mat.diffuse_color.z /= 255;
	if ((obj->mat.diffuse_color.x < 0 || obj->mat.diffuse_color.y < 0
	|| obj->mat.diffuse_color.z < 0) || obj->mat.diffuse_color.x > 1
	|| obj->mat.diffuse_color.y > 1 || obj->mat.diffuse_color.z > 1)
		return (rt_jtoc_sdl_log_error("COLOR ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_object(t_object *obj, t_jnode *n,
				t_obj_texture *texture)
{
	int		err;

	ft_bzero(obj, sizeof(t_object));
	if (rt_jtoc_get_obj_param(obj, n, texture))
		return (FUNCTION_FAILURE);
	if (rt_jtoc_get_obj_color_param(obj, n))
		return (FUNCTION_FAILURE);
	err = 0;
	err = obj->e_type == o_sphere ? rt_jtoc_get_sphere(obj, n) : err;
	err = obj->e_type == o_torus ? rt_jtoc_get_torus(obj, n) : err;
	err = obj->e_type == o_plane ? rt_jtoc_get_plane(obj, n) : err;
	err = obj->e_type == o_cone ? rt_jtoc_get_cone(obj, n) : err;
	err = obj->e_type == o_cylinder ? rt_jtoc_get_cylinder(obj, n) : err;
	err = obj->e_type == o_hyper ? rt_jtoc_get_hyper(obj, n) : err;
	err = obj->e_type == o_sqr ? rt_jtoc_get_sqr(obj, n) : err;
	err = obj->e_type == o_mandelbulb ? rt_jtoc_get_mandelbulb(obj, n) : err;
	if (err != 0)
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_objects(t_rt *rt, t_jnode *n, t_obj_texture *texture)
{
	t_jnode			*tmp;
	t_object		*objects;
	int				i;
	unsigned int	g;

	g = 0;
	if (rt_jtoc_get_objects_num_in_arr(&g, n))
		return (FUNCTION_FAILURE);
	rt->counter.all_obj = g;
	objects = ft_memalloc(sizeof(t_object) * rt->counter.all_obj);
	tmp = n->down;
	i = 0;
	while (tmp)
	{
		if (tmp->type != object)
			return (rt_jtoc_sdl_log_error("OBJECT TYPE ERROR", i));
		if (rt_jtoc_get_object(&(objects[i]), tmp, texture))
			return (rt_jtoc_sdl_log_error("OBJECT DATA ERROR", i));
		i++;
		tmp = tmp->right;
	}
	rt->obj = objects;
	return (FUNCTION_SUCCESS);
}

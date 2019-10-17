/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_jtoc_get_object_params.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkeli <rkeli@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:18:16 by rkeli             #+#    #+#             */
/*   Updated: 2019/10/06 17:20:30 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

int			rt_jtoc_compare_str_with_texture_name(t_obj_texture *texture,
																	char *str)
{
	int i;
	int j;
	int cache_counter;

	i = -1;
	cache_counter = texture->textures_count;
	while (++i < cache_counter)
	{
		j = ft_strlen(texture->textures_path[i]);
		while (texture->textures_path[i][j] != '/')
			j--;
		if ((ft_strstr(&texture->textures_path[i][j], str)))
			return (i);
	}
	return (-2);
}

int			rt_jtoc_get_object_texture(t_object *obj, t_obj_texture *texture,
																	t_jnode *n)
{
	t_jnode	*tmp;
	int		id;
	char	*str;

	g_err_str = "TEXTURE ERROR OR TEXTURE IS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "texture"))
		|| tmp->type != string)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	str = jtoc_get_string(tmp);
	if (!(ft_strcmp(str, "none")))
		id = -1;
	else
		id = rt_jtoc_compare_str_with_texture_name(texture, str);
	g_err_str = "TEXTURE NAME ERROR OR TEXTURE NAME IS MISSING";
	if (id == -2)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	obj->mat.texture_id = id;
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_object_type(t_object *obj, t_jnode *n)
{
	char	*str;
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "type")) || tmp->type != string)
		return (rt_jtoc_sdl_log_error("TYPE ERROR OR TYPE IS MISSING", -1));
	str = jtoc_get_string(tmp);
	obj->e_type = 0;
	obj->e_type = ft_strcmp(str, "sphere") ? obj->e_type : o_sphere;
	obj->e_type = ft_strcmp(str, "torus") ? obj->e_type : o_torus;
	obj->e_type = ft_strcmp(str, "cylinder") ? obj->e_type : o_cylinder;
	obj->e_type = ft_strcmp(str, "cone") ? obj->e_type : o_cone;
	obj->e_type = ft_strcmp(str, "plane") ? obj->e_type : o_plane;
	obj->e_type = ft_strcmp(str, "hyper") ? obj->e_type : o_hyper;
	obj->e_type = ft_strcmp(str, "sqr") ? obj->e_type : o_sqr;
	obj->e_type = ft_strcmp(str, "mandelbulb") ? obj->e_type : o_mandelbulb;
	if (obj->e_type == 0)
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_phong_param(t_object *obj, t_jnode *n)
{
	t_jnode *tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "al2"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG al2 ERROR", -1));
	obj->mat.al.y = jtoc_get_float(tmp);
	if (obj->mat.al.y < 0)
		return (rt_jtoc_sdl_log_error("PHONG al2 ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "sp_ex"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG sp_ex ERROR", -1));
	obj->mat.sp_ex = jtoc_get_float(tmp);
	if (obj->mat.sp_ex < 1)
		return (rt_jtoc_sdl_log_error("PHONG sp_ex ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_obj_color_param2(t_object *obj, t_jnode *n,
																t_jnode *tmp)
{
	if (obj->mat.reflection > 1.f)
		return (rt_jtoc_sdl_log_error("REFLECTION ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "refraction"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("REFRACTION ERROR OR MISSING", -1));
	obj->mat.refraction = jtoc_get_float(tmp);
	if (obj->mat.refraction < 1.f || obj->mat.refraction > 1.3f)
		obj->mat.refraction = 0.f;
	if (!(tmp = jtoc_node_get_by_path(n, "clouding"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("CLOUDING ERROR OR MISSING", -1));
	obj->mat.clouding = jtoc_get_float(tmp);
	if (obj->mat.clouding > 1.f || obj->mat.clouding < 0.96)
		obj->mat.clouding = 1.f;
	return (FUNCTION_SUCCESS);
}

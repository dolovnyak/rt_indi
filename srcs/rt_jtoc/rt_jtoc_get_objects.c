#include "rt.h"
#include "rt_jtoc.h"

int 		rt_jtoc_compare_str_with_texture_name(t_obj_texture *texture, char *str)
{
	int i;
	int cache_counter;

	i = -1;
	cache_counter = texture->textures_count;
	while (++i < cache_counter)
		if ((ft_strstr(texture->textures_path[i], str)))
			return (i);
	return (-2);
}
int 		rt_jtoc_get_object_texture(t_object *obj, t_obj_texture *texture, t_jnode *n)
{
	t_jnode	*tmp;
	int 	id;
	char 	*str;

	if (!(tmp = jtoc_node_get_by_path(n, "texture"))
		|| tmp->type != string)
		return (rt_jtoc_sdl_log_error("TEXTURE ERROR OR TEXTURE IS MISSING", -1));
	str = jtoc_get_string(tmp);
	if (!(ft_strcmp(str, "none")))
		id = -1;
	else
		id = rt_jtoc_compare_str_with_texture_name(texture, str);
	if (id == -2)
		return (rt_jtoc_sdl_log_error("TEXTURE NAME ERROR OR TEXTURE NAME IS MISSING", -1));
	obj->mat.texture_id = id;
	return (FUNCTION_SUCCESS);
}

int	rt_jtoc_get_object_type(t_object *obj, t_jnode *n)
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
	obj->type = obj->e_type - 1;
	if (obj->e_type == 0)
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int	rt_jtoc_phong_processing(t_object *obj, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(n = jtoc_node_get_by_path(n, "phong")) || n->type != object)
		return (rt_jtoc_sdl_log_error("PHONG TYPE ERROR OR PHONG IS MISSING", -1));

	if (!(tmp = jtoc_node_get_by_path(n, "al1")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG al1 ERROR", -1));
	obj->mat.al.x = jtoc_get_float(tmp);
	if (obj->mat.al.x <= 0)
		return (rt_jtoc_sdl_log_error("PHONG al1 ERROR", -1));

	if (!(tmp = jtoc_node_get_by_path(n, "al2")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG al2 ERROR", -1));
	obj->mat.al.y = jtoc_get_float(tmp);
	if (obj->mat.al.y < 0)
		return (rt_jtoc_sdl_log_error("PHONG al2 ERROR", -1));

	if (!(tmp = jtoc_node_get_by_path(n, "sp_ex")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("PHONG sp_ex ERROR", -1));
	obj->mat.sp_ex = jtoc_get_float(tmp);
	if (obj->mat.sp_ex < 1)
		return (rt_jtoc_sdl_log_error("PHONG sp_ex ERROR", -1));
	return (FUNCTION_SUCCESS);
}

int rt_jtoc_get_object(t_object *obj, t_jnode *n, t_obj_texture *texture)
{
	t_jnode	*tmp;
	int		err;

	ft_bzero(obj, sizeof(t_object));
	if (!(tmp = jtoc_node_get_by_path(n, "transform")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("TRANSFORM TYPE ERROR OR TRANSFORM MISSING", -1));
	if (!(tmp = jtoc_node_get_by_path(tmp, "pos")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("POS TYPE ERROR OR MISSING", -1));
	if (rt_jtoc_get_float3(&(obj->center), tmp))
		return (rt_jtoc_sdl_log_error("POS ERROR", -1));
	if (rt_jtoc_get_object_type(obj, n))
		return (rt_jtoc_sdl_log_error("NOT VALID TYPE", -1));
	if (rt_jtoc_get_object_texture(obj, texture , n))
		return(rt_jtoc_sdl_log_error("NOT VALID TEXTURE", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "color")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("COLOR TYPE ERROR OR COLOR IS MISSING", -1));
	if (rt_jtoc_get_float3(&obj->mat.diffuse_color, tmp))
		return (rt_jtoc_sdl_log_error("COLOR ERROR", -1));
	if ((obj->mat.diffuse_color.x < 0 || obj->mat.diffuse_color.y < 0 || obj->mat.diffuse_color.z < 0)
		|| obj->mat.diffuse_color.x > 1 || obj->mat.diffuse_color.y > 1 || obj->mat.diffuse_color.z > 1)
		return (rt_jtoc_sdl_log_error("COLOR ERROR", -1));
	if (rt_jtoc_phong_processing(obj, n))
		return (rt_jtoc_sdl_log_error("PHONG ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "emission")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("EMISSION ERROR OR MISSING", -1));
	if (rt_jtoc_get_float3(&obj->mat.emission, tmp))
		return (rt_jtoc_sdl_log_error("EMISSION PARAM ERROR", -1));
	if ((obj->mat.emission.x < 0 || obj->mat.emission.y < 0 || obj->mat.emission.z < 0))
		return (rt_jtoc_sdl_log_error("EMISSION ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "reflection")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("REFLECTION ERROR OR MISSING", -1));
	obj->mat.reflection = jtoc_get_float(tmp);
	if (obj->mat.reflection >= 1.f)
		return (rt_jtoc_sdl_log_error("REFLECTION ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "refraction")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("REFRACTION ERROR OR MISSING", -1));
	obj->mat.refraction = jtoc_get_float(tmp);

	err = 0;
	err = obj->e_type == o_sphere ? rt_jtoc_get_sphere(obj, n) : err;
	err = obj->e_type == o_torus ? rt_jtoc_get_torus(obj, n) : err;
	err = obj->e_type == o_plane ? rt_jtoc_get_plane(obj, n) : err;
	err = obj->e_type == o_cone ? rt_jtoc_get_cone(obj, n) : err;
	err = obj->e_type == o_cylinder ? rt_jtoc_get_cylinder(obj, n) : err;
	err = obj->e_type == o_hyper ? rt_jtoc_get_hyper(obj, n) : err;
	err = obj->e_type == o_sqr ? rt_jtoc_get_sqr(obj, n) : err;
	if (err != 0)
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}

int rt_jtoc_get_objects(t_rt *rt, t_jnode *n, t_obj_texture *texture)
{
	t_jnode		*tmp;
	t_object	*objects;
	int			i;
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
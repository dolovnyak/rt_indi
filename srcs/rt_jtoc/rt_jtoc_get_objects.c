#include "rt.h"
#include "rt_jtoc.h"
//#include "rt_raycast.h"
//
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
//	obj->type = ft_strcmp(str, "prm") ? obj->type : o_prm;
	if (obj->e_type == 0)
		return (FUNCTION_FAILURE);
	return (FUNCTION_SUCCESS);
}
//
//int	rt_jtoc_check_and_get_id_for_object(t_scene *scene, t_jnode *n, t_object *object)
//{
//	t_jnode		*tmp;
//	cl_uint		id;
//
//	if (!(tmp = jtoc_node_get_by_path(n, "id")) || tmp->type != integer)
//		return (rt_jtoc_sdl_log_error("ID ERROR", -1));
//		id = jtoc_get_int(tmp);
//		if (id <= 0)
//			return (FUNCTION_FAILURE);
//		if (scene->camera.transform.id == (cl_uint)id)
//			return (rt_jtoc_sdl_log_error("THAT ID ALREADY EXISTS IN CAMERA", id));
//		if (rt_find_light_by_id(scene->lights, id))
//			return (rt_jtoc_sdl_log_error("THAT ID ALREADY EXISTS IN LIGHTS", id));
//		if (rt_find_object_by_id(scene->objects, id))
//			return (rt_jtoc_sdl_log_error("THAT ID ALREADY EXISTS IN OBJECTS", id));
//	object->transform.id = id;
//	return (FUNCTION_SUCCESS);
//}

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

	err = 0;
	err = obj->e_type == o_sphere ? rt_jtoc_get_sphere(obj, n) : err;
	err = obj->e_type == o_torus ? rt_jtoc_get_torus(obj, n) : err;
	err = obj->e_type == o_plane ? rt_jtoc_get_plane(obj, n) : err;
	err = obj->e_type == o_cone ? rt_jtoc_get_cone(obj, n) : err;
	err = obj->e_type == o_cylinder ? rt_jtoc_get_cylinder(obj, n) : err;
//	if (err != 0)
//		return (FUNCTION_FAILURE);

	return (FUNCTION_SUCCESS);
}

int rt_jtoc_get_objects(t_mlx *mlx, t_jnode *n, t_obj_texture *texture)
{
	t_jnode		*tmp;
	t_object	*objects;
	int			i;

	mlx->objects_count = 0;
	if (rt_jtoc_get_objects_num_in_arr(&mlx->objects_count, n))
		return (FUNCTION_FAILURE);
	printf("%d\n", mlx->objects_count);
	objects = ft_memalloc(sizeof(t_object) * mlx->objects_count);
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
	mlx->obj = objects;
	return (FUNCTION_SUCCESS);
}
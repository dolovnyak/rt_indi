#include "rt.h"
#include "rt_jtoc.h"

static int 		rt_jtoc_get_texture_path(char **textures, t_jnode *n, int i)
{
	t_jnode	*tmp;
	char 	*str;

	if (!(tmp = jtoc_node_get_by_path(n, "path")) || tmp->type != string)
		return (rt_jtoc_sdl_log_error("PATH ERROR OR PATH IS MISSING", -1));
	str = jtoc_get_string(tmp);
	textures[i] = ft_memalloc(ft_strlen(str) + 1);
	ft_strcat(textures[i], str);
	return (FUNCTION_SUCCESS);
}

int			rt_jtoc_get_textures_by_path(t_obj_texture *texture, t_jnode *n)
{
	t_jnode		*tmp;
	char		**textures;
	int			i;

	texture->textures_count = 0;
	if (rt_jtoc_get_objects_num_in_arr(&texture->textures_count, n))
		return (FUNCTION_FAILURE);
	if (!(textures = (char **)ft_memalloc(sizeof(char*) * texture->textures_count)))
		return (FUNCTION_FAILURE);
	tmp = n->down;
	i = 0;
	while (tmp)
	{
		if (tmp->type != object)
			return (rt_jtoc_sdl_log_error("STRING TYPE ERROR", i));
		if (rt_jtoc_get_texture_path(textures, tmp, i))
			return (FUNCTION_FAILURE);
		i++;
		tmp = tmp->right;
	}
	texture->textures_path = textures;
	return (FUNCTION_SUCCESS);
}

int 	rt_jtoc_get_textures(const char *path, t_obj_texture *texture)
{
	t_jnode	*root;
	t_jnode	*tmp;

	if (!(root = jtoc_read(path)))
		return (rt_jtoc_sdl_log_error("JSON PATH ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "textures")) || tmp->type != array)
		return (rt_jtoc_sdl_log_error("TEXTURE TYPE ERROR OR TEXTURE IS MISSING", -1));
	if (rt_jtoc_get_textures_by_path(texture, tmp))
		return (rt_jtoc_sdl_log_error("SYKA ERROR", -1));
	jtoc_node_clear(root);
	return (FUNCTION_SUCCESS);
}

int		rt_jtoc_textures_setup(t_rt *rt, const char *json)
{
	int				i;
	t_obj_texture	*texture;

	if (!(texture = (t_obj_texture *)ft_memalloc(sizeof(t_obj_texture))))
		return (FUNCTION_FAILURE);
	i = -1;
	if (rt_jtoc_get_textures(json, texture))
	{
		rt_jtoc_sdl_log_error("TEXTURE ERROR", i);
		exit(-1);
	}
	rt->texture = texture;
	return (FUNCTION_SUCCESS);
}
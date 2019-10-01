#include "rt.h"
#include "rt_jtoc.h"


static int	rt_jtoc_get_scene(const char *path, t_rt *rt, t_obj_texture *texture)
{
	t_jnode	*root;
	t_jnode	*tmp;

	if (!(root = jtoc_read(path)))
		return (rt_jtoc_sdl_log_error("JSON", -1));

	if (!(tmp = jtoc_node_get_by_path(root, "camera")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("CAMERA TYPE ERROR OR CAMERA IS NOT SET", -1));
	if (rt_jtoc_get_camera(&(rt->cam), tmp))
		return (rt_jtoc_sdl_log_error("CAMERA DATA ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "lights")) || tmp->type != array)
		return (rt_jtoc_sdl_log_error("LIGHTS ERROR OR LIGHTS MISSING", -1));
	if (rt_jtoc_get_lights(rt, tmp))
		return (rt_jtoc_sdl_log_error("LIGHTS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "effects")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("EFFECTS ERROR OR EFFECTS MISSING", -1));
	if (rt_jtoc_get_effects(rt, tmp))
		return (rt_jtoc_sdl_log_error("EFFECTS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "objects")) || tmp->type != array)
		return (rt_jtoc_sdl_log_error("OBJECTS TYPE ERROR OR OBJECTS AREN'T SET", -1));
	if (rt_jtoc_get_objects(rt, tmp, texture))
		return (rt_jtoc_sdl_log_error("OBJECTS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "antialiasing")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("ANTIALIASING ERRROR OR ANTIALIASING MISSING", -1));
	rt->screen.fsaa_n = jtoc_get_int(tmp);
	if (rt->screen.fsaa_n % 2 != 0)
		rt->screen.fsaa_n += 1;

	jtoc_node_clear(root);
	return (FUNCTION_SUCCESS);
}

int rt_jtoc_scene_setup(t_rt *rt, const char *json)
{
	int		i;
	t_rt	*tmp_rt;

	tmp_rt = rt;
	i = -1;
	if (rt_jtoc_get_scene(json, rt, rt->texture))
	{
		rt_jtoc_sdl_log_error("SCENE ERROR", i);
		exit(-1);
	}
	rt = tmp_rt;
	return (FUNCTION_SUCCESS);
}
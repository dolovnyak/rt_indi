#include "rt.h"
#include "rt_jtoc.h"

char		*g_err_str;

static int		rt_jtoc_get_posteffects(t_rt *rt, t_jnode *root)
{
	t_jnode *tmp;

	if ((!(tmp = jtoc_node_get_by_path(root, "ambient"))
		|| tmp->type != fractional))
		return (rt_jtoc_sdl_log_error("AMBIENT TYPE ERROR OR NOT SET", -1));
	rt->ambient = jtoc_get_float(tmp);
	if (rt->ambient < 0.f || rt->ambient > 1.f)
		return (rt_jtoc_sdl_log_error("AMBIENT ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "antialiasing"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error("ANTIALIASING ERRROR OR MISSING", -1));
	rt->screen.fsaa_n = jtoc_get_int(tmp);
	rt->screen.fsaa_n % 2 != 0 ? rt->screen.fsaa_n++ : 0;
	rt->screen.fsaa_n > 6 || rt->screen.fsaa_n < 0 ? rt->screen.fsaa_n = 0 : 0;
	if (!(tmp = jtoc_node_get_by_path(root, "brightness"))
		|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("BRIGHTNESS ERROR", -1));
	rt->screen.brightness = jtoc_get_float(tmp);
	if (rt->screen.brightness < 0 || rt->screen.brightness > 5)
		rt->screen.brightness = 0;
	if (!(tmp = jtoc_node_get_by_path(root, "effects"))
	|| tmp->type != object)
		return (rt_jtoc_sdl_log_error("EFFECTS ERROR OR EFFECTS MISSING", -1));
	return (rt_jtoc_get_effects(rt, tmp) ?
		rt_jtoc_sdl_log_error("EFFECTS ERROR", -1) : FUNCTION_SUCCESS);
}

static int		rt_jtoc_get_samples(t_rt *rt, t_jnode *root)
{
	t_jnode *tmp;

	g_err_str = "SAMPLES ERRROR OR SAMPLES MISSING";
	if (!(tmp = jtoc_node_get_by_path(root, "samples"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	rt->screen.samples = jtoc_get_int(tmp);
	if (rt->screen.samples < 2 || rt->screen.samples > 900)
		return (rt_jtoc_sdl_log_error("SAMPLES ERROR", -1));
	return (FUNCTION_SUCCESS);
}

static int		rt_jtoc_get_scene(const char *path, t_rt *rt,
				t_obj_texture *texture)
{
	t_jnode	*root;
	t_jnode	*tmp;

	if (!(root = jtoc_read(path)))
		return (rt_jtoc_sdl_log_error("JSON", -1));
	g_err_str = "CAMERA TYPE ERROR OR CAMERA IS NOT SET";
	if (!(tmp = jtoc_node_get_by_path(root, "camera"))
	|| tmp->type != object)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (rt_jtoc_get_camera(&(rt->cam), tmp))
		return (rt_jtoc_sdl_log_error("CAMERA DATA ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "lights"))
	|| tmp->type != array)
		return (rt_jtoc_sdl_log_error("LIGHTS ERROR OR LIGHTS MISSING", -1));
	if (rt_jtoc_get_lights(rt, tmp))
		return (rt_jtoc_sdl_log_error("LIGHTS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "objects"))
	|| tmp->type != array)
		return (rt_jtoc_sdl_log_error("OBJECTS TYPE ERROR OR AREN'T SET", -1));
	if (rt_jtoc_get_objects(rt, tmp, texture))
		return (rt_jtoc_sdl_log_error("OBJECTS ERROR", -1));
	if (rt_jtoc_get_posteffects(rt, root) || rt_jtoc_get_samples(rt, root))
		return (FUNCTION_FAILURE);
	jtoc_node_clear(root);
	return (FUNCTION_SUCCESS);
}

int				rt_jtoc_scene_setup(t_rt *rt, const char *json)
{
	int		i;

	i = -1;
	if (rt_jtoc_get_scene(json, rt, rt->texture))
	{
		rt_jtoc_sdl_log_error("SCENE ERROR", i);
		exit(-1);
	}
	return (FUNCTION_SUCCESS);
}

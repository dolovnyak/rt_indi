#include "rt.h"
#include "rt_jtoc.h"

int 			rt_jtoc_get_mouse(t_mouse *mouse, const char *path)
{
	t_jnode	*root;
	t_jnode	*tmp;

	if (!(root = jtoc_read(path)))
		return (rt_jtoc_sdl_log_error("JSON PATH ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_r")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("MOUSE_R ERROR OR MOUSE_R MISSING", -1));
	mouse->r = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_l")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("MOUSE_L ERROR OR MOUSE_L MISSING", -1));
	mouse->l = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_x")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("MOUSE_X ERROR OR MOUSE_X MISSING", -1));
	mouse->x = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_y")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("MOUSE_Y ERROR OR MOUSE_Y MISSING", -1));
	mouse->y = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_alpha")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("MOUSE_ALPHA ERROR OR MOUSE_ALPHA MISSING", -1));
	mouse->alpha = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_betta")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("MOUSE_BETTA ERROR OR MOUSE_BETTA MISSING", -1));
	mouse->betta = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(root, "mouse_center")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("MOUSE_CENTER ERROR OR MOUSE_CENTER MISSING", -1));
	if (rt_jtoc_get_float3(&mouse->center, tmp))
		return (rt_jtoc_sdl_log_error("MOUSE_CENTER PARAM ERROR OR MISSING", -1));
	return (FUNCTION_SUCCESS);
}

int 			rt_jtoc_mouse_setup(t_mlx *mlx, const char *json)
{
	int				i;
	t_mouse			mouse;

	i = -1;
	if (rt_jtoc_get_mouse(&mouse, json))
	{
		rt_jtoc_sdl_log_error("MOUSE ERROR", i);
		exit(-1);
	}
	mlx->mouse = mouse;
	return (FUNCTION_SUCCESS);
}

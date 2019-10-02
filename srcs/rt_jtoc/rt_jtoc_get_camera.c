
#include "rt.h"
#include "rt_jtoc.h"

char	*g_err_str;

int			rt_jtoc_get_camera(t_cam *camera, t_jnode *n)
{
	t_jnode		*tmp;

	ft_bzero(camera, sizeof(t_cam));
	g_err_str = "TRANSFORM TYPE ERROR OR TRANSFORM MISSING";
	if (!(n = jtoc_node_get_by_path(n, "transform")) || n->type != object)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	if (!(tmp = jtoc_node_get_by_path(n, "pos")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("POS TYPE ERROR OR MISSING", -1));
	if (rt_jtoc_get_float3(&(camera->center), tmp))
		return (rt_jtoc_sdl_log_error("POS ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "alpha"))
											|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("ALPHA ERROR", -1));
	camera->alpha = jtoc_get_float(tmp);
	if (camera->alpha < 0 || camera->alpha > 2 * M_PI)
		camera->alpha = 0;
	if (!(tmp = jtoc_node_get_by_path(n, "betta"))
											|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("BETTA ERROR", -1));
	camera->betta = jtoc_get_float(tmp);
	if (camera->betta < 0 || camera->betta > M_PI)
		camera->betta = M_PI_2;
	return (FUNCTION_SUCCESS);
}


#include "rt.h"
#include "rt_jtoc.h"

char	*g_err_str;

int		rt_jtoc_get_effects(t_rt *rt, t_jnode *n)
{
	t_jnode *tmp;

	g_err_str = "EFFECTS NAME ERROR OR EFFECTS MISSING";
	if (!(tmp = jtoc_node_get_by_path(n, "gray"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	rt->screen.effects.s0 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "3d"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	rt->screen.effects.s1 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "gauss_blure"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	rt->screen.effects.s2 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "dithering"))
		|| tmp->type != integer)
		return (rt_jtoc_sdl_log_error(g_err_str, -1));
	rt->screen.effects.s3 = jtoc_get_int(tmp);
	return (FUNCTION_SUCCESS);
}

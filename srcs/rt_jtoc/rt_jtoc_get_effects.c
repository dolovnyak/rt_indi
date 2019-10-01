#include "rt.h"
#include "rt_jtoc.h"

int 	rt_jtoc_get_effects(t_rt *rt, t_jnode *n)
{
	t_jnode *tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "1")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("EFFECTS NAME ERROR OR EFFECTS MISSING", -1));
	rt->screen.effects.s0 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "2")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("EFFECTS NAME ERROR OR EFFECTS MISSING", -1));
	rt->screen.effects.s1 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "3")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("EFFECTS NAME ERROR OR EFFECTS MISSING", -1));
	rt->screen.effects.s2 = jtoc_get_int(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "4")) || tmp->type != integer)
		return (rt_jtoc_sdl_log_error("EFFECTS NAME ERROR OR EFFECTS MISSING", -1));
	rt->screen.effects.s3 = jtoc_get_int(tmp);
	return (FUNCTION_SUCCESS);
}

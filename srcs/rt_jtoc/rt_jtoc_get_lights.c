
#include "rt.h"
#include "rt_jtoc.h"

int	rt_jtoc_get_light(t_light *light, t_jnode *n)
{
	t_jnode *tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "pos")) || tmp->type != object)
		return (rt_jtoc_sdl_log_error("POS ERROR OR POS MISSING", -1));
	if (rt_jtoc_get_float3(&light->center, tmp))
		return (rt_jtoc_sdl_log_error("POS PARAM ERROR", -1));
	if (!(tmp = jtoc_node_get_by_path(n, "intens"))
												|| tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("INTENS ERROR OR INTENS MISSING",
				-1));
	light->intens = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}

int rt_jtoc_get_lights(t_rt *rt, t_jnode *n)
{
	t_jnode		*tmp;
	t_light		*lights;
	int			i;
	unsigned int g;

	g = 0;
	if (rt_jtoc_get_objects_num_in_arr(&g, n))
		return (FUNCTION_FAILURE);
	rt->counter.l = g;
	lights = ft_memalloc(sizeof(t_light) * rt->texture->textures_count);
	i = 0;
	tmp = n->down;
	while (tmp)
	{
		if (tmp->type != object)
			return (rt_jtoc_sdl_log_error(
					"LIGHTS IS NOT SET OR LIGHT TYPE ERROR",i));
		if (rt_jtoc_get_light(&(lights[i]), tmp))
			return (rt_jtoc_sdl_log_error("LIGHT DATA ERROR", i));
		i++;
		tmp = tmp->right;
	}
	rt->light = lights;
	return (FUNCTION_SUCCESS);
}

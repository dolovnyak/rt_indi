
#include "rt.h"

int		ft_esc(t_rt *rt)
{
	release_gpu_mem(rt);
	mlx_destroy_image(rt->mlx_ptr, rt->img.img_ptr);
	mlx_destroy_window(rt->mlx_ptr, rt->win);
	exit(0);
	return (0);
}

int		ft_aa(int keycode, t_rt *rt)
{
	rt->screen.fsaa_n = 2 * (keycode - 18);
	return (0);
}

int		ft_effects(int keycode, t_rt *rt)
{
	if (keycode == 6)
		rt->screen.effects.x = (rt->screen.effects.x + 1) % 2;
	else if (keycode == 7)
	{
		rt->screen.effects.w = 0;
		rt->screen.effects.y = (rt->screen.effects.y + 1) % 2;
	}
	else if (keycode == 8)
		rt->screen.effects.z = (rt->screen.effects.z + 1) % 2;
	else if (keycode == 9)
	{
		rt->screen.effects.y = 0;
		rt->screen.effects.w = (rt->screen.effects.w + 1) % 2;
	}
	else if (keycode == 37 && (rt->screen.params & PATH_TRACE))
	{
		rt->screen.params &= !(PATH_TRACE);
		rt->screen.params |= PHONG;
	}
	else if (keycode == 37)
	{
		rt->screen.params &= !(PHONG);
		rt->screen.params |= PATH_TRACE;
	}
	return (0);
}

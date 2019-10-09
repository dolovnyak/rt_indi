/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 16:43:50 by broggo            #+#    #+#             */
/*   Updated: 2019/09/30 01:23:12 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_jtoc.h"

int		fps(t_rt *rt)
{
	float	s;
	char	*str;

	if (rt->fps.stop.tv_usec - rt->fps.start.tv_usec > 0)
	{
		s = 1000000.0 / (double)(rt->fps.stop.tv_usec - rt->fps.start.tv_usec);
		str = ft_itoa(s);
		mlx_string_put(rt->mlx_ptr, rt->win, 60, 10, 0x000000, str);
		free(str);
	}
	return (1);
}

void	draw_picture(t_rt *rt)
{
	calc_screen(&rt->screen, &rt->cam);
	cl_worker(rt);
	mlx_clear_window(rt->mlx_ptr, rt->win);
	mlx_put_image_to_window(rt->mlx_ptr, rt->win, rt->img.img_ptr, 0, 0);
	mlx_string_put(rt->mlx_ptr, rt->win, 10, 10, 0x000000, "FPS: ");
	fps(rt);
}

int		new_mlx(t_rt *rt)
{
	rt->mlx_ptr = mlx_init();
	rt->win = mlx_new_window(rt->mlx_ptr, WIDTH, HEIGHT, "RT");
	rt->img.img_ptr = mlx_new_image(rt->mlx_ptr, WIDTH, HEIGHT);
	rt->img.data = (int *)mlx_get_data_addr(rt->img.img_ptr,
			&rt->img.bpp, &rt->img.size_l, &rt->img.endian);
	return (1);
}

void	cl_setup_program(t_rt *rt)
{
	rt->cl = cl_setup((char *[]){"scls/phong_render.cl",
							"scls/scene_intersect.cl",
							"scls/uv_mapping.cl",
							"scls/utilities.cl",
							NULL},
								(char *[]){"phong_render", NULL}, NULL);
	cl_setup((char *[]){"scls/scene_intersect.cl",
						"scls/uv_mapping.cl",
						"scls/utilities.cl",
						"scls/path_trace_render.cl", NULL},
		(char *[]){"path_trace_render", NULL}, rt->cl);
	cl_setup((char *[]){"scls/post_processing.cl", "scls/utilities.cl",
						NULL},
		(char *[]){"post_processing", "gauss_blur_x",
						"gauss_blur_y", NULL}, rt->cl);
}

int		main(int argc, char **argv)
{
	t_rt		*rt;

	if (!(rt = (t_rt *)ft_memalloc(sizeof(t_rt))))
		exit(-1);
	ft_bzero(rt, sizeof(t_rt));
	if (argc != 3)
	{
		ft_putstr("usage: ./RT path_map path_texture\n");
		exit(-1);
	}
	cl_setup_program(rt);
	if (!(new_mlx(rt)))
		exit(-1);
	rt_jtoc_textures_setup(rt, argv[2]);
	rt_jtoc_scene_setup(rt, argv[1]);
	rt->screen.params |= PHONG;
	fill_gpu_mem(rt);
	draw_picture(rt);
	mlx_hook(rt->win, 2, 0, check_key, rt);
	mlx_hook(rt->win, 17, 0, ft_esc, rt);
	mlx_hook(rt->win, 4, 0, mouse_press, rt);
	mlx_hook(rt->win, 5, 0, mouse_release, rt);
	mlx_hook(rt->win, 6, 0, mouse_move, rt);
	mlx_loop(rt->mlx_ptr);
	return (0);
}

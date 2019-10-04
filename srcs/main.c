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

# include "rt.h"
# include "rt_jtoc.h"
#include <sys/time.h>

void	draw_picture(t_rt *rt)
{
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	calc_screen(&rt->screen, &rt->cam);
	cl_worker(rt);
	mlx_put_image_to_window(rt->mlx_ptr, rt->win, rt->img.img_ptr, 0, 0);
	
	gettimeofday(&stop, NULL);
//	printf("took %u\n", stop.tv_usec - start.tv_usec);
//	printf("------------------ \n");

}

int	new_mlx(t_rt *rt, char *name)
{
	rt->mlx_ptr = mlx_init();
	rt->win = mlx_new_window(rt->mlx_ptr, WIDTH, HEIGHT, "RT");
	rt->img.img_ptr = mlx_new_image(rt->mlx_ptr, WIDTH, HEIGHT);
	rt->img.data = (int *)mlx_get_data_addr(rt->img.img_ptr,
			&rt->img.bpp, &rt->img.size_l, &rt->img.endian);
	(void)name;

	return (1);
}

void		release_gpu_mem(t_rt *rt)
{
	clReleaseProgram(*rt->cl->program);
	clReleaseContext(*rt->cl->context);
	clReleaseCommandQueue(*rt->cl->queue);
	clReleaseMemObject(rt->gpu_mem->cl_texture);
	clReleaseMemObject(rt->gpu_mem->cl_texture_w);
	clReleaseMemObject(rt->gpu_mem->cl_texture_h);
	clReleaseMemObject(rt->gpu_mem->cl_prev_texture_size);
	clReleaseMemObject(rt->gpu_mem->cl_img_buffer);
	clReleaseMemObject(rt->gpu_mem->cl_aux_buffer);
	clReleaseMemObject(rt->gpu_mem->cl_light_buffer);
	clReleaseMemObject(rt->gpu_mem->cl_obj_buffer);
	clReleaseMemObject(rt->gpu_mem->cl_counter_buffer);
}

int			main(int argc, char **argv)
{
	t_rt		rt;

	ft_bzero(&rt, sizeof(t_rt));
	if (argc != 3)
    {
	    ft_putstr("usage: ./RT path_map path_texture\n");
	    exit (-1);
    }

	rt.cl = cl_setup((char *[]){"scls/rt.cl", "scls/post_processing.cl", NULL},
			(char *[]){"post_processing", "gauss_blur_x", "gauss_blur_y", "rt", NULL});
	if (new_mlx(&rt, argv[1]))
	{

		rt_jtoc_textures_setup(&rt, argv[2]);
		rt_jtoc_scene_setup(&rt, argv[1]);


		rt.screen.params |= PHONG;
		fill_gpu_mem(&rt);
		draw_picture(&rt);
		mlx_hook(rt.win, 2, 0, check_key, &rt);
		mlx_hook(rt.win, 17, 0, ft_esc, &rt);
		mlx_hook(rt.win, 4, 0, mouse_press, &rt);
		mlx_hook(rt.win, 5, 0, mouse_release, &rt);
		mlx_hook(rt.win, 6, 0, mouse_move, &rt);
		mlx_loop(rt.mlx_ptr);
	}
	return (0);
}


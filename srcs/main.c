/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/08 16:43:50 by broggo            #+#    #+#             */
/*   Updated: 2019/09/29 22:55:55 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rt.h"
# include "rt_jtoc.h"
#include <sys/time.h>

void	draw_picture(t_mlx *mlx)
{
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	
	calc_screen(&mlx->screen, &mlx->cam);
	cl_worker(mlx);
	
	mlx_put_image_to_window(mlx->mlx_ptr, mlx->win, mlx->img.img_ptr, 0, 0);
	
	gettimeofday(&stop, NULL);
	printf("took %u\n", stop.tv_usec - start.tv_usec);
	printf("------------------ \n");

}

void	default_mlx(t_mlx *mlx)
{
	mlx->cam.center.x = -40.f;
	mlx->cam.center.y = 0.f;
	mlx->cam.center.z = 0.f;
	mlx->cam.alpha = 0.f;
	mlx->cam.betta = (float)M_PI_2;
	mlx->screen.fsaa_n = 0;
	cl_int8 effects = {{ 0, 0, 0, 0, 0, 0, 0, 0}};
	mlx->screen.effects = effects;
	mlx->mouse.r = 0;
	mlx->mouse.l = 0;
	mlx->mouse.x = 0;
	mlx->mouse.y = 0;
	mlx->mouse.alpha = 0.f;
	mlx->mouse.betta = 0.f;
	mlx->mouse.center.x = 0.f;
	mlx->mouse.center.y = 0.f;
	mlx->mouse.center.z = 0.f;
}

int	new_mlx(t_mlx *mlx, char *name)
{
	t_cl_arg	cl_arg;
	cl_int		err;

	default_mlx(mlx);
	mlx->mlx_ptr = mlx_init();
	mlx->win = mlx_new_window(mlx->mlx_ptr, WIDTH, HEIGHT, "RT");
	mlx->img.img_ptr = mlx_new_image(mlx->mlx_ptr, WIDTH, HEIGHT);
	mlx->img.data = (int *)mlx_get_data_addr(mlx->img.img_ptr,
											 &mlx->img.bpp, &mlx->img.size_l, &mlx->img.endian);
	cl_arg.device = create_device();
	cl_arg.context = clCreateContext(NULL, 1, &cl_arg.device, NULL, NULL, &err);
	if(err < 0) {
		perror("Couldn't create a context");
		exit(1);
	}
	cl_arg.program = build_program(cl_arg.context, cl_arg.device, PROGRAM_FILE);
	mlx->cl = cl_arg;
	
	if (!(read_map(name, mlx)))
		return (0);
	return (1);
}

void		emission(t_mlx *mlx)
{
	mlx->obj[0].mat.emission.x = 0.f;
	mlx->obj[0].mat.emission.y = 0.f;
	mlx->obj[0].mat.emission.z = 0.f;

	mlx->obj[1].mat.emission.x = 0.f;
	mlx->obj[1].mat.emission.y = 0.f;
	mlx->obj[1].mat.emission.z = 0.f;

	mlx->obj[2].mat.emission.x = 1.f;
	mlx->obj[2].mat.emission.y = 1.f;
	mlx->obj[2].mat.emission.z = 1.f;

	mlx->obj[3].mat.emission.x = 0.f;
	mlx->obj[3].mat.emission.y = 0.f;
	mlx->obj[3].mat.emission.z = 0.f;

	mlx->obj[4].mat.emission.x = 0.f;
	mlx->obj[4].mat.emission.y = 0.f;
	mlx->obj[4].mat.emission.z = 0.f;

	mlx->obj[5].mat.emission.x = 0.f;
	mlx->obj[5].mat.emission.y = 0.f;
	mlx->obj[5].mat.emission.z = 0.f;

	mlx->obj[6].mat.emission.x = 0.f;
	mlx->obj[6].mat.emission.y = 0.f;
	mlx->obj[6].mat.emission.z = 0.f;

	mlx->obj[7].mat.emission.x = 0.f;
	mlx->obj[7].mat.emission.y = 0.f;
	mlx->obj[7].mat.emission.z = 0.f;

	mlx->obj[8].mat.emission.x = 0.f;
	mlx->obj[8].mat.emission.y = 0.f;
	mlx->obj[8].mat.emission.z = 0.f;

	mlx->obj[9].mat.emission.x = 0.f;
	mlx->obj[9].mat.emission.y = 0.f;
	mlx->obj[9].mat.emission.z = 0.f;
}

int			main(int argc, char **argv)
{
	t_mlx		mlx;

	if (argc == 2)
	{
		if (new_mlx(&mlx, argv[1]))
		{
			mlx.aux = (int *)malloc(sizeof(int) * WIDTH * HEIGHT);
			emission(&mlx);
			rt_jtoc_textures_setup(&mlx, "json/textures.json");
			fill_gpu_mem(&mlx);
			draw_picture(&mlx);
			mlx_hook(mlx.win, 2, 0, check_key, &mlx);
			mlx_hook(mlx.win, 17, 0, ft_esc, &mlx);
			mlx_hook(mlx.win, 4, 0, mouse_press, &mlx);
			mlx_hook(mlx.win, 5, 0, mouse_release, &mlx);
			mlx_hook(mlx.win, 6, 0, mouse_move, &mlx);
			
			mlx_loop(mlx.mlx_ptr);
			
			clReleaseProgram(mlx.cl.program);
			clReleaseContext(mlx.cl.context);
			clReleaseCommandQueue(mlx.cl.queue);
		}
	}
	ft_putstr("usage: ./RT path_file\n");
	return (0);
}


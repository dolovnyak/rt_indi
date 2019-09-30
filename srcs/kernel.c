/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 21:03:50 by broggo            #+#    #+#             */
/*   Updated: 2019/09/22 22:01:53 by rkeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rt.h"

void		render(t_rt *rt, cl_mem screen_buffer, size_t global_size[2], size_t local_size[2])
{
	cl_mem		cam_buffer;
	cl_kernel	*kernel;
	cl_int2		rands;
	cl_int		err;

	cam_buffer = clCreateBuffer(*rt->cl->context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_cam), &(rt->cam), &err);
	cl_error_handler("Couldn't create cam buffer", err);
	kernel = cl_get_kernel_by_name(rt->cl, "rt");
	rands.x = rand();
	rands.y = rand();
	err = clSetKernelArg(*kernel, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel, 1, sizeof(cl_mem), &cam_buffer);
	err |= clSetKernelArg(*kernel, 2, sizeof(cl_mem), &screen_buffer);
	err |= clSetKernelArg(*kernel, 3, sizeof(cl_mem), &rt->gpu_mem->cl_counter_buffer);
	err |= clSetKernelArg(*kernel, 4, sizeof(cl_mem), &rt->gpu_mem->cl_light_buffer);
	err |= clSetKernelArg(*kernel, 5, sizeof(cl_mem), &rt->gpu_mem->cl_obj_buffer);
	err |= clSetKernelArg(*kernel, 6, sizeof(cl_int2), &rands);
	err |= clSetKernelArg(*kernel, 7, sizeof(cl_mem), &rt->gpu_mem->cl_texture);
	err |= clSetKernelArg(*kernel, 8, sizeof(cl_mem), &rt->gpu_mem->cl_texture_w);
	err |= clSetKernelArg(*kernel, 9, sizeof(cl_mem), &rt->gpu_mem->cl_texture_h);
	err |= clSetKernelArg(*kernel, 10, sizeof(cl_mem), &rt->gpu_mem->cl_prev_texture_size);
	cl_error_handler("Couldn't create a kernel rt argument", err);
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	cl_error_handler("Couldn't enqueue the kernel rt", err);
	clReleaseMemObject(cam_buffer);
}

void		gauss_blur(t_rt *rt, cl_mem screen_buffer, size_t global_size[2], size_t local_size[2])
{
	cl_kernel	*kernel;
	cl_int		err;

	err = 0;
	kernel = cl_get_kernel_by_name(rt->cl, "gauss_blur_x");
	err |= clSetKernelArg(*kernel, 0, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
	err |= clSetKernelArg(*kernel, 1, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel, 2, sizeof(cl_mem), &screen_buffer);
	cl_error_handler("Couldn't create a kernel gauss_blur_x argument", err);
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	cl_error_handler("Couldn't enqueue the kernel gauss_blur_x", err);

	kernel = cl_get_kernel_by_name(rt->cl, "gauss_blur_y");
	err |= clSetKernelArg(*kernel, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel, 1, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
	err |= clSetKernelArg(*kernel, 2, sizeof(cl_mem), &screen_buffer);
	cl_error_handler("Couldn't create a kernel gauss_blur_y argument", err);
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	cl_error_handler("Couldn't enqueue the kernel gauss_blur_y", err);

	err = clEnqueueReadBuffer(*rt->cl->queue, rt->gpu_mem->cl_img_buffer, CL_TRUE, 0,
							  WIDTH * HEIGHT * sizeof(int), rt->img.data, 0, NULL, NULL);
	cl_error_handler("Couldn't read the gauss_blur buffer", err);
}

void		post_processing(t_rt *rt, cl_mem screen_buffer, size_t global_size[2], size_t local_size[2])
{
	cl_kernel	*kernel;
	cl_int		err;

	kernel = cl_get_kernel_by_name(rt->cl, "post_processing");
	err = 0;
	err |= clSetKernelArg(*kernel, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel, 1, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
	err |= clSetKernelArg(*kernel, 2, sizeof(cl_mem), &screen_buffer);
	cl_error_handler("Couldn't create a kernel post_processing argument", err);
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel, 2, NULL, global_size,
			local_size, 0, NULL, NULL);
	cl_error_handler("Couldn't enqueue the kernel post_processing", err);

	if (rt->screen.effects.z == 1)
		gauss_blur(rt, screen_buffer, global_size, local_size);
}

int			cl_worker(t_rt *rt)
{
	cl_mem				screen_buffer;
	cl_int				err;
	size_t				global_size[2];
	size_t				local_size[2];

	global_size[0] = WIDTH;
	global_size[1] = HEIGHT;
	local_size[0] = 8;		//	надо как-то нормально ставить
	local_size[1] = 8;		//	вроде бы можно посчитать количество work групп. и просто WIDTH / work_group_count

	screen_buffer = clCreateBuffer(*rt->cl->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_screen), &(rt->screen), &err);
	cl_error_handler("Couldn't create screen buffer", err);

	render(rt, screen_buffer, global_size, local_size);
	post_processing(rt, screen_buffer, global_size, local_size);

	err = clEnqueueReadBuffer(*rt->cl->queue, rt->gpu_mem->cl_img_buffer, CL_TRUE, 0,
			WIDTH * HEIGHT * sizeof(int), rt->img.data, 0, NULL, NULL);
	cl_error_handler("Couldn't read the final buffer", err);

	clReleaseMemObject(screen_buffer);
	return (err);
}


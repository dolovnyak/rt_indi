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

int			cl_worker(t_rt *rt)
{
	cl_mem				cam_buffer;
	cl_mem				screen_buffer;
	cl_mem				counter_buffer;	//
	cl_int				err;
	size_t				global_size[2];
	size_t				local_size[2];
	cl_kernel			*kernel_rt;
	cl_kernel			*kernel_pp;
	cl_kernel			*kernel_gb1;
	cl_kernel			*kernel_gb2;

	global_size[0] = WIDTH;
	global_size[1] = HEIGHT;
	local_size[0] = 8;		//	надо как-то нормально ставить
	local_size[1] = 8;		//	вроде бы можно посчитать количество work групп. и просто WIDTH / work_group_count

	cam_buffer = clCreateBuffer(*rt->cl->context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_cam), &(rt->cam), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	screen_buffer = clCreateBuffer(*rt->cl->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_screen), &(rt->screen), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	counter_buffer = clCreateBuffer(*rt->cl->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_counter), &(rt->counter), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};

	kernel_rt = cl_get_kernel_by_name(rt->cl, "rt");

	cl_int2 rands;
	rands.x = rand();
	rands.y = rand();
	err = clSetKernelArg(*kernel_rt, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel_rt, 1, sizeof(cl_mem), &cam_buffer);
	err |= clSetKernelArg(*kernel_rt, 2, sizeof(cl_mem), &screen_buffer);
	err |= clSetKernelArg(*kernel_rt, 3, sizeof(cl_mem), &counter_buffer);
	err |= clSetKernelArg(*kernel_rt, 4, sizeof(cl_mem), &rt->gpu_mem->cl_light_buffer);
	err |= clSetKernelArg(*kernel_rt, 5, sizeof(cl_mem), &rt->gpu_mem->cl_obj_buffer);
	err |= clSetKernelArg(*kernel_rt, 6, sizeof(cl_int2), &rands);
	err |= clSetKernelArg(*kernel_rt, 7, sizeof(cl_mem), &rt->gpu_mem->cl_texture);
	err |= clSetKernelArg(*kernel_rt, 8, sizeof(cl_mem), &rt->gpu_mem->cl_texture_w);
	err |= clSetKernelArg(*kernel_rt, 9, sizeof(cl_mem), &rt->gpu_mem->cl_texture_h);
	err |= clSetKernelArg(*kernel_rt, 10, sizeof(cl_mem), &rt->gpu_mem->cl_prev_texture_size);
	if(err < 0) {
		perror("Couldn't create a kernel argument");
		exit(1);
	}
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel_rt, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't enqueue the kernel");
		exit(1);
	}


	kernel_pp = cl_get_kernel_by_name(rt->cl, "post_processing");
	err |= clSetKernelArg(*kernel_pp, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel_pp, 1, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
	err |= clSetKernelArg(*kernel_pp, 2, sizeof(cl_mem), &screen_buffer);
	if(err < 0) {
		perror("Couldn't create a kernel argument");
		exit(1);
	}
	err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel_pp, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't enqueue the kernel");
		exit(1);
	}


	if (rt->screen.effects.z == 1)
	{
		kernel_gb1 = cl_get_kernel_by_name(rt->cl, "gauss_blur_x");
		err |= clSetKernelArg(*kernel_gb1, 0, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
		err |= clSetKernelArg(*kernel_gb1, 1, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
		err |= clSetKernelArg(*kernel_gb1, 2, sizeof(cl_mem), &screen_buffer);
		if(err < 0) {
			perror("Couldn't create a kernel argument");
			exit(1);
		}
		err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel_gb1, 2, NULL, global_size,
									 local_size, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't enqueue the kernel");
			exit(1);
		}

		kernel_gb2 = cl_get_kernel_by_name(rt->cl, "gauss_blur_y");
		err |= clSetKernelArg(*kernel_gb2, 0, sizeof(cl_mem), &rt->gpu_mem->cl_aux_buffer);
		err |= clSetKernelArg(*kernel_gb2, 1, sizeof(cl_mem), &rt->gpu_mem->cl_img_buffer);
		err |= clSetKernelArg(*kernel_gb2, 2, sizeof(cl_mem), &screen_buffer);
		if(err < 0) {
			perror("Couldn't create a kernel argument");
			exit(1);
		}
		err = clEnqueueNDRangeKernel(*rt->cl->queue, *kernel_gb2, 2, NULL, global_size,
									 local_size, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't enqueue the kernel");
			exit(1);
		}
		err = clEnqueueReadBuffer(*rt->cl->queue, rt->gpu_mem->cl_img_buffer, CL_TRUE, 0,
								  WIDTH * HEIGHT * sizeof(int), rt->img.data, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't read the buffer");
			exit(1);
		}
	}

	err = clEnqueueReadBuffer(*rt->cl->queue, rt->gpu_mem->cl_img_buffer, CL_TRUE, 0,
			WIDTH * HEIGHT * sizeof(int), rt->img.data, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(1);
	}

	clReleaseMemObject(cam_buffer);
	clReleaseMemObject(screen_buffer);
	clReleaseMemObject(counter_buffer);

	return (err);
}


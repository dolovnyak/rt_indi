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

cl_device_id create_device() {

	cl_platform_id platform;
	cl_device_id dev;
	int err;

	/* Identify a platform */
	err = clGetPlatformIDs(1, &platform, NULL);
	if(err < 0) {
		perror("Couldn't identify a platform");
		exit(1);
	}

	// Access a device
	// GPU
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
	if(err == CL_DEVICE_NOT_FOUND) {
		// CPU
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
	}
	if(err < 0) {
		perror("Couldn't access any devices");
		exit(1);
	}

	return dev;
}

cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {

	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	int err;

	/* Read program file and place content into buffer */
	program_handle = fopen(filename, "r");
	if(program_handle == NULL) {
		perror("Couldn't find the program file");
		exit(1);
	}
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

	/* Create program from file
	 
	 Creates a program from the source code in the add_numbers.cl file.
	 Specifically, the code reads the file's content into a char array
	 called program_buffer, and then calls clCreateProgramWithSource.
	 */
	program = clCreateProgramWithSource(ctx, 1,
										(const char**)&program_buffer, &program_size, &err);
	if(err < 0) {
		perror("Couldn't create the program");
		exit(1);
	}
	free(program_buffer);

	/* Build program
	 
	 The fourth parameter accepts options that configure the compilation.
	 These are similar to the flags used by gcc. For example, you can
	 define a macro with the option -DMACRO=VALUE and turn off optimization
	 with -cl-opt-disable.s
	 */
	err = clBuildProgram(program, 0, NULL, "-DOPENCL___", NULL, NULL);
	if(err < 0) {

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
							  0, NULL, &log_size);
		program_log = (char*) malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
							  log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}

	return program;
}


int			cl_worker(t_mlx *mlx)
{
	cl_mem				img_buffer;		//
	cl_mem				cam_buffer;
	cl_mem				screen_buffer;
	cl_mem				counter_buffer;	//
	cl_mem				light_buffer;	//
	cl_mem				obj_buffer;		//
	cl_mem				aux_buffer;		//
	cl_int				err;
	size_t				global_size[2];
	size_t				local_size[2];
	cl_kernel			kernel_rt;
	cl_kernel			kernel_pp;
	cl_kernel			kernel_gb1;
	cl_kernel			kernel_gb2;

	global_size[0] = WIDTH;
	global_size[1] = HEIGHT;
	local_size[0] = 8;		//	надо как-то нормально ставить
	local_size[1] = 8;		//	вроде бы можно посчитать количество work групп. и просто WIDTH / work_group_count


	aux_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_READ_WRITE, WIDTH * HEIGHT * sizeof(int), NULL, &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	img_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_WRITE_ONLY, WIDTH * HEIGHT * sizeof(int), NULL, &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	cam_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_WRITE_ONLY |
												 CL_MEM_COPY_HOST_PTR, sizeof(t_cam), &(mlx->cam), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	screen_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_READ_ONLY |
													CL_MEM_COPY_HOST_PTR, sizeof(t_screen), &(mlx->screen), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	counter_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_READ_ONLY |
													 CL_MEM_COPY_HOST_PTR, sizeof(t_counter), &(mlx->counter), &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	light_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_READ_ONLY |
												   CL_MEM_COPY_HOST_PTR, mlx->counter.l * sizeof(t_light), mlx->light, &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};
	obj_buffer = clCreateBuffer(mlx->cl.context, CL_MEM_READ_ONLY |
												 CL_MEM_COPY_HOST_PTR, mlx->counter.all_obj * sizeof(t_object), mlx->obj, &err);
	if(err < 0) {
		perror("Couldn't create a buffer");
		exit(1);
	};

	kernel_rt = clCreateKernel(mlx->cl.program, KERNEL_FUNC_RT, &err);
	if(err < 0) {
		perror("Couldn't create a kernel");
		exit(1);
	}

	cl_int2 rands;
	rands.x = rand();
	rands.y = rand();
	err = clSetKernelArg(kernel_rt, 0, sizeof(cl_mem), &aux_buffer);
	err |= clSetKernelArg(kernel_rt, 1, sizeof(cl_mem), &cam_buffer);
	err |= clSetKernelArg(kernel_rt, 2, sizeof(cl_mem), &screen_buffer);
	err |= clSetKernelArg(kernel_rt, 3, sizeof(cl_mem), &counter_buffer);
	err |= clSetKernelArg(kernel_rt, 4, sizeof(cl_mem), &light_buffer);
	err |= clSetKernelArg(kernel_rt, 5, sizeof(cl_mem), &obj_buffer);
	err |= clSetKernelArg(kernel_rt, 6, sizeof(cl_int2), &rands);
	err |= clSetKernelArg(kernel_rt, 7, sizeof(cl_mem), &mlx->gpu_mem->cl_texture);
	err |= clSetKernelArg(kernel_rt, 8, sizeof(cl_mem), &mlx->gpu_mem->cl_texture_w);
	err |= clSetKernelArg(kernel_rt, 9, sizeof(cl_mem), &mlx->gpu_mem->cl_texture_h);
	err |= clSetKernelArg(kernel_rt, 10, sizeof(cl_mem), &mlx->gpu_mem->cl_prev_texture_size);
	if(err < 0) {
		perror("Couldn't create a kernel argument");
		exit(1);
	}
	err = clEnqueueNDRangeKernel(mlx->cl.queue, kernel_rt, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't enqueue the kernel");
		exit(1);
	}


	kernel_pp = clCreateKernel(mlx->cl.program, KERNEL_FUNC_PP, &err);
	if(err < 0) {
		perror("Couldn't create a kernel");
	}
	err |= clSetKernelArg(kernel_pp, 0, sizeof(cl_mem), &aux_buffer);
	err |= clSetKernelArg(kernel_pp, 1, sizeof(cl_mem), &img_buffer);
	err |= clSetKernelArg(kernel_pp, 2, sizeof(cl_mem), &screen_buffer);
	if(err < 0) {
		perror("Couldn't create a kernel argument");
		exit(1);
	}
	err = clEnqueueNDRangeKernel(mlx->cl.queue, kernel_pp, 2, NULL, global_size,
								 local_size, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't enqueue the kernel");
		exit(1);
	}


	if (mlx->screen.effects.z == 1)
	{
		kernel_gb1 = clCreateKernel(mlx->cl.program, KERNEL_FUNC_GB_X, &err);
		if(err < 0) {
			perror("Couldn't create a kernel");
		}
		err |= clSetKernelArg(kernel_gb1, 0, sizeof(cl_mem), &img_buffer);
		err |= clSetKernelArg(kernel_gb1, 1, sizeof(cl_mem), &aux_buffer);
		err |= clSetKernelArg(kernel_gb1, 2, sizeof(cl_mem), &screen_buffer);
		if(err < 0) {
			perror("Couldn't create a kernel argument");
			exit(1);
		}
		err = clEnqueueNDRangeKernel(mlx->cl.queue, kernel_gb1, 2, NULL, global_size,
									 local_size, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't enqueue the kernel");
			exit(1);
		}

		kernel_gb2 = clCreateKernel(mlx->cl.program, KERNEL_FUNC_GB_Y, &err);
		if(err < 0) {
			perror("Couldn't create a kernel");
		}
		err |= clSetKernelArg(kernel_gb2, 0, sizeof(cl_mem), &aux_buffer);
		err |= clSetKernelArg(kernel_gb2, 1, sizeof(cl_mem), &img_buffer);
		err |= clSetKernelArg(kernel_gb2, 2, sizeof(cl_mem), &screen_buffer);
		if(err < 0) {
			perror("Couldn't create a kernel argument");
			exit(1);
		}
		err = clEnqueueNDRangeKernel(mlx->cl.queue, kernel_gb2, 2, NULL, global_size,
									 local_size, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't enqueue the kernel");
			exit(1);
		}
		err = clEnqueueReadBuffer(mlx->cl.queue, img_buffer, CL_TRUE, 0,
								  WIDTH * HEIGHT * sizeof(int), mlx->img.data, 0, NULL, NULL);
		if(err < 0) {
			perror("Couldn't read the buffer");
			exit(1);
		}
	}

	err = clEnqueueReadBuffer(mlx->cl.queue, img_buffer, CL_TRUE, 0,
							  WIDTH * HEIGHT * sizeof(int), mlx->img.data, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(1);
	}

	clReleaseKernel(kernel_rt);
	clReleaseKernel(kernel_pp);
	if (mlx->screen.effects.z == 1)
	{
		clReleaseKernel(kernel_gb1);
		clReleaseKernel(kernel_gb2);
	}
	clReleaseMemObject(img_buffer);
	clReleaseMemObject(aux_buffer);
	clReleaseMemObject(obj_buffer);
	clReleaseMemObject(light_buffer);
	clReleaseMemObject(cam_buffer);
	clReleaseMemObject(screen_buffer);
	clReleaseMemObject(counter_buffer);

	return (err);
}


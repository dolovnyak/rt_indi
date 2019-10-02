#include "rt.h"

static void	texture_mem_gpu_initialization(t_rt *rt)
{
	int	err;

	err = 0;
	rt->gpu_mem->cl_texture = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)
		* rt->texture->texture_size, rt->texture->texture, NULL);
	cl_error_handler("Couldn't create texture buffer", err);
	rt->gpu_mem->cl_texture_w = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)
		* 100, rt->texture->texture_w, NULL);
	cl_error_handler("Couldn't create texture_w buffer", err);
	rt->gpu_mem->cl_texture_h = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)
		* 100, rt->texture->texture_h, NULL);
	cl_error_handler("Couldn't create texture_h buffer", err);
	rt->gpu_mem->cl_prev_texture_size = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int)
		* 100, rt->texture->prev_texture_size, NULL);
	cl_error_handler("Couldn't create prev_texture_size buffer", err);
}

static void	fiil_textures_mem(t_rt *rt)
{
	find_textures_size(rt, rt->texture->textures_path,
			rt->texture->textures_count);
	if (!(rt->texture->texture = (int *)ft_memalloc(sizeof(int)
			* rt->texture->texture_size)))
		return ;
	get_textures(rt, rt->texture->textures_path, rt->texture->textures_count);
	if (!(rt->texture->texture = (int *)ft_memalloc(sizeof(int)
			* rt->texture->texture_size)))
		cl_exit_error("Couldn't allocate memory");
	get_textures(rt, rt->texture->textures_path, rt->texture->textures_count);
	texture_mem_gpu_initialization(rt);
}

void		fill_gpu_mem(t_rt *rt)
{
	int	err;

	rt->gpu_mem = (t_gpu_mem *)ft_memalloc(sizeof(t_gpu_mem));
	fiil_textures_mem(rt);
	err = 0;
	rt->gpu_mem->cl_img_buffer = clCreateBuffer(*rt->cl->context,
		CL_MEM_WRITE_ONLY, WIDTH * HEIGHT * sizeof(int), NULL, &err);
	cl_error_handler("Couldn't create img buffer", err);
	rt->gpu_mem->cl_aux_buffer = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_WRITE, WIDTH * HEIGHT * sizeof(int), NULL, &err);
	cl_error_handler("Couldn't create aux buffer", err);
	rt->gpu_mem->cl_light_buffer = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		rt->counter.l * sizeof(t_light), rt->light, &err);
	cl_error_handler("Couldn't create light buffer", err);
	rt->gpu_mem->cl_obj_buffer = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		rt->counter.all_obj * sizeof(t_object), rt->obj, &err);
	cl_error_handler("Couldn't create obj buffer", err);
	rt->gpu_mem->cl_counter_buffer = clCreateBuffer(*rt->cl->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_counter),
		&(rt->counter), &err);
	cl_error_handler("Couldn't create counter buffer", err);
}

int			gauss_blur_gpu_init(t_rt *rt, cl_kernel *kernel,
											cl_mem screen_buffer)
{
	cl_int		err;

	err = 0;
	err |= clSetKernelArg(*kernel, 0, sizeof(cl_mem),
			&rt->gpu_mem->cl_aux_buffer);
	err |= clSetKernelArg(*kernel, 1, sizeof(cl_mem),
			&rt->gpu_mem->cl_img_buffer);
	err |= clSetKernelArg(*kernel, 2, sizeof(cl_mem), &screen_buffer);
	return (err);
}

#include "rt.h"

void		fill_gpu_mem(t_mlx *mlx)
{
	int					err;

	err = 0;
	mlx->cl.queue = clCreateCommandQueue(mlx->cl.context, mlx->cl.device, 0, &err);
	if(err < 0) {
		perror("Couldn't create a command queue");
		exit(1);
	};
	mlx->gpu_mem =(t_gpu_mem *)ft_memalloc(sizeof(t_gpu_mem));
	find_textures_size(mlx, mlx->texture->textures_path, mlx->texture->textures_count);
	if (!(mlx->texture->texture = (int *)ft_memalloc(sizeof(int) * mlx->texture->texture_size)))
		return ;
	get_textures(mlx, mlx->texture->textures_path, mlx->texture->textures_count);
	if (!(mlx->texture->texture = (int *)ft_memalloc(sizeof(int) * mlx->texture->texture_size)))
	{
		perror("Couldn't allocate memory");
		exit (1);
	}
	get_textures(mlx, mlx->texture->textures_path, mlx->texture->textures_count);
	mlx->gpu_mem->cl_texture = clCreateBuffer(mlx->cl.context,
											  CL_MEM_READ_ONLY, 4 * mlx->texture->texture_size,
											  NULL, NULL);
	clEnqueueWriteBuffer(mlx->cl.queue, mlx->gpu_mem->cl_texture, CL_TRUE, 0,
						 4 * mlx->texture->texture_size,
						 mlx->texture->texture, 0, NULL, NULL);
	mlx->gpu_mem->cl_texture_w = clCreateBuffer(mlx->cl.context,
												CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(mlx->cl.queue, mlx->gpu_mem->cl_texture_w, CL_TRUE, 0,
						 sizeof(int) * 100, &mlx->texture->texture_w, 0, NULL, NULL);
	mlx->gpu_mem->cl_texture_h = clCreateBuffer(mlx->cl.context,
												CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(mlx->cl.queue, mlx->gpu_mem->cl_texture_h, CL_TRUE, 0,
						 sizeof(int) * 100, &mlx->texture->texture_h, 0, NULL, NULL);
	mlx->gpu_mem->cl_prev_texture_size = clCreateBuffer(mlx->cl.context,
														CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(mlx->cl.queue, mlx->gpu_mem->cl_prev_texture_size, CL_TRUE, 0,
						 sizeof(int) * 100, &mlx->texture->prev_texture_size, 0, NULL, NULL);
}

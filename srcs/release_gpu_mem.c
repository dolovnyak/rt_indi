
#include "rt.h"

void 	release_mem_gpu_obj(t_rt *rt)
{
	int err;

	err = 0;
	err |= clReleaseMemObject(rt->gpu_mem->cl_texture);
	err |= clReleaseMemObject(rt->gpu_mem->cl_texture_w);
	err |= clReleaseMemObject(rt->gpu_mem->cl_texture_h);
	err |= clReleaseMemObject(rt->gpu_mem->cl_prev_texture_size);
	err |= clReleaseMemObject(rt->gpu_mem->cl_img_buffer);
	err |= clReleaseMemObject(rt->gpu_mem->cl_aux_buffer);
	err |= clReleaseMemObject(rt->gpu_mem->cl_light_buffer);
	err |= clReleaseMemObject(rt->gpu_mem->cl_obj_buffer);
	err |= clReleaseMemObject(rt->gpu_mem->cl_counter_buffer);
}

void	release_gpu_mem(t_rt *rt)
{
	t_list	*tmp_l;
	int		err;

	err = 0;
	clFinish(*rt->cl->queue);
	release_mem_gpu_obj(rt);
	tmp_l = rt->cl->programs;
	while (tmp_l)
	{
		err |= clReleaseProgram(*(cl_program *)tmp_l->content);
		tmp_l = tmp_l->next;
	}
	err |= clReleaseContext(*rt->cl->context);
	err |= clReleaseCommandQueue(*rt->cl->queue);
	err |= clReleaseDevice(rt->cl->device_id);
	err |= clReleaseKernel(*cl_get_kernel_by_name(rt->cl, "post_processing"));
	err |= clReleaseKernel(*cl_get_kernel_by_name(rt->cl, "phong_render"));
	err |= clReleaseKernel(*cl_get_kernel_by_name(rt->cl, "path_trace_render"));
	err |= clReleaseKernel(*cl_get_kernel_by_name(rt->cl, "gauss_blur_x"));
	err |= clReleaseKernel(*cl_get_kernel_by_name(rt->cl, "gauss_blur_y"));
}
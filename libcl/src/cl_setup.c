/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_cl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbosmer <sbosmer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 22:06:08 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/24 21:40:08 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcl.h"

static cl_device_id	cl_get_device_id(void)
{
	cl_platform_id	platform;
	cl_device_id	device;
	cl_int			err;

	err = clGetPlatformIDs(1, &platform, NULL);
	if (err != 0)
		cl_exit_error("get platform");
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != 0)
		cl_exit_error("get device id");
	return (device);
}

t_cl				*cl_setup(char **files, char **kernels)
{
	cl_device_id	device_id;
	t_cl			*cl;

	cl = cl_init();
	device_id = cl_get_device_id();
	cl_context_init(cl, device_id);
	cl_queue_init(cl, device_id);
	cl->program = cl_create_program(*cl->context, files, device_id);
	cl_fill_kernels(cl, kernels);
	return (cl);
}

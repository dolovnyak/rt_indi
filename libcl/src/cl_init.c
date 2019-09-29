/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 21:39:47 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/24 21:39:47 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcl.h"

t_cl		*cl_init(void)
{
	t_cl *cl;

	cl = (t_cl *)ft_x_memalloc(sizeof(t_cl));
	return (cl);
}

void		cl_queue_init(t_cl *cl, cl_device_id device_id)
{
	cl_int err;

	cl->queue = (cl_command_queue *)ft_x_memalloc(sizeof(cl_command_queue));
	*cl->queue = clCreateCommandQueue(*cl->context, device_id, 0, &err);
	if (err != 0)
		cl_exit_error("create command queue");
}

void		cl_context_init(t_cl *cl, cl_device_id device_id)
{
	cl_int err;

	cl->context = (cl_context *)ft_x_memalloc(sizeof(cl_context));
	*cl->context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
	if (err != 0)
		cl_exit_error("create context");
}

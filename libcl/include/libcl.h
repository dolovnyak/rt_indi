/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libcl.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:39:33 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/24 21:40:46 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBCL_H
# define LIBCL_H

# define CL_SILENCE_DEPRECATION
# include <OpenCL/opencl.h>
# include <stdio.h>
# include "libft.h"

typedef struct			s_cl
{
	cl_command_queue	*queue;
	cl_context			*context;
	cl_program			*program;
	t_list				*kernels;
}						t_cl;

t_cl					*cl_setup(char **files, char **kernels);
cl_kernel				*cl_get_kernel_by_name(t_cl *cl, char *name);

t_cl					*cl_init(void);
void					cl_queue_init(t_cl *cl, cl_device_id device_id);
void					cl_context_init(t_cl *cl, cl_device_id device_id);
cl_program				*cl_create_program(cl_context context, char **files,
		cl_device_id device_id);
char					*cl_get_file_buf(const char *name,
		size_t *program_size);
size_t					cl_get_files_num(char **files);
void					cl_fill_kernels(t_cl *cl, char **kernels);
void					cl_exit_error(char *output_error);
void					cl_error_handler(char *output_error, int err);

#endif

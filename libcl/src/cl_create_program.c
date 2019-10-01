/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_create_program.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 21:39:01 by sbecker           #+#    #+#             */
/*   Updated: 2019/10/01 12:16:30 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcl.h"

static void	cl_build_program(cl_device_id device, cl_program *program)
{
	cl_int	err;
	char	*log;
	size_t	log_size;

	err = clBuildProgram(*program, 1, &device, "-DOPENCL___ -I include/ ",
			NULL, NULL);
	if (err != 0)
	{
		clGetProgramBuildInfo(*program, device, CL_PROGRAM_BUILD_LOG,
				0, NULL, &log_size);
		log = (char*)malloc(log_size);
		clGetProgramBuildInfo(*program, device,
				CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("build program - ERROR (%d)\n", err);
		printf("%s\n", log);
		exit(-1);
	}
}

static void	get_files_buf(char **program_buf,
		size_t *program_size, char **files)
{
	size_t i;

	i = -1;
	while (files[++i])
		program_buf[i] = cl_get_file_buf(files[i], &program_size[i]);
}

static void	free_bufs(char **program_buf,
		size_t *program_size, size_t files_num)
{
	size_t	i;

	i = -1;
	while (++i < files_num)
		free(program_buf[i]);
	free(program_buf);
	free(program_size);
}

cl_program	*cl_create_program(cl_context context, char **files,
		cl_device_id device_id)
{
	cl_program	*program;
	char		**program_buf;
	size_t		*program_size;
	size_t		files_num;
	cl_int		err;

	program = (cl_program *)ft_x_memalloc(sizeof(cl_program));
	files_num = cl_get_files_num(files);
	program_buf = (char**)malloc(sizeof(char*) * files_num);
	program_size = (size_t*)malloc(sizeof(size_t) * files_num);
	get_files_buf(program_buf, program_size, files);
	*program = clCreateProgramWithSource(context, files_num,
			(const char**)program_buf, (const size_t*)program_size, &err);
	if (err != 0)
		cl_exit_error("create program");
	cl_build_program(device_id, program);
	free_bufs(program_buf, program_size, files_num);
	return (program);
}

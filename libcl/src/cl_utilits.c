/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilits_for_initialization_cl.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbosmer <sbosmer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 19:18:14 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/03 21:03:28 by sbosmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcl.h"

void	cl_exit_error(char *error)
{
	printf("ERROR: %s\n", error);
	exit(-1);
}

size_t	cl_get_files_num(char **files)
{
	size_t i;

	i = 0;
	while (files[i])
		i++;
	return (i);
}

char	*cl_get_file_buf(const char *name, size_t *program_size)
{
	size_t	file_size;
	FILE	*fd;
	char	*buf;

	fd = fopen(name, "r");
	if (!fd)
	{
		printf("ERROR: Open kernel file '%s'\n", name);
		exit(-1);
	}
	fseek(fd, 0, SEEK_END);
	file_size = ftell(fd);
	rewind(fd);
	buf = (char*)malloc(file_size + 1);
	fread(buf, sizeof(char), file_size, fd);
	buf[file_size] = '\0';
	fclose(fd);
	*program_size = file_size;
	return (buf);
}

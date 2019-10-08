/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_kernels.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 21:37:14 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/24 21:38:41 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcl.h"

static void	cl_add_kernel_by_name(t_cl *cl, char *name)
{
	cl_int		err;
	t_list		*lst;
	int			hash;
	cl_kernel	*kernel;
	cl_program	*program;

	program = (cl_program *)cl->programs->content;
	lst = NULL;
	kernel = (cl_kernel *)ft_x_memalloc(sizeof(cl_kernel));
	*kernel = clCreateKernel(*program, name, &err);
	if (err != 0 || !(lst = ft_lstnew(NULL, 0)))
		cl_exit_error("create kernel");
	hash = ft_strhash(name);
	lst->content = (void *)kernel;
	lst->content_size = hash;
	ft_lstadd(&(cl->kernels), lst);
}

void		cl_fill_kernels(t_cl *cl, char **kernels)
{
	size_t	i;

	i = -1;
	cl->kernels = NULL;
	while (kernels[++i])
		cl_add_kernel_by_name(cl, kernels[i]);
}

cl_kernel	*cl_get_kernel_by_name(t_cl *cl, char *name)
{
	cl_kernel	*kernel;
	t_list		*tmp;
	int			hash;

	kernel = NULL;
	tmp = cl->kernels;
	hash = ft_strhash(name);
	while (tmp && !kernel)
	{
		if (tmp->content_size == (size_t)hash)
			kernel = (cl_kernel *)tmp->content;
		tmp = tmp->next;
	}
	return (kernel);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:59:04 by broggo            #+#    #+#             */
/*   Updated: 2019/01/31 16:48:37 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		f_line(char **thread, char **line, int fd)
{
	char	*str;
	int		dif;

	dif = ft_strchr(thread[fd], '\n') - thread[fd];
	*line = ft_strsub(thread[fd], 0, dif);
	str = ft_strdup(ft_strchr(thread[fd], '\n') + 1);
	free(thread[fd]);
	thread[fd] = str;
	if (!thread[fd][0])
		ft_strdel(&thread[fd]);
	return (1);
}

int		f_end(char **thread, char **line, int fd)
{
	if (!thread[fd][0])
		return (0);
	*line = ft_strdup(thread[fd]);
	ft_strdel(&thread[fd]);
	return (1);
}

int		get_next_line(const int fd, char **line)
{
	static char	*thread[0];
	char		buf[BUFF_SIZE + 1];
	char		*c;
	int			rd;

	if (line == NULL || read(fd, buf, 0) < 0)
		return (-1);
	while ((rd = read(fd, buf, BUFF_SIZE)) >= 0)
	{
		buf[rd] = 0;
		if (!thread[fd])
			thread[fd] = ft_strnew(0);
		c = ft_strjoin(thread[fd], buf);
		free(thread[fd]);
		thread[fd] = c;
		if (ft_strchr(thread[fd], '\n'))
			return (f_line(thread, line, fd));
		if (!rd)
			return (f_end(thread, line, fd));
	}
	if (thread[fd])
		ft_strdel(&thread[fd]);
	return (-1);
}

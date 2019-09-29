/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:59:11 by broggo            #+#    #+#             */
/*   Updated: 2019/01/29 15:59:11 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 6
# include "libft.h"

int		get_next_line(int const fd, char **line);
int		f_line(char **thread, char **line, int fd);
int		f_end(char **thread, char **line, int fd);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jtoc_get_raw_data_help.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 03:57:30 by sbecker           #+#    #+#             */
/*   Updated: 2019/08/19 04:02:37 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	jtoc_get_raw_data_help(t_jnode *cur, char *res, void **tmp, size_t *i)
{
	if ((cur->type == string && !(*tmp = get_string(cur))) ||
			(cur->type == array && !(*tmp = get_array(cur))) ||
			(cur->type == object && !(*tmp = jtoc_get_raw_data(cur))))
	{
		free(res);
		return (1);
	}
	else if (cur->type == none)
		*tmp = NULL;
	ft_memcpy(res + *i, tmp, 8);
	*i += 4;
	return (0);
}

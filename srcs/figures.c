/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   figures.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 16:12:17 by broggo            #+#    #+#             */
/*   Updated: 2019/08/05 16:12:18 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"


int			hex_to_int(char *s, int *err)
{
	int	numb;
	int	i;
	
	numb = 0;
	i = -1;
	while (i++ < 5)
	{
		if (*(s + i) >= '0' && *(s + i) <= '9')
		numb += pow(16, 5 - i) * (*(s + i) - '0');
		else if (*(s + i) >= 'A' && *(s + i) <= 'F')
		numb += pow(16, 5 - i) * (*(s + i) - 'A' + 10);
		else
		*err = 1;
	}
	return (numb);
}

float	get_float2(char *str, int *err)
{
	float	numb;
	int			i;
	
	i = 0;
	numb = 0;
	if (*str == '.')
	i++;
	while (*(str + i) != '\0')
	{
		if (!ft_isdigit(*(str + i)))
		{
			*err = 1;
			return (0);
		}
		numb = numb + (*(str + i) - '0') * pow(0.1, i);
		i++;
	}
	return (numb);
}

float	get_float(char *str, int *err)
{
	float	numb;
	int			i;
	int			m;
	
	i = 0;
	m = 1;
	numb = 0;
	if (*str == '-')
	{
		str++;
		m = -1;
	}
	else if (*str == '+')
	str++;
	while ((*(str + i) != '\0') & (*(str + i) != '.'))
	{
		if (!ft_isdigit(*(str + i)))
		{
			*err = 1;
			return (0);
		}
		numb = numb * 10 + *(str + i) - '0';
		i++;
	}
	return ((float)(m * (numb + get_float2(str + i, err))));
}

cl_float3	get_vector(char *s1, char *s2, char *s3, int *err)
{
	cl_float3	v;
	
	v.x = get_float(s1, err);
	v.y = get_float(s2, err);
	v.z = get_float(s3, err);
	return (v);
}

int			get_color_int(char *str, int *err)
{
	if (ft_strlen(str) != 8)
	{
		*err = 1;
		return (0);
	}
	if ((str[0] != '0') || (str[1] != 'x'))
	{
		*err = 1;
		return (0);
	}
	return (hex_to_int(str + 2, err));
}

cl_float3	v_color(int col)
{
	cl_float3	v;
	
	v.x = (float)((col >> 16) & 0xFF) / 255;
	v.y = (float)((col >> 8) & 0xFF) / 255;
	v.z = (float)((col) & 0xFF) / 255;
	return (v);
}

void		str_cleaner(char **s, int n)
{
	int	i;
	
	i = 0;
	while (i < n)
	{
		free(s[i]);
		i++;
	}
	free(s);
}

t_material	new_mat(char **arr, int first, int *err)
{
	t_material	m;

	m.diffuse_color = v_color(get_color_int(arr[first], err));
	m.al.x = get_float(arr[first + 1], err);
	m.al.y = get_float(arr[first + 2], err);
	m.sp_ex = get_float(arr[first + 3], err);
	if (!((m.al.x > 0) && (m.al.y >= 0) && (m.sp_ex >= 1)))
	*err = 1;
	return (m);
}

void		vect_is_not_null(cl_float3 v, int *err)
{
	if ((fabsf(v.x) < 1e-5f) && (fabsf(v.y) < 1e-5f) && (fabsf(v.z) < 1e-5f))
	*err = 1;
}

int		new_objects_f(int fd, t_rt *rt, t_counter *counter)
{
	char	*s;
	int		i;
	int err = 0;

	get_next_line(fd, &s);
	counter->all_obj = getnbr(s, &err);
	free(s);
	if (!(rt->obj = (t_object *)malloc(counter->all_obj * sizeof(t_object))))
		return (0);
	i = 0;
	while ((i < counter->all_obj) && (get_next_line(fd, &s) == 1))
	{
		char		**arr;
		size_t		j;
	
		arr = ft_strsplit(s, '\t', &j);
		if (j != 13)
		{
			str_cleaner(arr, j);
			ft_putstr("Error 2\n");
			free(rt->obj);
			return (0);
		}
		rt->obj[i].type = getnbr(arr[0], &err);
		rt->obj[i].center =  get_vector(arr[1], arr[2], arr[3], &err);
		rt->obj[i].vector =  get_vector(arr[4], arr[5], arr[6], &err);
		rt->obj[i].radius = get_float(arr[7], &err);
		rt->obj[i].param = get_float(arr[8], &err);
		rt->obj[i].mat = new_mat(arr, 9, &err);
		str_cleaner(arr, 13);
        
        cl_to_norm(&rt->obj[i].vector);
        vect_is_not_null(rt->obj[i].vector, &err);
		if (rt->obj[i].radius < 0)
            err = 1;
        
		free(s);
		if (err == 1)
		{
			free(rt->obj);
			return (0);
		}
		i++;
	}
	if (i != counter->all_obj)
		return (0);
	return (1);
}

int	new_lights_f(int fd, t_rt *rt, t_counter *counter)
{
	char	*s;
	int		i;
	int err = 0;

	get_next_line(fd, &s);
	counter->l = getnbr(s, &err);
	free(s);
	if (!(rt->light = (t_light *)malloc(counter->l * sizeof(t_light))))
		return (0);
	i = 0;
	while ((i < counter->l) && (get_next_line(fd, &s) == 1))
	{
		char		**arr;
		size_t		j;

		arr = ft_strsplit(s, '\t', &j);
		if (j != 4)
		{
			str_cleaner(arr, j);
			ft_putstr("Error 1\n");
			free(rt->light);
			return (0);
		}
		rt->light[i].center =  get_vector(arr[0], arr[1], arr[2], &err);
		rt->light[i].intens = get_float(arr[3], &err);
		str_cleaner(arr, 4);
		free(s);
		if (err == 1)
		{
			free(rt->light);
			return (0);
		}
		i++;
	}
	if (i != counter->l)
		return (0);
	return (1);
}

int	read_map(char *name, t_rt *rt)
{
	int		fd;

	if ((fd = open(name, O_RDONLY)) <= 0)
	{
		ft_putstr("Error name\n");
		return (0);
	}
	if (new_lights_f(fd, rt, &rt->counter))
	{
		if (new_objects_f(fd, rt, &rt->counter))
		{
			close(fd);
			return (1);
		}
	}
	ft_putstr("read map\n");
	close(fd);
	return (0);
}

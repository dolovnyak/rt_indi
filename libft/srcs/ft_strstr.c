/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: broggo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:20:26 by broggo            #+#    #+#             */
/*   Updated: 2018/09/24 15:14:56 by broggo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strstr(const char *haystack, const char *needle)
{
	int i;
	int k;

	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while (*(haystack + i))
	{
		if (*(haystack + i) == *needle)
		{
			k = 0;
			while (*(needle + k) && (*(needle + k) == *(haystack + i + k)))
				k++;
			if (*(needle + k) == '\0')
				return ((char *)haystack + i);
		}
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:20:27 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:20:29 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_data	*ft_new(char *bf)
{
	t_data	*list;

	if (!bf)
		return (NULL);
	list = malloc(sizeof(t_data));
	if (list)
	{
		list->str = bf;
		list->next = NULL;
		return (list);
	}
	return (NULL);
}

t_data	*ft_last(t_data *lst, int find_nl, int push_loop)
{
	t_data	*last;

	if (!lst)
		return (lst);
	last = lst;
	if (last == lst && last->str[ft_len(last->str)] == '\n' && find_nl)
		return (last);
	while (last->next)
	{
		if (find_nl && last->str && last->str[ft_len(last->str)] == '\n')
			return (last);
		last = last->next;
	}
	if (find_nl && !push_loop && !last->next)
		return (NULL);
	return (last);
}

char	*ft_join(char *s1, char *s2, size_t len)
{
	size_t					i;
	size_t					j;
	char					*result;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (s1 && s1[i] && i < len)
	{
		result[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] && i < len)
	{
		result[i] = s2[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

size_t	ft_len(char *bf)
{
	size_t		i;

	i = 0;
	while (bf[i] && bf[i] != '\n')
		i++;
	return (i);
}

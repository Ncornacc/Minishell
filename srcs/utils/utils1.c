/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils1.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:42:09 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/05 19:21:34 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return ;
	while (i < n)
	{
		*(((char *)s) + i) = 0;
		i++;
	}
}

int		ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	if (c >= '0' && c <= '9')
		return (2);
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t i;

	i = 0;
	if (!dst && !src)
		return (NULL);
	while (i < n)
	{
		*((char *)(dst + i)) = *((const char *)(src + i));
		i++;
	}
	return (dst);
}

void	indexing(char **line, char **valid_escape_char, int *index)
{
	*index = 0;
	while (valid_escape_char[*index])
	{
		if (match_str(*line, valid_escape_char[*index], 0))
			break ;
		(*index)++;
	}
}

void	iter_whitespaces(char **line, int *index)
{
	while (index && (*line)[*index] &&
			((*line)[*index] == ' ' || (*line)[*index] == '\t'))
		(*index)++;
	while (!index && line && *line && **line &&
			(**line == ' ' || **line == '\t'))
		(*line)++;
}

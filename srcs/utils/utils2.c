/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:16:36 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:16:39 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s || fd == -1)
		return ;
	write(1, s, str_len(s, NULL, 0));
}

char	*ft_strchr(const char *s, int c)
{
	if (!(*s) && !c)
		return ((char *)s);
	if (*s && (int)(*s) == c)
		return ((char *)s);
	if (!(*s) && c)
		return (NULL);
	return (ft_strchr(s + 1, c));
}

int		ft_strcmp(void *s1, void *s2)
{
	int		i;
	char	*str1;
	char	*str2;

	i = 0;
	str1 = s1;
	str2 = s2;
	while ((str1[i] && str2[i]) && str1[i] == str2[i])
		i++;
	if (!str1[i] && !str2[i])
		return (0);
	return (str1[i] - str2[i]);
}

int		match_str(char *s1, char *s2, char full)
{
	if (!s1 || !s2)
		return (false);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (!full && !*s2)
		return (true);
	if (!*s1 && !*s2)
		return (true);
	return (false);
}

size_t	str_len(char *str, char *stop, char add)
{
	size_t len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
	{
		if (stop && ft_strchr(stop, str[len]))
			return (add ? len + 1 : len);
		len++;
	}
	return (len);
}

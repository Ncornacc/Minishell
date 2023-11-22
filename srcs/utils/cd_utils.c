/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:11:41 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:11:44 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		valid_sub(char *current_path)
{
	return (!match_str(current_path, "/..", 0));
}

size_t	get_new_len(char *new_path, size_t new_path_len)
{
	size_t	index;
	size_t	len;

	index = 0;
	len = 0;
	while (index < new_path_len)
	{
		if ((new_path)[index])
			len++;
		index++;
	}
	return (len);
}

int		is_relative_dot(char *dir)
{
	size_t index;
	size_t dir_len;

	index = 0;
	dir_len = str_len(dir, NULL, 0);
	while (index < dir_len)
	{
		if (match_str(dir + index, ".", 1)
				|| match_str(dir + index, "..", 1))
			return (true);
		if (match_str(dir + index, "./", 0))
			index += 2;
		else if (match_str(dir + index, "../", 0))
			index += 3;
		else
			return (false);
	}
	return (true);
}

int		is_dot_dir(char *dir)
{
	return (match_str(dir, "./", 0)
			|| match_str(dir, "../", 0)
			|| match_str(dir, ".", 1)
			|| match_str(dir, "..", 1));
}

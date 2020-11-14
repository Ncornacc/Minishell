/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_path_utils2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 04:35:32 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/09 05:21:08 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_update_current_path(char *new_path, size_t *current_path)
{
	if (!match_str(new_path + *current_path, "../", 0)
			&& !match_str(new_path + *current_path, "..", 1))
		return ;
	if (*current_path == 1)
	{
		new_path[*current_path] = '\0';
		new_path[*current_path + 1] = '\0';
		return ;
	}
	(*current_path) -= 2;
	while (new_path[*current_path] != '/')
		(*current_path)--;
}

void	update_index(char *new_path, size_t new_path_len,
		size_t current_path, size_t *i)
{
	size_t index;

	index = current_path;
	while (index < new_path_len)
	{
		if (new_path[index] == '/')
			break ;
		index++;
	}
	*i = index;
}

void	delete_shift_slash(char *new_path, size_t index, size_t new_path_len)
{
	size_t i;

	i = index + 1;
	while (i < new_path_len && new_path[i] == '/')
	{
		new_path[i] = '\0';
		i++;
	}
	if (i == index + 1)
		return ;
	index++;
	while (i < new_path_len)
	{
		new_path[index] = new_path[i];
		new_path[i] = '\0';
		i++;
		index++;
	}
}

void	remove_dot_component(char *new_path, size_t new_path_len)
{
	size_t	i;

	i = 0;
	while (i < new_path_len)
	{
		if (match_str(new_path + i, "/./", 0)
				|| match_str(new_path + i, "/.", 1))
		{
			(new_path)[i] = '\0';
			(new_path)[i + 1] = '\0';
			shift_path_dot(new_path, new_path_len, i);
		}
		else
			i++;
	}
	join_path(new_path, new_path_len);
	if (!new_path[0])
		new_path[0] = '/';
}

void	remove_dot_dot_component(char *new_path, size_t new_path_len)
{
	size_t	i;
	size_t	current_path;

	i = 0;
	current_path = 1;
	while (i < new_path_len)
	{
		while (match_str(new_path + i, "/..", 0))
		{
			delete_current_path(new_path, new_path_len, current_path, i);
			shift_path(new_path, new_path_len, current_path, i);
			cd_update_current_path(new_path, &current_path);
			update_index(new_path, new_path_len, current_path, &i);
		}
		if (new_path[i] == '/')
			current_path = i + 1;
		i++;
	}
}

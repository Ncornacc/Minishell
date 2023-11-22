/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_path_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:11:22 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:11:24 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_current_path(char *new_path, size_t new_path_len,
		size_t current_path, size_t i)
{
	size_t	to_delete;
	size_t	index;

	to_delete = i + 3;
	index = current_path;
	while (index < new_path_len && index < to_delete)
	{
		new_path[index] = '\0';
		index++;
	}
}

void	shift_path(char *new_path, size_t new_path_len,
		size_t current_path, size_t i)
{
	size_t	to_delete;
	size_t	index;

	index = current_path;
	to_delete = i + 4;
	while (index < new_path_len && to_delete < new_path_len)
	{
		if (!new_path[to_delete] && to_delete + 1 < new_path_len)
			to_delete++;
		new_path[index] = new_path[to_delete];
		new_path[to_delete] = '\0';
		index++;
		to_delete++;
	}
}

void	shift_path_dot(char *new_path, size_t new_path_len, size_t index)
{
	size_t	i;

	i = index + 2;
	while (i < new_path_len)
	{
		new_path[index] = new_path[i];
		new_path[i] = '\0';
		index++;
		i++;
	}
}

void	join_path(char *new_path, size_t new_path_len)
{
	size_t	index;

	index = 0;
	while (index < new_path_len)
	{
		if (!new_path[index])
			shift_path_dot(new_path, new_path_len, index);
		index++;
	}
}

void	slash_sub(char *new_path, size_t new_path_len)
{
	size_t	index;

	index = 0;
	while (index < new_path_len)
	{
		if (new_path[index] == '/')
			delete_shift_slash(new_path, index, new_path_len);
		index++;
	}
}

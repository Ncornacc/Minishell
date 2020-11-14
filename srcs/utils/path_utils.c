/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 12:06:09 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/09 12:06:07 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_path(char *cmd_name)
{
	int	i;

	i = 0;
	while (cmd_name && cmd_name[i])
	{
		if (cmd_name[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

char	*check_exec_copy_path(t_shell **shell)
{
	char	*path_n;
	char	*full_path;

	path_n = fetch_env((*shell)->path_env_name, shell, NULL);
	full_path = ft_join(path_n, NULL, str_len(path_n, NULL, 0));
	if (!full_path)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	return (full_path);
}

void	check_exec_free(t_shell **shell, char **full_path, char **exec_name)
{
	if (full_path && *full_path)
		free(*full_path);
	if (exec_name && *exec_name)
		free(*exec_name);
	if (shell)
	{
		free((*shell)->absolute_path);
		(*shell)->absolute_path = NULL;
	}
}

char	*check_exec_get_exec_name(t_shell **shell, char **full_path)
{
	char	*exec_name;

	exec_name = ft_join("/", (*((*shell)->current_command))->name,
			str_len(((*((*shell)->current_command)))->name, NULL, 0) + 1);
	if (!exec_name)
	{
		free(*full_path);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	return (exec_name);
}

char	update_current_path(t_shell **shell, char **full_path,
		char **exec_name, int *i)
{
	int		current_path;
	char	next_path;

	current_path = *i;
	*i += str_len(*full_path + *i, ":", 0);
	next_path = (*full_path)[*i];
	(*full_path)[*i] = '\0';
	(*shell)->absolute_path = ft_join(*full_path + current_path,
			*exec_name, *i + str_len(*exec_name, NULL, 0));
	if (!(*shell)->absolute_path)
	{
		check_exec_free(NULL, full_path, exec_name);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	return (next_path);
}

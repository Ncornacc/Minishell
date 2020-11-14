/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 20:02:17 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 18:23:16 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	checks if command is a builtin and if so, if it is piped
*/

bool	execute_builtin(t_shell **shell, int *tmp_fd)
{
	int		index;
	char	**commands_name;
	char	*arg;

	commands_name = (*shell)->built_in_str;
	arg = (*((*shell)->current_command))->name;
	index = 0;
	while (commands_name[index])
	{
		iter_whitespaces(&arg, NULL);
		if (match_str(commands_name[index], arg, 1))
		{
			(*((*shell)->current_command))->exec |= PROCESS_BUILT_IN;
			if ((*((*shell)->current_command))->exec & PROCESS_PIPED)
				exec_call_builtin(shell, index, tmp_fd);
			else
				(*shell)->built_in_fun[index](shell);
			return (true);
		}
		index++;
	}
	return (false);
}

/*
**	if builtin is called in a pipe, forks process and executes
*/

void	exec_call_builtin(t_shell **shell, int index, int *tmp_fd)
{
	pid_t	process;

	process = fork();
	(*((*shell)->current_command))->pid = process;
	(*((*shell)->current_command))->exec |= PROCESS_FORKED;
	if (process == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if (process == 0)
	{
		(*shell)->process_level = 0;
		if (close(tmp_fd[IN]) == -1)
			error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
		(*shell)->built_in_fun[index](shell);
		update_status(shell);
		built_in_exit(shell);
	}
}

/*
**	forks and executes command
*/

void	exec_call(t_shell **shell, int *tmp_fd)
{
	pid_t	process;
	char	*absolute_path;

	init_exec_args(shell);
	absolute_path = (*shell)->absolute_path ? (*shell)->absolute_path :
					(*((*shell)->current_command))->name;
	process = fork();
	(*((*shell)->current_command))->exec |= PROCESS_FORKED;
	(*((*shell)->current_command))->pid = process;
	if (process == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if (process == 0)
	{
		(*shell)->process_level = 0;
		if (close(tmp_fd[IN]) == -1)
			error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
		if (execve(absolute_path, (*((*shell)->current_command))->exec_args,
				(*shell)->envp_cpy) == -1)
		{
			error_handler(shell, NULL, EXEC_FAILURE, strerror(errno));
			(*shell)->status = EXEC_FAILURE;
			built_in_exit(shell);
		}
	}
}

void	init_exec_args(t_shell **shell)
{
	t_list	*current_arg;
	int		n_args;

	if (!*((*shell)->current_command))
		return ;
	n_args = (*((*shell)->current_command))->n_args;
	(*((*shell)->current_command))->exec_args = malloc(
		(n_args + 2) * sizeof(char *));
	if (!(*((*shell)->current_command))->exec_args)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	current_arg = (*((*shell)->current_command))->args;
	n_args = 0;
	while (current_arg)
	{
		(*((*shell)->current_command))->exec_args[n_args] = current_arg->name;
		current_arg = current_arg->next;
		n_args++;
	}
	(*((*shell)->current_command))->exec_args[n_args] = NULL;
}

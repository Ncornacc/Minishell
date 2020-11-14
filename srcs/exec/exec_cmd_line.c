/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_cmd_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/02 21:13:50 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 20:33:01 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	loops through commands, expanding variables,
**	processing redirects and pipes, and executing commands.
*/

void	exec_cmd_line(t_shell **shell)
{
	int		saved_std[2];
	int		tmp_fd[2];
	t_cmd	**current;
	t_cmd	*wait_start;

	exec_signals(shell);
	current = &((*shell)->commands);
	wait_start = (*shell)->commands;
	(*shell)->current_command = current;
	save_std(shell, saved_std);
	tmp_fd[IN] = saved_std[IN];
	while (*current)
	{
		if (variable_expansion(shell) == SUCCESS &&
				process_in(shell, tmp_fd, saved_std) == SUCCESS &&
				process_out(shell, tmp_fd, saved_std) == SUCCESS)
			exec_process(shell, tmp_fd);
		if (handle_semicolon(shell, current, &wait_start) == ERROR)
			break ;
		current = &((*current)->next);
		(*shell)->current_command = current;
	}
	reset_std(shell, saved_std);
	if (wait_start)
		wait_and_signal_check(shell, wait_start);
}

/*
**	saves standard in/output in holder fds
*/

void	save_std(t_shell **shell, int *saved_std)
{
	saved_std[IN] = dup(STDIN_FILENO);
	saved_std[OUT] = dup(STDOUT_FILENO);
	if (saved_std[IN] == -1 || saved_std[OUT] == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	(*shell)->current_status = SUCCESS;
}

/*
**	executes process either as builtin or binary, retrieves path
*/

void	exec_process(t_shell **shell, int *tmp_fd)
{
	if (!(*((*shell)->current_command))->name)
		return ;
	if (execute_builtin(shell, tmp_fd))
		return ;
	if (match_str((*((*shell)->current_command))->name, "..", 1)
			|| match_str((*((*shell)->current_command))->name, ".", 1))
		error_handler(shell, NULL, CMD_NOT_FOUND, ERR_CMD_NOT_FOUND);
	else if (is_path((*((*shell)->current_command))->name))
		check_exec_absolute_path(shell);
	else
		check_exec_relative_path(shell);
	if ((*shell)->current_status == SUCCESS)
		exec_call(shell, tmp_fd);
}

/*
**	resets standard in/output fds to original values
*/

void	reset_std(t_shell **shell, int *saved_std)
{
	if (dup2(saved_std[IN], STDIN_FILENO) == -1
			|| dup2(saved_std[OUT], STDOUT_FILENO) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if (close(saved_std[IN]) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if (close(saved_std[OUT]) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
}

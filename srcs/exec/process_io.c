/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_io.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:21:05 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:21:07 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	if process input does not point to read end of a pipe,
**	we duplicate STDIN. Function then redirects input to input file,
**	closing any previously opened files if multiple inputs.
**	process input is reset at end in case pipes are encountered in process_out
*/

int	process_in(t_shell **shell, int *tmp_fd, int *saved_std)
{
	t_list	**current;

	current = &((*((*shell)->current_command))->in_files);
	if (tmp_fd[IN] == saved_std[IN])
	{
		tmp_fd[IN] = dup(saved_std[IN]);
		if (tmp_fd[IN] == -1)
			error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	}
	while (*current)
	{
		if (close(tmp_fd[IN]) == -1)
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		if (open_file(shell, current, &tmp_fd[IN], 0) == ERROR)
		{
			tmp_fd[IN] = saved_std[IN];
			return (ERROR);
		}
		current = &((*current)->next);
	}
	dup2_and_close(shell, tmp_fd[IN], STDIN_FILENO);
	tmp_fd[IN] = saved_std[IN];
	return (SUCCESS);
}

/*
**	initialises process output to STDOUT, then redirects to output file if any.
**	if pipe is found, calls function to open pipe.
*/

int	process_out(t_shell **shell, int *tmp_fd, int *saved_std)
{
	t_list **current;

	tmp_fd[OUT] = dup(saved_std[OUT]);
	if (tmp_fd[OUT] == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	current = &((*((*shell)->current_command))->out_files);
	while (*current)
	{
		if (close(tmp_fd[OUT]) == -1)
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		if (open_file(shell, current, &tmp_fd[OUT], 1) == ERROR)
			return (ERROR);
		current = &((*current)->next);
	}
	if ((*((*shell)->current_command))->separator == PIPE)
		handle_pipe(shell, current, tmp_fd);
	dup2_and_close(shell, tmp_fd[OUT], STDOUT_FILENO);
	return (SUCCESS);
}

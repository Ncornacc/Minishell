/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_separator.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 19:58:51 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/06 22:41:21 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	calls pipe on shell->filedes.
**	if output is not assigned, assigns process output to write end of pipe.
**	else closes unused write end of pipe.
**	input for the next process after the pipe is assigned to read end of pipe.
**	if output is not assigned, separator is changed to a semicolon,
**	making next process wait and preventing simultaneous execution that would
**	throw an error in case of input like "ls > file | cat < file"
*/

void	handle_pipe(t_shell **shell, t_list **current, int *tmp_fd)
{
	if (pipe((*shell)->filedes) == -1)
		error_handler(shell, NULL, INTERNAL_ERROR, strerror(errno));
	if ((*((*shell)->current_command))->out == NOT_ASSIGNED)
	{
		if (close(tmp_fd[OUT]) == -1)
			error_handler(shell, (*current)->name, ERROR, strerror(errno));
		tmp_fd[OUT] = (*shell)->filedes[OUT];
	}
	else
	{
		if (close((*shell)->filedes[OUT]) == -1)
			error_handler(shell, (*current)->name, ERROR, strerror(errno));
	}
	tmp_fd[IN] = (*shell)->filedes[IN];
	if ((*((*shell)->current_command))->out != NOT_ASSIGNED)
		(*((*shell)->current_command))->separator = SEMICOLON;
	(*((*shell)->current_command))->exec |= PROCESS_PIPED;
	if ((*((*shell)->current_command))->next)
		(*((*shell)->current_command))->next->exec |= PROCESS_PIPED;
}

/*
**	if semicolon is found, waits for child processes to exit,
**	moves wait_start, and checks for errors before continuing exec.
*/

int		handle_semicolon(t_shell **shell, t_cmd **current,
							t_cmd **wait_start)
{
	if ((*current)->separator == SEMICOLON)
	{
		wait_and_signal_check(shell, *wait_start);
		*wait_start = (*current)->next;
		if ((*shell)->current_status == PARSING_ERROR
				|| (*shell)->current_status == BAD_SUBSTITUTION
				|| (*shell)->current_status == NO_MULTI_LINE)
			return (ERROR);
		update_status(shell);
	}
	return (SUCCESS);
}

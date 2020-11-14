/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/14 13:44:28 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 18:23:45 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lexer_error_handler(t_shell **shell, char *args, int error_code,
								char *error_message)
{
	if (shell && *shell)
		(*shell)->current_status = error_code;
	error_handler(shell, args, error_code, error_message);
}

void	error_handler(t_shell **shell, char *args, int error_code,
						char *error_message)
{
	char	*command;

	command = NULL;
	if (shell)
		(*shell)->current_status = error_code;
	if (error_code == INTERNAL_ERROR)
	{
		(*shell)->status = ERROR;
		error_printer(NULL, NULL, error_message);
		built_in_exit(shell);
	}
	if (error_code == PARSING_ERROR || error_code == NO_MULTI_LINE ||
			error_code == SHELL_ERROR || error_code == BAD_SUBSTITUTION)
		command = "minishell";
	else if (shell && *((*shell)->current_command))
		command = (*(*shell)->current_command)->name;
	error_printer(command, args, error_message);
}

void	error_printer(char *command, char *args, char *error_message)
{
	if (command)
	{
		write(STDERR_FILENO, command, str_len(command, NULL, 0));
		write(STDERR_FILENO, ": ", 2);
	}
	if (args)
	{
		write(STDERR_FILENO, args, str_len(args, NULL, 0));
		write(STDERR_FILENO, ": ", 2);
	}
	if (error_message)
	{
		write(STDERR_FILENO, error_message, str_len(error_message, NULL, 0));
		write(STDERR_FILENO, "\n", 1);
	}
}

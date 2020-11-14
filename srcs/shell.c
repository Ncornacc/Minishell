/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 18:28:26 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/10 13:34:26 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	run_shell(t_shell **shell)
{
	char	*input;

	input = NULL;
	while ((*shell)->current_status != INTERNAL_ERROR)
	{
		reset_shell(shell);
		shell_signals(shell);
		if (g_signal)
			g_signal = 0;
		prompt();
		read_line(shell, &input);
		if (g_signal)
		{
			(*shell)->status = g_signal;
			g_signal = 0;
		}
		lexer(shell, &input);
		parser(shell);
		if ((*shell)->current_status == SUCCESS)
			exec_cmd_line(shell);
		update_status(shell);
	}
}

void	prompt(void)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("$>", STDOUT_FILENO);
}

void	update_status(t_shell **shell)
{
	if ((*shell)->current_status == SHELL_ERROR ||
			(*shell)->current_status == BAD_SUBSTITUTION ||
			(*shell)->current_status == NO_OPT ||
			(*shell)->current_status == NO_ARG ||
			(*shell)->current_status == AMBIGUOUS_REDIRECT)
		(*shell)->status = ERROR;
	else if ((*shell)->current_status == NO_MULTI_LINE)
		(*shell)->status = 2;
	else
		(*shell)->status = (*shell)->current_status;
}

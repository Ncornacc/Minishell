/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:10:37 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:10:43 by ncornacc      ########   odam.nl         */
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
	ft_putstr_fd("minishell$> ", STDOUT_FILENO);
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

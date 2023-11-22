/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:10:50 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:10:53 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	shell = new_shell(envp);
	if (!shell)
		built_in_exit(NULL);
	run_shell(&shell);
	(void)ac;
	(void)av;
	return (0);
}

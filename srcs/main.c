/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 18:33:37 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/09 20:29:03 by mjiam         ########   odam.nl         */
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

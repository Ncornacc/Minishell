/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:22:05 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:22:06 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Flags error if option is found. Otherwise calls getcwd and outputs path
*/

void	built_in_pwd(t_shell **shell)
{
	t_list	*arg;
	size_t	len;

	arg = NULL;
	if ((*((*shell)->current_command))->args)
		arg = (*((*shell)->current_command))->args->next;
	if (arg && *(arg->name) == '-')
	{
		error_handler(shell, arg->name, ERROR, ERR_NO_OPT);
		return ;
	}
	len = str_len((*shell)->saved_pwd, NULL, 0);
	if (len > 1 && (*shell)->saved_pwd[len - 1] == '/')
		len--;
	write(STDOUT_FILENO, (*shell)->saved_pwd, len);
	write(STDOUT_FILENO, "\n", 1);
}

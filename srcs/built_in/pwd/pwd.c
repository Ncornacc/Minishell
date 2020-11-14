/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/07/20 19:47:44 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 14:49:01 by totartar      ########   odam.nl         */
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

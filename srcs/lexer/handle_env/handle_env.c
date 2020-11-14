/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_env.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:38:18 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/06 11:44:48 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				handle_env(t_shell **shell, char **input, int *i,
		unsigned char *parse)
{
	if (*parse & LEXER)
		handle_env_lexer_level(shell, input, i);
	else
		handle_env_exec_level(shell, input, i, parse);
	if ((*shell)->current_status != SUCCESS)
		return (ERROR);
	return (SUCCESS);
}

void			handle_env_lexer_level(t_shell **shell, char **input, int *i)
{
	int	index;

	index = *i;
	get_offset(shell, input, index, i);
	if (*i != index)
		(*i)--;
}

void			handle_env_exec_level(t_shell **shell, char **input, int *i,
		unsigned char *parse)
{
	if (!((*parse & QUOTED_DOUBLE) && ((*input)[*i + 1] == '"')))
		integrate_env(input, i, shell);
	if ((*shell)->current_status == IGN_ENV_SUB)
		(*shell)->current_status = SUCCESS;
}

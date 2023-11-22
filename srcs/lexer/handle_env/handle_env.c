/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_env.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:18:48 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:18:51 by ncornacc      ########   odam.nl         */
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

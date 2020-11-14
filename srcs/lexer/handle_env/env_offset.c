/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_offset.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:32:51 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/06 12:10:47 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned char	get_offset(t_shell **shell, char **lit_token, int index, int *i)
{
	(*shell)->parse_env = 0;
	if (flag_offset(&((*shell)->parse_env), lit_token, index, i) == IGN_ENV_SUB)
		return (SUCCESS);
	while ((*lit_token)[*i])
	{
		if ((*lit_token)[*i] == '}')
			return (get_offset_brackets(shell, i, index));
		else if ((*lit_token)[*i] == '?')
		{
			if (get_offset_no_opt(shell, lit_token, i))
				return (SUCCESS);
		}
		else if (get_offset_is_option(lit_token, i, index))
		{
			get_offset_skip_option(shell, lit_token, index, i);
			return (get_offset_end(shell));
		}
		else if (!ft_isalnum((*lit_token)[*i]) && (*lit_token)[*i] != '_')
		{
			get_offset_stop_parsing(shell, lit_token, index, i);
			break ;
		}
		(*i)++;
	}
	return (get_offset_end(shell));
}

unsigned char	get_offset_brackets(t_shell **shell, int *i, int index)
{
	if (((*shell)->parse_env & 2) == 2)
		(*i)++;
	else if (*i - index == 1)
	{
		*i = index;
		(*shell)->current_status = IGN_ENV_SUB;
	}
	return (SUCCESS);
}

bool			get_offset_no_opt(t_shell **shell, char **new_literal_token,
		int *i)
{
	if (!(((*shell)->parse_env & 1) == 1)
			|| ((((*shell)->parse_env & 1) == 1)
				&& (*new_literal_token)[*i + 1] != '}'))
	{
		lexer_error_handler(shell, NULL, NO_OPT, ERR_NO_OPT);
		return (true);
	}
	return (false);
}

void			get_offset_ignore_env(t_shell **shell, char **new_literal_token,
		int *i, int index)
{
	if (!ft_strchr("\"'", (*new_literal_token)[*i]))
	{
		*i = index;
		(*shell)->current_status = IGN_ENV_SUB;
	}
}

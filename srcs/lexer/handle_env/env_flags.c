/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_flags.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:18:12 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:18:15 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void			set_env_sub_error(t_shell **shell, char **new_literal_token,
		int *i, unsigned char bracket_sub)
{
	int	n_quotes;

	if (!bracket_sub)
		return ;
	n_quotes = 0;
	if ((*new_literal_token)[*i] == '\\')
		lexer_error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
	if ((*shell)->current_status != SUCCESS)
		return ;
	while ((*new_literal_token)[*i])
	{
		if (ft_strchr("'\"", (*new_literal_token)[*i]))
			n_quotes++;
		if ((*new_literal_token)[*i] == '}')
		{
			if (n_quotes % 2)
				lexer_error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
			else
				lexer_error_handler(shell, NULL, BAD_SUBSTITUTION, ERR_BAD_SUB);
			(*i)++;
			return ;
		}
		(*i)++;
	}
	lexer_error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
}

unsigned char	flag_offset(unsigned char *parse, char **new_literal_token,
		int index, int *i)
{
	if ((*new_literal_token)[*i + 1] && (*new_literal_token)[*i + 1] == '?')
		*parse |= LAST_STATUS;
	if ((*new_literal_token)[*i + 1] && (*new_literal_token)[*i + 1] == '{')
		*parse |= BRACKET_SUB;
	if (*parse)
		*i = index + 2;
	else
		(*i)++;
	if ((*parse & LAST_STATUS))
		return (IGN_ENV_SUB);
	else
		*parse |= (((*new_literal_token)[*i]
					&& (*new_literal_token)[*i] == '?') & LAST_STATUS);
	return (SUCCESS);
}

void			handle_bslash_input(t_shell **shell,
		char **new_literal_token, int i, unsigned char *parse)
{
	if (*parse & LITERAL_BACKSLASH)
	{
		*parse ^= LITERAL_BACKSLASH;
		return ;
	}
	if ((*new_literal_token)[i + 1] == '\\')
		*parse |= LITERAL_BACKSLASH;
	else if ((*new_literal_token)[i + 1] == '$')
		*parse |= LITERAL_ENV;
	else if ((*new_literal_token)[i + 1] == ' ')
		*parse |= LITERAL_WHITESPACE;
	else if (!(*new_literal_token)[i + 1])
	{
		error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
		(*shell)->current_status = BACKSLASH_TERM;
		return ;
	}
	else if (ft_strchr("'\"", (*new_literal_token)[i + 1]))
	{
		if (!(*parse & QUOTED_SINGLE))
			*parse |= LITERAL_QUOTE;
	}
}

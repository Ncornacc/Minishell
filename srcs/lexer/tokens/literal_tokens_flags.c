/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   literal_tokens_flags.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:01:30 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/06 12:38:48 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			flag_literal_bslash(unsigned char *parse, unsigned char *delete)
{
	if (!(*parse & QUOTED_SINGLE))
		*delete = 1;
	*parse |= LITERAL_BACKSLASH;
}

void			flag_literal_env(unsigned char *parse, unsigned char *delete)
{
	if (!(*parse & QUOTED_SINGLE))
		*delete = 1;
	*parse |= LITERAL_ENV;
}

void			flag_bslash_term(t_shell **shell, unsigned char *delete)
{
	(*shell)->current_status = BACKSLASH_TERM;
	*delete = 1;
}

void			flag_literal_quote(unsigned char *parse, unsigned char *delete,
		char to_escape)
{
	if (*parse & QUOTED_DOUBLE)
	{
		*delete = to_escape == '"';
		*parse |= LITERAL_QUOTE;
	}
	else if (!(*parse & QUOTED_SINGLE))
	{
		*delete = 1;
		*parse |= LITERAL_QUOTE;
	}
}

void			flag_quotes(int n_quotes, t_shell **shell)
{
	if (n_quotes % 2)
	{
		(*shell)->current_status = NO_MULTI_LINE;
		error_handler(shell, NULL, NO_MULTI_LINE, ERR_MULTI_LINE);
	}
}

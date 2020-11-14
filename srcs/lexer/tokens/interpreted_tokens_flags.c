/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   interpreted_tokens_flags.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:08:22 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/05 11:36:53 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			set_literal_interpreted(char *line, unsigned char *parse)
{
	if (match_str(line, "\\\"", 0))
		*parse |= LITERAL_QUOTE;
	if (match_str(line, "\\'", 0))
		*parse |= LITERAL_QUOTE;
}

void			reset_parse_bslash(char **new_literal_token, int i,
		unsigned char *parse)
{
	if ((*new_literal_token)[i] == '\\' && (*parse & LITERAL_BACKSLASH))
		*parse ^= LITERAL_BACKSLASH;
}

void			reset_parse_literal_env(char **new_literal_token, int i,
		unsigned char *parse)
{
	if (i > 0 && (*new_literal_token)[i] && (*new_literal_token)[i] == '$'
			&& (*parse & LITERAL_ENV))
		*parse ^= LITERAL_ENV;
}

int				handle_quotes(char **new_literal_token, int i,
		unsigned char *parse, bool trim)
{
	if (*parse & LITERAL_QUOTE)
	{
		*parse ^= LITERAL_QUOTE;
		return (0);
	}
	if ((*new_literal_token)[i] == '\'')
	{
		if (!(*parse & QUOTED_DOUBLE))
		{
			*parse ^= QUOTED;
			*parse ^= QUOTED_SINGLE;
			if (trim)
				(*new_literal_token)[i] = '\0';
			return (1);
		}
	}
	else if (!(*parse & QUOTED_SINGLE))
	{
		*parse ^= QUOTED;
		*parse ^= QUOTED_DOUBLE;
		if (trim)
			(*new_literal_token)[i] = '\0';
		return (1);
	}
	return (0);
}

void			handle_bslash(t_shell **shell, char **new_literal_token, int i,
		unsigned char *parse)
{
	unsigned char	delete;

	delete = 0;
	if ((*new_literal_token)[i + 1] == '\\')
		flag_literal_bslash(parse, &delete);
	else if ((*new_literal_token)[i + 1] == '$')
		flag_literal_env(parse, &delete);
	else if (!(*new_literal_token)[i + 1])
		flag_bslash_term(shell, &delete);
	else if (ft_strchr("'\"", (*new_literal_token)[i + 1]))
		flag_literal_quote(parse, &delete, (*new_literal_token)[i + 1]);
	else if ((*new_literal_token)[i + 1] == '`' && !(*parse & QUOTED_SINGLE))
		delete = 1;
	if (!(*parse & QUOTED) && !delete)
		delete = 1;
	if (delete)
		(*new_literal_token)[i] = '\0';
}

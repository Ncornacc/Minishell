/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   trim_input.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:19:38 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:19:40 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		trim_words(char *line)
{
	int				len;
	unsigned char	parse;

	parse = 0;
	len = 0;
	while (line[len] && (line[len] == ' ' || line[len] == '\t'))
		len++;
	if (!line[len])
		return (0);
	if (ft_strchr("|><;", line[len]))
		return (parse_meta_token(line, len));
	while (line[len] && line[len] != ' ' && line[len] != '\t')
	{
		if (parse & LITERAL_WHITESPACE)
			parse ^= LITERAL_WHITESPACE;
		set_literal(line + len, &parse);
		if (trim_words_is_meta(line[len], parse))
			return (len);
		if (ft_strchr("|><;", line[len]))
			parse = 0;
		trim_quotes(line, &len, parse);
		trim_brackets(line, &len);
		trim_words_update_parse_len(&parse, &len, line);
	}
	return (len);
}

bool	trim_words_is_meta(char c, unsigned char parse)
{
	return ((c == '|' && !(parse & LITERAL_PIPE))
			|| ((c == '>' || c == '<') && !(parse & LITERAL_REDIR))
			|| (c == ';' && !(parse & LITERAL_SEMICOLON)));
}

void	trim_quotes(char *line, int *i, unsigned char parse)
{
	int	trim;

	trim = 1;
	if (parse & LITERAL_QUOTE || (!ft_strchr("\"'", line[*i])))
		return ;
	if (line[*i] == '"' || line[*i] == '\'')
	{
		while ((line + *i)[trim] && (line + *i)[trim] != line[*i])
		{
			if (line[*i] == '"' && (line + *i)[trim] == '\\'
					&& (line + *i)[trim + 1] == '\\')
				trim += 2;
			else if (line[*i] == '"' && (line + *i)[trim] == '\\'
					&& (line + *i)[trim + 1] == line[*i])
				trim += 2;
			else
				trim++;
		}
		if (!(line + *i)[trim])
			return ;
		if (trim > 1)
			*i += trim;
	}
}

void	trim_brackets(char *line, int *i)
{
	int	trim;

	trim = 1;
	if (line[*i] == '{')
	{
		while ((line + *i)[trim] && (line + *i)[trim] != '}')
			trim++;
		if (!(line + *i)[trim])
			return ;
		if (trim > 1)
			*i += trim;
	}
}

void	trim_words_update_parse_len(unsigned char *parse, int *len, char *line)
{
	if (*parse & LITERAL_WHITESPACE
			&& (!line[*len + 2] || !ft_strchr("<>|;", line[*len + 2])))
	{
		*len += 2;
		*parse = 0;
	}
	else
		(*len)++;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_home.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 14:40:42 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/04 17:32:38 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			handle_tilde(char **input, int *i, t_shell **shell,
		unsigned char *parse)
{
	char	*lh_string;
	char	*rh_string;
	size_t	lh_len;
	size_t	rh_len;

	if (((*input)[*i + 1] && !ft_strchr("/ \t", (*input)[*i + 1]))
			|| integrate_home(input, i, shell, parse))
		return ;
	lh_len = *i;
	(*input)[*i] = '\0';
	lh_string = ft_join(*input, "", lh_len);
	if (!lh_string)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	(*i)++;
	rh_len = (*shell)->home_parent_shell_len + str_len(*input + *i, NULL, 0);
	rh_string = ft_join((*shell)->home_parent_shell, *input + *i, rh_len);
	if (!rh_string)
	{
		free(lh_string);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	(*i) += (*shell)->home_parent_shell_len - 2;
	update_input(input, &lh_string, &rh_string, lh_len + rh_len);
	if (!*input)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
}

bool			integrate_home(char **input, int *i, t_shell **shell,
		unsigned char *parse)
{
	char	*lh_string;
	char	*rh_string;
	int		home_len;
	int		size;

	home_len = 0;
	(*input)[*i] = '\0';
	lh_string = create_lh_string(shell, input, parse, &home_len);
	if (home_len == -1)
		return (false);
	(*i)++;
	size = str_len(*input + *i, NULL, 0);
	rh_string = ft_join(*input + *i, NULL, size);
	if (!rh_string)
	{
		free(lh_string);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	size += home_len + *i - 1;
	(*i) = str_len(lh_string, NULL, 0) - 1;
	update_input(input, &lh_string, &rh_string, size);
	if (!*input)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	return (true);
}

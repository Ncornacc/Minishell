/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   read_input.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/02 17:09:17 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/10 12:57:39 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_line(t_shell **shell, char **input)
{
	int		ret;

	ret = get_next_line(STDIN_FILENO, input);
	if (ret == INTERNAL_ERROR)
	{
		free(*input);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	if (ret)
		return ;
	handle_eof(shell, input, ret);
}

void	handle_eof(t_shell **shell, char **input, int ret)
{
	char	*saved;
	char	*new;

	saved = NULL;
	new = NULL;
	if (!ret && !**input)
	{
		free(*input);
		write(1, "exit\n", 5);
		built_in_exit(shell);
	}
	new = ft_join(*input, NULL, str_len(*input, NULL, 0));
	if (!new)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	while (!ret)
	{
		write(1, "  \b\b", 4);
		save_input(shell, input, &saved, &new);
		ret = new_input(shell, input, &saved, &new);
	}
}

void	save_input(t_shell **shell, char **input, char **saved, char **new)
{
	free(*input);
	*input = ft_join(*saved, *new, str_len(*saved, NULL, 0)
			+ str_len(*new, NULL, 0));
	if (!*input)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	free(*saved);
	*saved = ft_join(*input, "", str_len(*input, NULL, 0));
	if (!saved)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	free(*new);
}

int		new_input(t_shell **shell, char **input, char **saved, char **new)
{
	int	ret;

	ret = get_next_line(STDIN_FILENO, new);
	if (ret == 1)
	{
		free(*input);
		*input = ft_join(*saved, *new, str_len(*saved, NULL, 0) +
				str_len(*new, NULL, 0));
		if (!*input)
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		free(*saved);
		*saved = NULL;
		free(*new);
		*new = NULL;
	}
	return (ret);
}

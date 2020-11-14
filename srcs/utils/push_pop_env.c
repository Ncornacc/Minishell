/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   push_pop_env.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 22:01:18 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 18:40:40 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	instead of reallocating entire array, frees one element,
**	shifts next pointers, and assign all remaining space to NULL.
**	updates n_env, not envp_cpy_size because we need latter to push variables.
*/

void	pop_env(t_shell **shell, char *name)
{
	unsigned int	env;

	env = 0;
	while ((*shell)->envp_cpy[env])
	{
		if (match_env((*shell)->envp_cpy[env], name))
		{
			free((*shell)->envp_cpy[env]);
			(*shell)->envp_cpy[env] = NULL;
			(*shell)->n_env--;
			break ;
		}
		env++;
	}
	if (env == (*shell)->n_env)
		return ;
	env++;
	while (env < (*shell)->envp_cpy_size)
	{
		(*shell)->envp_cpy[env - 1] = (*shell)->envp_cpy[env];
		(*shell)->envp_cpy[env] = NULL;
		env++;
	}
}

/*
**	if new variable fits in array, just assign first NULL pointer to it,
**	if not reallocate the array.
*/

int		push_env(t_shell **shell, char *name, int internal)
{
	int env;
	int len;

	env = 0;
	if ((*shell)->n_env < (*shell)->envp_cpy_size)
	{
		while ((*shell)->envp_cpy[env])
			env++;
		len = str_len(name, NULL, 0);
		(*shell)->envp_cpy[env] = ft_join(name, NULL, len);
		if (!(*shell)->envp_cpy[env])
		{
			if (internal)
				return (ERROR);
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		}
		(*shell)->envp_cpy[env + 1] = NULL;
		(*shell)->n_env++;
		return (SUCCESS);
	}
	return (realloc_env(shell, name, internal));
}

void	update_envcpy(t_shell **shell, char **temp)
{
	(*shell)->envp_cpy = temp;
	(*shell)->envp_cpy_size++;
	(*shell)->n_env++;
}

int		realloc_env(t_shell **shell, char *name, int internal)
{
	char			**temp;
	unsigned int	env;

	env = 0;
	temp = malloc(sizeof(char *) * ((*shell)->envp_cpy_size + 2));
	if (!temp)
	{
		if (internal)
			return (ERROR);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	while (env < (*shell)->envp_cpy_size)
	{
		temp[env] = ft_join((*shell)->envp_cpy[env], "",
							str_len((*shell)->envp_cpy[env], NULL, 0));
		free_if_failed(shell, temp[env], temp);
		env++;
	}
	temp[env] = ft_join(name, "", str_len(name, NULL, 0));
	free_if_failed(shell, temp[env], temp);
	temp[env + 1] = NULL;
	free_array((*shell)->envp_cpy);
	update_envcpy(shell, temp);
	return (SUCCESS);
}

void	free_if_failed(t_shell **shell, char *str, char **temp)
{
	if (str)
		return ;
	else
	{
		free_array(temp);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_dir_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 04:42:10 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/09 18:40:56 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_fetch_dir(t_shell **shell, t_list *arg, char **fetch_dir,
		bool *previous)
{
	if (str_len(arg->name, NULL, 0) == 1)
	{
		*fetch_dir = built_in_cd_fetch_oldpwd(shell);
		*previous = true;
	}
	else
	{
		error_handler(shell, arg->name, ERROR, ERR_NO_OPT);
		return (ERROR);
	}
	if (!*fetch_dir)
	{
		error_handler(shell, NULL, ERROR, "OLDPWD not set");
		return (ERROR);
	}
	return (SUCCESS);
}

void	get_new_old_pwd(t_shell **shell, char *tmp, char **new_oldpwd,
		char *new_path)
{
	if (!tmp)
		pop_env(shell, "OLDPWD");
	else
	{
		*new_oldpwd = ft_join("OLDPWD=", tmp, 7 + str_len(tmp, NULL, 0));
		if (!*new_oldpwd && free_paths(NULL, NULL, NULL, new_path))
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		if (built_in_export_with_arg(shell, *new_oldpwd, 1) == ERROR
				&& free_paths(NULL, NULL, new_oldpwd, new_path))
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		free(*new_oldpwd);
	}
}

char	*built_in_cd_fetch_oldpwd(t_shell **shell)
{
	int		i;
	char	*env_str;

	i = 0;
	env_str = NULL;
	while ((*shell)->envp_cpy[i])
	{
		if (match_env((*shell)->envp_cpy[i], "OLDPWD"))
			env_str = (*shell)->envp_cpy[i] + str_len("OLDPWD", NULL, 0) + 1;
		i++;
	}
	return (env_str);
}

char	*fetch_pwd(t_shell **shell)
{
	int		i;
	char	*env_str;

	i = 0;
	env_str = NULL;
	while ((*shell)->envp_cpy[i])
	{
		if (match_env((*shell)->envp_cpy[i], "PWD"))
			env_str = (*shell)->envp_cpy[i] + str_len("PWD", NULL, 0) + 1;
		i++;
	}
	return (env_str);
}

char	*built_in_cd_get_dir(t_shell **shell, t_list *arg, bool *previous)
{
	char	*fetch_dir;
	char	*dir_cpy;

	fetch_dir = NULL;
	if (arg && *(arg->name) == '-')
	{
		if (get_fetch_dir(shell, arg, &fetch_dir, previous) == ERROR)
			return (NULL);
		dir_cpy = ft_join(fetch_dir, NULL, str_len(fetch_dir, NULL, 0));
	}
	else if (arg)
		dir_cpy = ft_join(arg->name, NULL, str_len(arg->name, NULL, 0));
	else
	{
		fetch_dir = check_env_str((*shell)->home_env_name, 4,
				(*shell)->envp_cpy, 0);
		if (!fetch_dir)
		{
			error_handler(shell, NULL, ERROR, "HOME not set");
			return (NULL);
		}
		dir_cpy = ft_join(fetch_dir, NULL, str_len(fetch_dir, NULL, 0));
	}
	return (dir_cpy);
}

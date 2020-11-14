/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_path_utils3.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: totartar <totartar@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/09 04:38:15 by totartar      #+#    #+#                 */
/*   Updated: 2020/11/09 18:37:07 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_new_path(t_shell **shell, char *dir)
{
	char	*new_path;
	size_t	dir_len;
	char	*pwd;

	dir_len = str_len(dir, NULL, 0);
	pwd = (*shell)->saved_pwd;
	if (dir && dir[0] == '/')
		new_path = ft_join(dir, NULL, dir_len);
	else if (!pwd || !pwd[0])
		new_path = ft_join("./", dir, dir_len + 2);
	else
		new_path = concat_new_path(shell, pwd, dir, dir_len);
	if (!new_path)
	{
		free(dir);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	return (new_path);
}

char	*make_new_path(char *pwd, char *dir_mod, char *dir, size_t len)
{
	char	*new_path;

	if (dir_mod)
		new_path = ft_join(pwd, dir_mod, len + 1);
	else
		new_path = ft_join(pwd, dir, len);
	return (new_path);
}

char	*concat_new_path(t_shell **shell, char *pwd, char *dir, size_t dir_len)
{
	size_t	pwd_len;
	char	*dir_mod;
	char	*new_path;

	pwd_len = str_len(pwd, NULL, 0);
	dir_mod = NULL;
	if (pwd[pwd_len] != '/')
	{
		dir_mod = ft_join("/", dir, dir_len + 1);
		if (!dir_mod)
		{
			free(dir);
			error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
		}
	}
	new_path = make_new_path(pwd, dir_mod, dir, pwd_len + dir_len);
	if (!new_path)
	{
		free(dir);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	if (dir_mod)
		free(dir_mod);
	return (new_path);
}

char	*dot_sub(char *new_path, size_t new_path_len)
{
	size_t	len;
	char	*sub_path;

	slash_sub(new_path, new_path_len);
	remove_dot_component(new_path, new_path_len);
	remove_dot_dot_component(new_path, new_path_len);
	len = get_new_len(new_path, new_path_len);
	sub_path = create_sub_path(new_path, len);
	if (!sub_path)
		return (NULL);
	slash_sub(sub_path, len);
	return (sub_path);
}

char	*create_sub_path(char *new_path, size_t len)
{
	char	*sub_path;
	size_t	i;
	size_t	j;

	sub_path = malloc(sizeof(char) * (len + 1));
	if (!sub_path)
		return (NULL);
	sub_path[len] = '\0';
	i = 0;
	j = 0;
	while (j < len)
	{
		if ((new_path)[i])
		{
			sub_path[j] = (new_path)[i];
			j++;
		}
		i++;
	}
	return (sub_path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   shell_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/19 19:59:19 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/09 18:24:17 by totartar      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_pwd(t_shell **shell)
{
	char	buf[PATH_MAX];
	char	*export;
	size_t	len;

	(*shell)->saved_pwd = NULL;
	if (!getcwd(buf, PATH_MAX))
		error_handler(shell, "getcwd:", ERROR, strerror(errno));
	len = str_len(buf, NULL, 0);
	(*shell)->saved_pwd = ft_join(buf, NULL, len);
	if (!(*shell)->saved_pwd)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	export = ft_join("PWD=", (*shell)->saved_pwd, len + 4);
	if (!export)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	if (built_in_export_with_arg(shell, export, 1) == ERROR)
	{
		free(export);
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	}
	free(export);
	return ;
}

t_shell	*new_shell(char **envp)
{
	t_shell	*new;

	new = malloc(sizeof(t_shell));
	if (!new)
		error_handler(NULL, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	new->absolute_path = NULL;
	new->tokens_info = NULL;
	new->tokens = NULL;
	new->commands = NULL;
	new->parse = 0;
	new->parse_env = 0;
	new->current_command = &(new->commands);
	new->current_n_tokens = 0;
	new->current_status = SUCCESS;
	new->status = SUCCESS;
	new->n_env = n_env(envp);
	new->envp_cpy_size = new->n_env;
	new->envp_cpy = copy_array(&new, envp, new->envp_cpy_size);
	assign_home_parent_shell(&new, &new->home_parent_shell);
	assign_pwd(&new);
	assign_built_in(&new);
	new->process_level = 1;
	ft_memcpy(new->home_env_name, "HOME\0", 5);
	ft_memcpy(new->path_env_name, "PATH\0", 5);
	return (new);
}

void	assign_home_parent_shell(t_shell **new, char **home_parent_shell)
{
	*home_parent_shell = check_env_str("HOME", 4, (*new)->envp_cpy, -1);
	(*new)->home_parent_shell_len = str_len(*home_parent_shell, NULL, 0);
	*home_parent_shell = ft_join(*home_parent_shell, NULL,
									(*new)->home_parent_shell_len);
	if (!*home_parent_shell)
		error_handler(NULL, NULL, INTERNAL_ERROR, ERR_INTERNAL);
}

void	assign_built_in(t_shell **shell)
{
	(*shell)->built_in_str[0] = "echo";
	(*shell)->built_in_str[1] = "cd";
	(*shell)->built_in_str[2] = "pwd";
	(*shell)->built_in_str[3] = "export";
	(*shell)->built_in_str[4] = "unset";
	(*shell)->built_in_str[5] = "env";
	(*shell)->built_in_str[6] = "exit";
	(*shell)->built_in_str[7] = NULL;
	(*shell)->built_in_fun[0] = &built_in_echo;
	(*shell)->built_in_fun[1] = &built_in_cd;
	(*shell)->built_in_fun[2] = &built_in_pwd;
	(*shell)->built_in_fun[3] = &built_in_export;
	(*shell)->built_in_fun[4] = &built_in_unset;
	(*shell)->built_in_fun[5] = &built_in_env;
	(*shell)->built_in_fun[6] = &built_in_exit;
	(*shell)->built_in_fun[7] = NULL;
}

/*
**	reset commands and args between calls but also used when exiting
*/

void	reset_shell(t_shell **shell)
{
	(*shell)->current_command = &((*shell)->commands);
	if ((*shell)->tokens)
	{
		free_array((*shell)->tokens);
		(*shell)->tokens = NULL;
	}
	free_commands(shell);
	if ((*shell)->tokens_info)
	{
		free((*shell)->tokens_info);
		(*shell)->tokens_info = NULL;
	}
	if ((*shell)->absolute_path)
		free((*shell)->absolute_path);
	(*shell)->parse = 0;
	(*shell)->parse_env = 0;
	(*shell)->current_status = SUCCESS;
	(*shell)->absolute_path = NULL;
	(*shell)->commands = NULL;
	(*shell)->tokens = NULL;
	*((*shell)->current_command) = NULL;
	(*shell)->current_command = &((*shell)->commands);
}

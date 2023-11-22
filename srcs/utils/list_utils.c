/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:15:26 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:15:29 by ncornacc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*new_node(t_shell **shell, char *name, int value)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		error_handler(shell, NULL, INTERNAL_ERROR, ERR_INTERNAL);
	new->name = name;
	new->value = value;
	new->token_type = LITERAL_TOKEN;
	new->next = NULL;
	return (new);
}

/*
**	returns address of last node in given list
*/

t_list	**last_node(t_list **list)
{
	t_list **tmp;

	tmp = list;
	if (!*tmp)
		return (NULL);
	while ((*tmp)->next)
		tmp = &((*tmp)->next);
	return (tmp);
}

/*
**	appends new to given node location, suffixing any pre-existing next
*/

void	push_to_list(t_list **location, t_list *new)
{
	t_list	*save;
	t_list	**tmp;

	if (!location)
		return ;
	if (!*location)
	{
		*location = new;
		return ;
	}
	save = (*location)->next;
	(*location)->next = new;
	if (!save)
		return ;
	tmp = (last_node(&new));
	(*tmp)->next = save;
}

void	free_list(t_list *list)
{
	t_list	*next_node;

	while (list)
	{
		next_node = list->next;
		if (list->token_type == EXPANDED_TOKEN && list->name)
		{
			free(list->name);
			list->name = NULL;
		}
		free(list);
		list = NULL;
		list = next_node;
	}
}

/*
**  called by reset_shell to free all allocated lists of current command
*/

void	free_all_lists(t_cmd **command)
{
	free_list((*command)->args);
	free_list((*command)->in_files);
	free_list((*command)->out_files);
}

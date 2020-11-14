/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utils2.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mjiam <mjiam@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/04 15:53:25 by mjiam         #+#    #+#                 */
/*   Updated: 2020/11/04 15:53:48 by mjiam         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_current(t_list ***current, t_list **node)
{
	if (*node)
	{
		(*(*current))->next = *node;
		*current = node;
	}
}

void	update_node(t_shell **shell, t_list **node, char *token)
{
	(*node) = new_node(shell, token, NOT_ASSIGNED);
	(*node)->token_type = EXPANDED_TOKEN;
}

void	update_list(t_shell **shell, t_list **list, char *token,
		t_list ***current)
{
	*list = new_node(shell, token, NOT_ASSIGNED);
	(*list)->token_type = EXPANDED_TOKEN;
	*current = list;
}

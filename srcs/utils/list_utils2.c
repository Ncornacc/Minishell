/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   list_utils2.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ncornacc <ncornacc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/22 11:15:36 by ncornacc      #+#    #+#                 */
/*   Updated: 2023/11/22 11:15:39 by ncornacc      ########   odam.nl         */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:27 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:28 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coders	*ft_lstnew(int id)
{
	t_coders	*new_node;

	new_node = (t_coders *)malloc(sizeof(t_coders));
	if (!new_node)
		return (NULL);
	new_node->id = id;
	new_node->last_time_for_compile = 0;
	new_node->time_left_compile = 0;
	new_node->compile = 0;
	new_node->data = NULL;
	new_node->next = NULL;
	new_node->ticket = 0;
	return (new_node);
}

t_coders	*ft_lstlast(t_coders *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	lstadd_back(t_coders **lst, t_coders *new_node)
{
	t_coders	*last;

	if (!lst || !new_node)
		return ;
	if (*lst == NULL)
	{
		*lst = new_node;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_node;
}

t_coders	*all_coders(int number_of_coders)
{
	t_coders	*list;
	t_coders	*node;
	int			index;

	list = NULL;
	index = 1;
	while (index <= number_of_coders)
	{
		node = ft_lstnew(index);
		if (!node)
			return (NULL);
		lstadd_back(&list, node);
		index++;
	}
	return (list);
}

void	free_coders(t_coders *lst)
{
	t_coders	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

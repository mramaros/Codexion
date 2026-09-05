/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:35 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:36 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle_allocs(t_dongle *new_node)
{
	new_node->coder = malloc(sizeof(int) * 2);
	if (!new_node->coder)
		return (0);
	new_node->mutex = malloc(sizeof(pthread_mutex_t));
	if (!new_node->mutex)
	{
		free(new_node->coder);
		return (0);
	}
	return (1);
}

t_dongle	*dongle_ft_lstnew(int id, int number_coders)
{
	t_dongle	*new_node;

	new_node = malloc(sizeof(t_dongle));
	if (!new_node)
		return (NULL);
	if (!init_dongle_allocs(new_node))
	{
		free(new_node);
		return (NULL);
	}
	new_node->id = id;
	new_node->coder[0] = id;
	new_node->coder[1] = (id % number_coders) + 1;
	new_node->time_compile = 0;
	pthread_mutex_init(new_node->mutex, NULL);
	new_node->data = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_dongle	*dongle_ft_lstlast(t_dongle *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	dongle_add_back(t_dongle **lst, t_dongle *new_node)
{
	t_dongle	*last;

	if (!lst || !new_node)
		return ;
	if (*lst == NULL)
	{
		*lst = new_node;
		return ;
	}
	last = dongle_ft_lstlast(*lst);
	last->next = new_node;
}

t_dongle	*all_dongles(int number_of_coders)
{
	t_dongle	*list;
	t_dongle	*node;
	int			index;

	list = NULL;
	index = 1;
	while (index <= number_of_coders)
	{
		node = dongle_ft_lstnew(index, number_of_coders);
		if (!node)
		{
			free_dongles(list);
			return (NULL);
		}
		dongle_add_back(&list, node);
		index++;
	}
	return (list);
}

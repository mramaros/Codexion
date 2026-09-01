#include "codexion.h"
#include <pthread.h>
#include <stdlib.h>

t_dongle	*dongle_ft_lstnew(int id, int number_coders)
{
	t_dongle	*new_node;

	new_node = (t_dongle *)malloc(sizeof(t_dongle));
	if (!new_node)
		return (NULL);
	
	new_node->coder = (int *)malloc(sizeof(int) * 2);
	if (!new_node->coder)
	{
		free(new_node);
		return (NULL);
	}

	new_node->id = id;
	new_node->coder[0] = id;
	if (number_coders == 1)
		new_node->coder[1] = 1;
	else
		new_node->coder[1] = (id % number_coders) + 1;

	new_node->is_used = 0;
	new_node->time_compile = 0;
	new_node->mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(new_node->mutex, NULL);
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
	int			i;

	list = NULL;
	i = 1;
	while (i <= number_of_coders)
	{
		node = dongle_ft_lstnew(i, number_of_coders);
		if (!node)
		{
			free_dongles(list);
			return (NULL);
		}
		dongle_add_back(&list, node);
		i++;
	}
	return (list);
}

void	free_dongles(t_dongle *lst)
{
	t_dongle	*tmp;

	while (lst)
	{
		tmp = lst->next;
		if (lst->mutex)
		{
			pthread_mutex_destroy(lst->mutex);
			free (lst->mutex);
		}
		if (lst->coder)
			free(lst->coder);
		free(lst);
		lst = tmp;
	}
}


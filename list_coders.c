#include "codexion.h"
#include <stdlib.h>

t_coders	*ft_lstnew(int id)
{
	t_coders	*new_node;

	new_node = (t_coders *)malloc(sizeof(t_coders));
	if (!new_node)
		return NULL;
	new_node->id = id;
	new_node->last_time_for_compile = 0;
	new_node->time_left_compile = 0;
	new_node->compile = 0;
	new_node->next = NULL;
	new_node->ticket = 0;
	return new_node;
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

	if (*lst == NULL)
	{
		*lst = new_node;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_node;
}

t_coders*	all_coders(int number_of_coders)
{
	t_coders*	list;
	t_coders	*node;
	int			i;

	i = 1;
	list = NULL;

	while (i <= number_of_coders)
	{
		node = ft_lstnew(i);
		if (!node)
			return NULL;
		lstadd_back(&list, node);
		i++;
	}
	return list;
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

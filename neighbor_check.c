/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   neighbor_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:57 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:58 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	my_neighbor(t_coders *coder, t_coders *other_coder)
{
	int	coder_nbr;

	coder_nbr = coder->data->number_of_coders;
	if (coder_nbr <= 1)
		return (0);
	if (other_coder->id == ((coder->id - 2 + coder_nbr) % coder_nbr + 1)
		|| other_coder->id == (coder->id % coder_nbr + 1))
		return (1);
	return (0);
}

int	my_turn_fifo(t_coders *coder)
{
	t_coders	*all_coders;

	all_coders = coder->data->coder;
	while (all_coders)
	{
		if (all_coders->id != coder->id && my_neighbor(coder, all_coders))
		{
			if (all_coders->ticket > 0 && all_coders->ticket < coder->ticket)
				return (0);
		}
		all_coders = all_coders->next;
	}
	return (1);
}

int	my_turn_edf(t_coders *coder)
{
	t_coders	*tmp;
	long long	my_deadline;
	long long	other_deadline;

	tmp = coder->data->coder;
	my_deadline = coder->last_time_for_compile + coder->data->time_to_burnout;
	while (tmp)
	{
		if (tmp->id != coder->id && my_neighbor(coder, tmp) && tmp->ticket > 0)
		{
			other_deadline = tmp->last_time_for_compile
				+ tmp->data->time_to_burnout;
			if (other_deadline >= my_deadline)
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

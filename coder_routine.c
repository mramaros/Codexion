/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:00:00 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/01 14:48:20 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coders	*coder;
	t_data		*data;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	int			righ_dongle;

	coder = (t_coders *)arg;
	data = coder->data;
	left_dongle = get_dongle_id(data->dongle, coder->id);
	righ_dongle = (coder->id % data->number_of_coders) + 1;
	right_dongle = get_dongle_id(data->dongle, righ_dongle);
	while (coder->compile < data->number_of_compiles_required)
	{
		if (ft_strcmp(data->scheduler, "fifo") == 0)
			is_fonction_fifo(data, coder);
		else if (ft_strcmp(data->scheduler, "edf") == 0)
			is_fonction_edf(data, coder);
		if (simulation_is_end(data))
			break ;
		taking_dongle_and_compile(data, coder, left_dongle, right_dongle);
		pthread_mutex_lock(&data->write_mutex);
		pthread_cond_broadcast(&data->cond);
		pthread_mutex_unlock(&data->write_mutex);
		if (simulation_is_end(data))
			break ;
		log_action(data, coder->id, "is debugging");
		usleep(data->time_to_debug * 1000);
		if (simulation_is_end(data))
			break ;
		log_action(data, coder->id, "is refactoring");
		usleep(data->time_to_refactor * 1000);
		coder->compile++;
	}
	return (NULL);
}

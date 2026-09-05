/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:14 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:18 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*is_fonction_fifo(t_data *data, t_coders *coder)
{
	pthread_mutex_lock(&data->write_mutex);
	data->ticket_place++;
	coder->ticket = data->ticket_place;
	pthread_mutex_unlock(&data->write_mutex);
	pthread_mutex_lock(&data->write_mutex);
	while (!my_turn_fifo(coder) && !data->burn_out_time)
		pthread_cond_wait(&data->cond, &data->write_mutex);
	coder->ticket = 0;
	pthread_cond_broadcast(&data->cond);
	pthread_mutex_unlock(&data->write_mutex);
	usleep(500);
	return (NULL);
}

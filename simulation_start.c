/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:12:15 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:12:16 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_threads(t_data *data)
{
	t_coders	*curr;

	curr = data->coder;
	while (curr)
	{
		curr->data = data;
		curr->last_time_for_compile = data->start_time;
		if (pthread_create(&curr->thread, NULL, coder_routine,
				(void *)curr) != 0)
		{
			printf("Erreur lors de la création du thread %d\n", curr->id);
			return ;
		}
		curr = curr->next;
	}
}

static void	join_threads(t_data *data)
{
	t_coders	*curr;

	curr = data->coder;
	while (curr)
	{
		pthread_join(curr->thread, NULL);
		curr = curr->next;
	}
}

void	start_simulation(t_data *data)
{
	pthread_t	monitor;

	data->start_time = timestamp();
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_cond_init(&data->cond, NULL);
	start_threads(data);
	if (pthread_create(&monitor, NULL, monitoring, (void *)data) != 0)
		return ;
	pthread_join(monitor, NULL);
	join_threads(data);
	pthread_cond_destroy(&data->cond);
	pthread_mutex_destroy(&data->write_mutex);
}

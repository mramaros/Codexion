/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:00:00 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/01 10:00:00 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

long long	timestamp(void);
void		*monitoring(void *arg);
void		*coder_routine(void *arg);

void	start_simulation(t_data *data)
{
	t_coders	*curr;
	pthread_t	monitor;

	data->start_time = timestamp();
	pthread_mutex_init(&data->write_mutex, NULL);
	curr = data->coder;
	pthread_cond_init(&data->cond, NULL);
	while (curr)
	{
		curr->data = data;
		curr->last_time_for_compile = data->start_time;
		if (pthread_create(&curr->thread, NULL, coder_routine,
			(void *)curr) != 0)
		{
			printf("Erreur lors de la création du thread %d\n",
				curr->id);
			return ;
		}
		curr = curr->next;
	}
	if (pthread_create(&monitor, NULL, monitoring, (void *)data) != 0)
		return ;
	pthread_join(monitor, NULL);
	curr = data->coder;
	while (curr)
	{
		pthread_join(curr->thread, NULL);
		curr = curr->next;
	}
	pthread_cond_destroy(&data->cond);
	pthread_mutex_destroy(&data->write_mutex);
}

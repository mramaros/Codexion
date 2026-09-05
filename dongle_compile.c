/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_compile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:10:43 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/05 06:38:40 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static void	lock_dongles(t_dongle *left, t_dongle *right)
{
	if (left->id < right->id)
	{
		pthread_mutex_lock(left->mutex);
		pthread_mutex_lock(right->mutex);
	}
	else
	{
		pthread_mutex_lock(right->mutex);
		pthread_mutex_lock(left->mutex);
	}
}

static void	unlock_dongles(t_dongle *left, t_dongle *right)
{
	pthread_mutex_unlock(left->mutex);
	pthread_mutex_unlock(right->mutex);
}

static void	manage_dongle_order(t_dongle *left, t_dongle *right,
		t_data *data, int coder_id)
{
	if (left->id < right->id)
	{
		manage_dongle(left, data, coder_id, "has taken left dongle");
		manage_dongle(right, data, coder_id, "has taken right dongle");
	}
	else
	{
		manage_dongle(right, data, coder_id, "has taken right dongle");
		manage_dongle(left, data, coder_id, "has taken left dongle");
	}
}

static void	excecute_compilation(t_data *data, t_coders *coder,
		t_dongle *left_dongle, t_dongle *right_dongle)
{
	long long	time_now;

	lock_dongles(left_dongle, right_dongle);
	pthread_mutex_lock(&data->write_mutex);
	coder->last_time_for_compile = timestamp();
	pthread_mutex_unlock(&data->write_mutex);
	manage_dongle_order(left_dongle, right_dongle, data, coder->id);
	log_action(data, coder->id, "is compiling");
	usleep(data->time_to_compile * 1000);
	time_now = timestamp();
	left_dongle->time_compile = time_now;
	right_dongle->time_compile = time_now;
	unlock_dongles(left_dongle, right_dongle);
	pthread_mutex_lock(&data->write_mutex);
	coder->last_time_for_compile = timestamp();
	pthread_mutex_unlock(&data->write_mutex);
}

void	*taking_dongle_and_compile(t_data *data, t_coders *coder,
		t_dongle *left_dongle, t_dongle *right_dongle)
{
	if (left_dongle == right_dongle)
	{
		pthread_mutex_lock(left_dongle->mutex);
		manage_dongle(left_dongle, data, coder->id, "has taken left dongle");
		while (!simulation_is_end(data))
			usleep(1000);
		pthread_mutex_unlock(left_dongle->mutex);
		return (NULL);
	}
	excecute_compilation(data, coder, left_dongle, right_dongle);
	return (NULL);
}

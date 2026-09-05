/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:10:23 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:10:24 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_for_turn(t_data *data, t_coders *coder)
{
	if (ft_strcmp(data->scheduler, "fifo") == 0)
		is_fonction_fifo(data, coder);
	else if (ft_strcmp(data->scheduler, "edf") == 0)
		is_fonction_edf(data, coder);
}

static void	do_debug_and_refactor(t_data *data, t_coders *coder)
{
	log_action(data, coder->id, "is debugging");
	usleep(data->time_to_debug * 1000);
	if (simulation_is_end(data))
		return ;
	log_action(data, coder->id, "is refactoring");
	usleep(data->time_to_refactor * 1000);
	coder->compile++;
}

static int	compile_once(t_data *data, t_coders *coder,
		t_dongle *left_dongle, t_dongle *right_dongle)
{
	wait_for_turn(data, coder);
	if (simulation_is_end(data))
		return (1);
	taking_dongle_and_compile(data, coder, left_dongle, right_dongle);
	pthread_mutex_lock(&data->write_mutex);
	pthread_cond_broadcast(&data->cond);
	pthread_mutex_unlock(&data->write_mutex);
	if (simulation_is_end(data))
		return (1);
	do_debug_and_refactor(data, coder);
	if (simulation_is_end(data))
		return (1);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coders	*coder;
	t_data		*data;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	int			right_dongle_id;

	coder = (t_coders *)arg;
	data = coder->data;
	left_dongle = get_dongle_id(data->dongle, coder->id);
	right_dongle_id = (coder->id % data->number_of_coders) + 1;
	right_dongle = get_dongle_id(data->dongle, right_dongle_id);
	while (coder->compile < data->number_of_compiles_required)
	{
		if (compile_once(data, coder, left_dongle, right_dongle))
			break ;
	}
	return (NULL);
}

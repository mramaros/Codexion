/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_for_burn-out.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:50 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:51 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_is_end(t_data *data)
{
	int	ending;

	pthread_mutex_lock(&data->write_mutex);
	ending = data->burn_out_time;
	pthread_mutex_unlock(&data->write_mutex);
	return (ending);
}

static void	print_burn_out(t_data *data, t_coders *coder)
{
	long long	timestamp_in_ms;
	const char	*color;

	timestamp_in_ms = timestamp() - data->start_time;
	if (data->outstanding_mode)
	{
		color = get_color_for_coder(coder->id);
		printf("%s%lld  %d %s\033[0m\n", color, timestamp_in_ms,
			coder->id, "burned out");
	}
	else
		printf("%lld  %d %s\n", timestamp_in_ms, coder->id, "burned out");
	data->burn_out_time = 1;
	pthread_cond_broadcast(&data->cond);
}

static int	check_coder_state(t_data *data, t_coders *coder, long long now)
{
	if (coder->compile < data->number_of_compiles_required)
	{
		if (now - coder->last_time_for_compile > data->time_to_burnout)
			return (1);
		return (0);
	}
	return (0);
}

static int	monitor_one_cycle(t_data *data)
{
	t_coders	*coder;
	long long	now;
	int			all_finished;

	coder = data->coder;
	all_finished = 1;
	now = timestamp();
	while (coder)
	{
		if (check_coder_state(data, coder, now))
		{
			print_burn_out(data, coder);
			return (1);
		}
		if (coder->compile < data->number_of_compiles_required)
			all_finished = 0;
		coder = coder->next;
	}
	if (all_finished)
	{
		data->burn_out_time = 1;
		pthread_cond_broadcast(&data->cond);
		return (1);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->write_mutex);
		if (monitor_one_cycle(data))
		{
			pthread_mutex_unlock(&data->write_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->write_mutex);
		usleep(1000);
	}
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:12:24 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:13:26 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	timestamp(void)
{
	struct timeval	time_laps;

	gettimeofday(&time_laps, NULL);
	return ((time_laps.tv_sec * 1000LL) + (time_laps.tv_usec / 1000));
}

void	log_action(t_data *data, int coder_id, char *msg)
{
	long long	timestamp_in_ms;
	const char	*color;

	pthread_mutex_lock(&data->write_mutex);
	if (!data->burn_out_time)
	{
		timestamp_in_ms = timestamp() - data->start_time;
		if (data->outstanding_mode)
		{
			color = get_color_for_coder(coder_id);
			printf("%s%lld  %d %s\033[0m\n", color, timestamp_in_ms,
				coder_id, msg);
		}
		else
			printf("%lld  %d %s\n", timestamp_in_ms, coder_id, msg);
	}
	pthread_mutex_unlock(&data->write_mutex);
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
			free(lst->mutex);
		}
		if (lst->coder)
			free(lst->coder);
		free(lst);
		lst = tmp;
	}
}

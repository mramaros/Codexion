#include "codexion.h"
#include <pthread.h>
#include <unistd.h>

int	simulation_is_end(t_data *data)
{
	int	ending;

	pthread_mutex_lock(&data->write_mutex);
	ending = data->burn_out_time;
	pthread_mutex_unlock(&data->write_mutex);
	return ending;
}

void	*monitoring(void *arg)
{
	t_data		*data;
	t_coders	*coder;
	long long	now;
	int			all_finished;
	long long	timestamp_in_ms;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->write_mutex);
		coder = data->coder;
		all_finished = 1;
		now = timestamp();
		while (coder)
		{
			if (coder->compile < data->number_of_compiles_required)
			{
				if (now - coder->last_time_for_compile > data->time_to_burnout)
				{
					timestamp_in_ms = timestamp() - data->start_time;
					printf("%lld  %d %s\n", timestamp_in_ms, coder->id, "burned out");
					data->burn_out_time = 1;
					pthread_cond_broadcast(&data->cond);
					pthread_mutex_unlock(&data->write_mutex);
					return NULL;
				}
				if (coder->compile < data->number_of_compiles_required)
					all_finished = 0;
			}
			coder = coder->next;
		}
		if (all_finished)
		{
			data->burn_out_time = 1;
			pthread_cond_broadcast(&data->cond);
			pthread_mutex_unlock(&data->write_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&data->write_mutex);
		usleep(1000);
	}
	return NULL;
}

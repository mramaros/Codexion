#include "codexion.h"
# include <sys/time.h>


long long	timestamp(void)
{
	struct timeval	time_laps;

	gettimeofday(&time_laps, NULL);
	return ((time_laps.tv_sec * 1000LL) + (time_laps.tv_usec / 1000));
}

void	log_action(t_data *data, int coder_id, char *msg)
{
	long long	timestamp_in_ms;

	pthread_mutex_lock(&data->write_mutex);
	
	if (!data->burn_out_time)
	{
		timestamp_in_ms = timestamp() - data->start_time;
		printf("%lld  %d %s\n", timestamp_in_ms, coder_id, msg);
	}
	pthread_mutex_unlock(&data->write_mutex);
}

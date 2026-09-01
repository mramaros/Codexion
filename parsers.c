#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "codexion.h"

int ft_strcmp(char *str, char *str2)
{
	int	i;

	i = 0;
	while (str[i] && str2[i] && str[i] == str2[i])
		i++;
	return ((unsigned char)str[i] - (unsigned char)str2[i]);
}

static int	ft_is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	Check_valide_data(t_data data_base)
{
	if (ft_strcmp(data_base.scheduler, "fifo") != 0 &&
		ft_strcmp(data_base.scheduler, "edf") != 0)
	{
		printf("[ERROR MESSAGE]: invalide input\n");
		exit(EXIT_FAILURE);
	}
        if (data_base.number_of_coders <= 0 || data_base.time_to_burnout <= 0 ||
            data_base.time_to_compile <= 0 ||
            data_base.number_of_compiles_required <= 0 ||
            data_base.time_to_debug <= 0 || data_base.time_to_refactor <= 0 ||
            data_base.dongle_cooldown < 0)
		{
          printf("[ERROR]: not proprely structured\n");
          exit(EXIT_FAILURE);
        }
}

t_data	parser(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 9)
	{
		printf("Usage format : ./codexion <number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> <scheduler>");
		exit(EXIT_FAILURE);
	}
	i = 1;
	while (i <= 7)
	{
		if (!ft_is_number(argv[i]))
		{
			printf("[ERROR]: Argument %d is not a valid positive number\n", i);
			exit(EXIT_FAILURE);
		}
		i++;
	}
        t_data data_base;

        data_base.number_of_coders = atoi(argv[1]);
        data_base.time_to_burnout = atoi(argv[2]);
        data_base.time_to_compile = atoi(argv[3]);
        data_base.time_to_debug = atoi(argv[4]);
        data_base.time_to_refactor = atoi(argv[5]);
        data_base.number_of_compiles_required = atoi(argv[6]);
        data_base.dongle_cooldown = atoi(argv[7]);
        data_base.scheduler = argv[8];
		data_base.start_time = 0;
		data_base.ticket_place = 0;
		data_base.burn_out_time = 0;
        Check_valide_data(data_base);

        return data_base;
}

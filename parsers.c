/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:12:06 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:12:07 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_strcmp(char *str, char *str2)
{
	int	index;

	index = 0;
	while (str[index] && str2[index] && str[index] == str2[index])
		index++;
	return ((unsigned char)str[index] - (unsigned char)str2[index]);
}

static int	ft_is_number(char *str)
{
	int	index;

	index = 0;
	if (!str || !str[0])
		return (0);
	while (str[index])
	{
		if (str[index] < '0' || str[index] > '9')
			return (0);
		index++;
	}
	return (1);
}

static void	check_valid_data(t_data *data_base)
{
	if (ft_strcmp(data_base->scheduler, "fifo") != 0
		&& ft_strcmp(data_base->scheduler, "edf") != 0)
	{
		printf("[ERROR MESSAGE]: invalide input\n");
		exit(EXIT_FAILURE);
	}
	if (data_base->number_of_coders <= 0 || data_base->time_to_burnout <= 0
		|| data_base->time_to_compile <= 0
		|| data_base->number_of_compiles_required <= 0
		|| data_base->time_to_debug <= 0 || data_base->time_to_refactor <= 0
		|| data_base->dongle_cooldown < 0)
	{
		printf("[ERROR]: not proprely structured\n");
		exit(EXIT_FAILURE);
	}
}

static void	check_arguments(int argc, char **argv, t_data *data_base)
{
	int	index;

	index = 1;
	if (argc == 10)
	{
		if (ft_strcmp(argv[9], "--outstanding") != 0)
		{
			printf("[ERROR]: invalid option: %s\n", argv[9]);
			exit(EXIT_FAILURE);
		}
		data_base->outstanding_mode = 1;
	}
	else
		data_base->outstanding_mode = 0;
	while (index <= 7)
	{
		if (!ft_is_number(argv[index]))
		{
			printf("[ERROR]: Argument %d is not a valid positive number\n",
				index);
			exit(EXIT_FAILURE);
		}
		index++;
	}
}

t_data	parser(int argc, char **argv)
{
	t_data	data_base;

	if (argc != 9 && argc != 10)
	{
		printf("Usage: ./codexion <number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> ");
		printf("<scheduler> [--outstanding]\n");
		exit(EXIT_FAILURE);
	}
	check_arguments(argc, argv, &data_base);
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
	check_valid_data(&data_base);
	return (data_base);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 01:11:41 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/03 01:11:42 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	all_data;
	int		number_of_coders;

	all_data = parser(argc, argv);
	number_of_coders = all_data.number_of_coders;
	all_data.coder = all_coders(number_of_coders);
	all_data.dongle = all_dongles(number_of_coders);
	if (!all_data.coder || !all_data.dongle)
	{
		printf("Erreur d'initialisation.\n");
		free_coders(all_data.coder);
		free_dongles(all_data.dongle);
		return (1);
	}
	start_simulation(&all_data);
	free_coders(all_data.coder);
	free_dongles(all_data.dongle);
	return (0);
}

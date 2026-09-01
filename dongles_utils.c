/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mramaros <mramaros@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:23:28 by mramaros          #+#    #+#             */
/*   Updated: 2026/09/01 10:23:30 by mramaros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*get_dongle_id(t_dongle *dongle, int id)
{
	t_dongle	*current;

	current = dongle;
	while (current)
	{
		if (current->id == id)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	manage_dongle(t_dongle *dongle, t_data *data, int coder_id, char *msg)
{
	(void)dongle;
	if (!simulation_is_end(data))
	{
		log_action(data, coder_id, msg);
	}
}

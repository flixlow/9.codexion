/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 09:27:37 by flauweri          #+#    #+#             */
/*   Updated: 2026/05/12 17:49:46 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	try_to_take(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	if (dongle->is_unavailable || (dongle->cooldown > get_time_ms()))
	{
		pthread_mutex_unlock(&dongle->dongle_mutex);
		return (0);
	}
	dongle->is_unavailable = 1;
	pthread_mutex_unlock(&dongle->dongle_mutex);
	return (1);
}

int	am_i_first(t_coder *coder)
{
	int	first;

	pthread_mutex_lock(&coder->global->scheduler_mutex);
	first = coder->global->queue[0];
	pthread_mutex_unlock(&coder->global->scheduler_mutex);
	return ((first % 2) == (coder->name % 2));
}

void	pop_n_push(t_coder *coder)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&coder->global->scheduler_mutex);
	while (i < coder->global->config.n_coders
		&& coder->global->queue[i] != coder->name)
		i++;
	while (i < coder->global->config.n_coders - 1)
	{
		coder->global->queue[i] = coder->global->queue[i + 1];
		i++;
	}
	coder->global->queue[i] = coder->name;
	pthread_mutex_unlock(&coder->global->scheduler_mutex);
}

void	edf_sort_queue(t_coder *coder)
{
	int		i;
	int		j;
	int		*queue;
	t_coder	*coders;

	i = 0;
	pthread_mutex_lock(&coder->global->scheduler_mutex);
	queue = coder->global->queue;
	coders = coder->global->coders;
	while (i < coder->global->config.n_coders)
	{
		j = 0;
		while (j < (coder->global->config.n_coders - 1))
		{
			if (get_coder_deadline(&coders[queue[j]])
				> get_coder_deadline(&coders[queue[j + 1]]))
				ft_swap(&queue[j], &queue[j + 1]);
			j++;
		}
		i++;
	}
	pthread_mutex_unlock(&coder->global->scheduler_mutex);
}

void	has_taken_a_dongle(t_coder *coder, t_dongle *first, t_dongle *second)
{
	while (simulation_is_running(coder->global))
	{
		if (am_i_first(coder))
		{
			if (try_to_take(first))
			{
				if (try_to_take(second))
				{
					print(coder->global, coder->name, "has taken a dongle");
					print(coder->global, coder->name, "has taken a dongle");
					if (coder->global->config.scheduler)
						edf_sort_queue(coder);
					pop_n_push(coder);
					break ;
				}
				else
					release_dongle(first);
			}
		}
		// usleep(500);
	}
}

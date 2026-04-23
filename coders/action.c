/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/23 14:45:10 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	is_debugging(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is debugging");
	usleep(coder->global->config.debug * 1000);
}

void	is_refactoring(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is refactoring");
	usleep(coder->global->config.refactor * 1000);
}

int	is_compiling(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
	{
		release_dongle(coder->dongle_one);
		release_dongle(coder->dongle_two);
		return (1);
	}
	print(coder->global, coder->name, "is compiling");
	pthread_mutex_lock(&coder->coder_mutex);
	coder->burnout = get_time_ms() + coder->global->config.burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	usleep(coder->global->config.compile * 1000);
	return (0);
}

int	try_to_take(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->cooldown - get_time_ms() > 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	am_i_first(t_coder *coder)
{
	int	first;

	pthread_mutex_lock(&coder->global->scheduler_mutex);
	first = coder->global->queue[0];
	pthread_mutex_unlock(&coder->global->scheduler_mutex);
	return (first == coder->name);
}

void	pop_n_push(t_coder *coder)
{
	int	i;

	i = 0;
	while (i < coder->global->config.n_coders
		&& coder->global->queue[i] != coder->name)
		i++;
	while (i < coder->global->config.n_coders - 1)
	{
		coder->global->queue[i] = coder->global->queue[i + 1];
		i++;
	}
	coder->global->queue[i] = coder->name;
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
					pthread_mutex_unlock(&first->mutex);
			}
		}
	}
}

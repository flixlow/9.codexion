/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 19:00:21 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void is_debugging(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is debugging");
	usleep(coder->global->config.time_to_debug * 1000);
}

void is_refactoring(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is refactoring");
	usleep(coder->global->config.time_to_refactor * 1000);
}

void is_compiling(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is compiling");
	coder->burnout_timing = get_time_ms() + coder->global->config.time_to_burnout;
	usleep(coder->global->config.time_to_compile * 1000);
}

int	try_to_take(t_coder *coder, t_dongle *dongle)
{
	long time_to_wait;

	if (!simulation_is_running(coder->global))
		return (1);
	pthread_mutex_lock(&dongle->mutex);
	time_to_wait = dongle->last_released - get_time_ms();
	if (time_to_wait > 0)
		usleep(time_to_wait * 1000);
	print(coder->global, coder->name, "has taken a dongle");
	return (0);
}

void has_taken_a_dongle(t_coder *coder)
{
	if (coder->dongle_one->name < coder->dongle_two->name)
	{
		if (try_to_take(coder, coder->dongle_one))
			return ;
		if (try_to_take(coder, coder->dongle_two))
			release_dongle(coder->dongle_one, coder->global->start);
	}
	else
	{
		if (try_to_take(coder, coder->dongle_two))
			return ;
		if (try_to_take(coder, coder->dongle_one))
			release_dongle(coder->dongle_two, coder->global->start);
	}
}

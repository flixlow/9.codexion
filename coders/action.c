/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 15:33:43 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void is_debugging(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return;
	print(coder->global, coder->name, "is debugging");
	usleep(coder->global->config.time_to_debug * 1000);
}

void is_refactoring(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return;
	print(coder->global, coder->name, "is refactoring");
	usleep(coder->global->config.time_to_refactor * 1000);
}

void is_compiling(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return;
	print(coder->global, coder->name, "is compiling");
	coder->burnout_timing = get_time_ms() + coder->global->config.time_to_burnout;
	usleep(coder->global->config.time_to_compile * 1000);
}

void try_to_take(t_coder *coder, t_dongle *dongle)
{
	int cooldown;
	long time;
	long time_to_wait;

	cooldown = coder->global->config.dongle_cooldown;
	time_to_wait = get_time_ms() - (dongle->last_released + cooldown);
	if (time_to_wait < 0)
		usleep(time_to_wait * -1 * 1000);
	if (!simulation_is_running(coder->global))
		return ;
	pthread_mutex_lock(&dongle->mutex);
	time = get_time_ms() - coder->global->start;
	print(coder->global, coder->name, "has taken a dongle");
}

void has_taken_a_dongle(t_coder *coder)
{
	if (coder->dongle_one->name < coder->dongle_two->name)
	{
		try_to_take(coder, coder->dongle_one);
		if (!simulation_is_running(coder->global))
			return (release_dongle(coder->dongle_one, coder->global->start));
		try_to_take(coder, coder->dongle_two);
	}
	else
	{
		if (!simulation_is_running(coder->global))
			return;
		try_to_take(coder, coder->dongle_two);
		if (!simulation_is_running(coder->global))
			return (release_dongle(coder->dongle_two, coder->global->start));
		try_to_take(coder, coder->dongle_one);
	}
}

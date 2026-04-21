/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 09:19:02 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void is_debugging(t_coder *coder)
{	
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"is debugging"
	);
	usleep(coder->global->config.time_to_debug * 1000);
}	

void is_refactoring(t_coder *coder)
{
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"is refactoring"
	);
	usleep(coder->global->config.time_to_refactor * 1000);
}

void is_compiling(t_coder *coder)
{
	long	current_time;

	current_time = get_time_ms() - coder->global->start;
	print(
		coder->global->mutex,
		current_time,
		coder->name,
		"is compiling"
	);
	coder->last_compile_start = current_time;
	usleep(coder->global->config.time_to_compile * 1000);
}

void try_to_take(t_coder *coder, t_dongle *dongle)
{
	int		cooldown;
	long	time;
	long	time_to_wait;

	cooldown = coder->global->config.dongle_cooldown;
	time_to_wait = get_time_ms() - (dongle->last_released + cooldown);
	if (time_to_wait < 0)
		usleep(time_to_wait * -1 * 1000);
	pthread_mutex_lock(&dongle->mutex);
	time = get_time_ms() - coder->global->start;
	print(coder->global->mutex, time, coder->name, "has taken a dongle");
}

void has_taken_a_dongle(t_coder *coder)
{
	if (coder->dongle_one->name < coder->dongle_two->name)
	{
		try_to_take(coder, coder->dongle_one);
		try_to_take(coder, coder->dongle_two);
	}
	else
	{
		try_to_take(coder, coder->dongle_two);
		try_to_take(coder, coder->dongle_one);
	}
}

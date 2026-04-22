/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/22 11:20:04 by flauweri         ###   ########.fr       */
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

void	is_compiling(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is compiling");
	pthread_mutex_lock(&coder->coder_mutex);
	coder->burnout = get_time_ms() + coder->global->config.burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	usleep(coder->global->config.compile * 1000);
}

int	try_to_take(t_coder *coder, t_dongle *dongle)
{
	long	time_to_wait;

	if (!simulation_is_running(coder->global))
		return (1);
	pthread_mutex_lock(&dongle->mutex);
	time_to_wait = dongle->last_released - get_time_ms();
	if (time_to_wait > 0)
		usleep(time_to_wait);
	print(coder->global, coder->name, "has taken a dongle");
	return (0);
}

void	has_taken_a_dongle(t_coder *coder)
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

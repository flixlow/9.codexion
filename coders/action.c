/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/05/13 14:42:07 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	is_debugging(t_coder *coder)
{
	long	start;

	start = get_time_ms();
	while (simulation_is_running(coder->global)
		&& (get_time_ms() - start) < coder->global->config.debug)
		usleep(1000);
	print(coder->global, coder->name, "is debugging");
}

void	is_refactoring(t_coder *coder)
{
	long	start;

	start = get_time_ms();
	while (simulation_is_running(coder->global)
		&& (get_time_ms() - start) < coder->global->config.refactor)
		usleep(1000);
	print(coder->global, coder->name, "is refactoring");
}

int	is_compiling(t_coder *coder)
{
	long	start;

	start = get_time_ms();
	if (!simulation_is_running(coder->global))
	{
		release_dongle(coder->dongle_one);
		release_dongle(coder->dongle_two);
		return (1);
	}
	print(coder->global, coder->name, "is compiling");
	pthread_mutex_lock(&coder->coder_mutex);
	coder->deadline = get_time_ms() + coder->global->config.burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	while (simulation_is_running(coder->global)
		&& (get_time_ms() - start) < coder->global->config.compile)
		usleep(1000);
	return (0);
}

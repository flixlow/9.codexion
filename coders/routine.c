/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:06:20 by flauweri          #+#    #+#             */
/*   Updated: 2026/05/13 13:28:31 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_unavailable = 0;
	dongle->cooldown = get_time_ms() + dongle->config_cooldown;
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

void	waiting_to_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->global->start_mutex);
	while (coder->global->start == 0)
		pthread_cond_wait(&coder->global->cond, &coder->global->start_mutex);
	pthread_mutex_unlock(&coder->global->start_mutex);
	if (coder->name % 2)
		usleep(1500);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	waiting_to_start(coder);
	while (simulation_is_running(coder->global) &&
	(get_compil_counter(coder) < coder->global->config.n_compiles))
	{
		if (coder->dongle_one->name < coder->dongle_two->name)
			has_taken_a_dongle(coder, coder->dongle_one, coder->dongle_two);
		else
			has_taken_a_dongle(coder, coder->dongle_two, coder->dongle_one);
		if (is_compiling(coder))
			break ;
		release_dongle(coder->dongle_one);
		release_dongle(coder->dongle_two);
		pthread_mutex_lock(&coder->coder_mutex);
		coder->compil_counter++;
		pthread_mutex_unlock(&coder->coder_mutex);
		is_debugging(coder);
		is_refactoring(coder);
	}
	pthread_mutex_lock(&coder->coder_mutex);
	coder->deadline = get_time_ms() + 100000000;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (NULL);
}

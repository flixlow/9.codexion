/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:06:20 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/22 15:43:40 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_dongle *dongle)
{
	dongle->cooldown = get_time_ms() + dongle->config_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void	waiting_to_start(t_global *global)
{
	pthread_mutex_lock(&global->mutex);
	while (global->start == 0)
		pthread_cond_wait(&global->cond, &global->mutex);
	pthread_mutex_unlock(&global->mutex);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	waiting_to_start(coder->global);
	while (get_compil_counter(coder) < coder->global->config.n_compiles)
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
	coder->burnout = get_time_ms() * 2;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (NULL);
}

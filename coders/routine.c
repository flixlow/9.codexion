/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:06:20 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/22 11:09:38 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_dongle *dongle, int dongle_cooldown)
{
	dongle->last_released = get_time_ms() + dongle_cooldown;
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
		has_taken_a_dongle(coder);
		is_compiling(coder);
		release_dongle(coder->dongle_one, coder->global->config.cooldown);
		release_dongle(coder->dongle_two, coder->global->config.cooldown);
		is_debugging(coder);
		is_refactoring(coder);
		pthread_mutex_lock(&coder->coder_mutex);
		coder->compil_counter++;		
		pthread_mutex_unlock(&coder->coder_mutex);
	}
	pthread_mutex_lock(&coder->coder_mutex);
	coder->burnout = get_time_ms() * 2;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (NULL);
}

// printf("[OK] %d has compilied %d times.",
// coder->name, coder->compil_counter);

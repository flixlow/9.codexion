/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:06:20 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 19:06:47 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void release_dongle(t_dongle *dongle, int dongle_cooldown)
{
	dongle->last_released = get_time_ms() + dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void waiting_to_start(t_global *global)
{
	pthread_mutex_lock(&global->mutex);
	while (global->start == 0)
		pthread_cond_wait(&global->cond, &global->mutex);
	pthread_mutex_unlock(&global->mutex);
}

void *routine(void *arg)
{
	t_coder *coder;
	int n_compiles_required;

	coder = (t_coder *)arg;
	n_compiles_required = coder->global->config.number_of_compiles_required;
	waiting_to_start(coder->global);
	while (coder->compil_counter < n_compiles_required)
	{
		has_taken_a_dongle(coder);
		is_compiling(coder);
		release_dongle(coder->dongle_one, coder->global->config.dongle_cooldown);
		release_dongle(coder->dongle_two, coder->global->config.dongle_cooldown);
		is_debugging(coder);
		is_refactoring(coder);
		coder->compil_counter++;
	}
	coder->burnout_timing = get_time_ms() * 2;
	// printf("[OK] %d has compilied %d times.", coder->name, coder->compil_counter);
	return (NULL);
}

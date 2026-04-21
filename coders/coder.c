/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 09:06:20 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 09:06:47 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongle_two->mutex);
	coder->dongle_two->last_released = get_time_ms() - coder->global->start;
	pthread_mutex_unlock(&coder->dongle_one->mutex);
	coder->dongle_one->last_released = get_time_ms() - coder->global->start;
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
	t_coder		*coder;
	int			compil_counter;

	coder = (t_coder *)arg;
	compil_counter = 0;
	waiting_to_start(coder->global);
	while (compil_counter < coder->global->config.number_of_compiles_required)
	{
		has_taken_a_dongle(coder);
		is_compiling(coder);
		release_dongles(coder);
		is_debugging(coder);
		is_refactoring(coder);
		compil_counter++;
	}
	// printf("[OK] %d has compilied %d times.", coder->name, compil_counter);
	return (NULL);
}

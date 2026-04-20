/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 18:02:56 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

void start(t_global *global)
{
	pthread_mutex_lock(&global->mutex);
	global->start = get_time_ms();
	pthread_cond_broadcast(&global->cond);
	pthread_mutex_unlock(&global->mutex);
}

int monitor(t_global *global)
{
	int i;

	i = 0;
	start(global);
	while (i < global->config.number_of_coders)
		pthread_join(global->coders[i++].thread, NULL);
	pthread_mutex_destroy(&global->mutex);
	pthread_cond_destroy(&global->cond);
	free_all(global);
	return (0);
}

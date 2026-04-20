/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 15:03:29 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void is_debugging(t_coder *coder)
{	
	printf("%d is debugging\n", coder->name);	
	usleep(coder->global->config.time_to_debug);
}	

void is_refactoring(t_coder *coder)
{
	printf("%d is refactoring\n", coder->name);
	usleep(coder->global->config.time_to_refactor);
}

void is_compiling(t_coder *coder)
{
	printf("%d is compiling\n", coder->name);
	usleep(coder->global->config.time_to_compile);
}

void release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongle_two->mutex);
	pthread_mutex_unlock(&coder->dongle_one->mutex);
}

void has_taken_a_dongle(t_coder * coder)
{
	if (coder->dongle_one->name < coder->dongle_two->name)
	{
		pthread_mutex_lock(&coder->dongle_one->mutex);
		printf("%d has taken a dongle\n", coder->name);
		pthread_mutex_lock(&coder->dongle_two->mutex);
		printf("%d has taken a dongle\n", coder->name);
	}
	else
	{
		pthread_mutex_lock(&coder->dongle_two->mutex);
		printf("%d has taken a dongle\n", coder->name);
		pthread_mutex_lock(&coder->dongle_one->mutex);
		printf("%d has taken a dongle\n", coder->name);
	}
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
	printf("[OK] %d has compilied %d times.", coder->name, compil_counter);
	return (NULL);
}

// check that no one has burnout
// wait time to compile time (mark the beginning of the compile)
// release dongles mutex unlock
// init dongles colldown
// wait time to debbug
// wait time to refactor
// how to print when multiple thread want to print (mutex on it)

void start(t_global *global)
{
	pthread_mutex_lock(&global->mutex);
	global->start = 1;
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

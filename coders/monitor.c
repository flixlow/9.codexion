/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 11:50:44 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void try_to_take_dongles(t_coder * coder)
{
	int			next;
	t_dongle	*dongle_one;
	t_dongle	*dongle_two;

	next = (coder->name + 1) % coder->global->config.number_of_coders;
	dongle_one = &coder->global->dongles[coder->name];
	dongle_two = &coder->global->dongles[next];
	if (dongle_one->name < dongle_two->name)
	{
		printf("%d has taken a dongle\n", coder->name);
		printf("%d has taken a dongle\n", coder->name);
	}
	else
	{
		printf("%d has taken a dongle\n", coder->name);
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
		try_to_take_dongles(coder);
		// compile();
		// release_dongles(coder);
		compil_counter++;
	}
	return (NULL);
}

// check that no one has burnout
// try to take dongles
// need to wait for dongle, mutex and cond
// succeed for taking dongles
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

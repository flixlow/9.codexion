/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:03:57 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 09:19:13 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *routine(void *arg)
{
	t_coder *coder;
	int compil_counter;

	coder = (t_coder *)arg;
	compil_counter = 0;
	pthread_mutex_lock(&coder->global->mutex);
	while (coder->global->start == 0)
		pthread_cond_wait(&coder->global->cond, &coder->global->mutex);
	pthread_mutex_unlock(&coder->global->mutex);
	// waiting to start
	// start
	// check that no one has burnout
	// try to take dongles
	//  need to wait for dongle, mutex and cond
	//  succeed for taking dongles
	//  wait time to compile time (mark the beginning of the compile)
	//
	//  how to print when multiple thread want to print (mutex on it)
	//
	//  while (compil_counter < coder->config.number_of_compiles_required)
	//  	compil_counter++;
	printf("%d ok\n", coder->number);
	return (NULL);
}

int init_thread(t_global *global)
{
	int i;

	i = 0;
	pthread_mutex_init(&global->mutex, NULL);
	pthread_cond_init(&global->cond, NULL);
	while (i < global->config.number_of_coders)
	{
		if (pthread_create(&global->coders[i].thread,
						   NULL, routine, &global->coders[i]))
			return (ft_error(6));
		i++;
	}
	pthread_mutex_lock(&global->mutex);
	global->start = 1;
	pthread_cond_broadcast(&global->cond);
	pthread_mutex_unlock(&global->mutex);
	i = 0;
	while (i < global->config.number_of_coders)
		pthread_join(global->coders[i++].thread, NULL);
	pthread_mutex_destroy(&global->mutex);
	pthread_cond_destroy(&global->cond);
	free_all(global);
	return (0);
}

int init_dongles_and_coders(t_global *global)
{
	int i;
	int n_coders;

	i = 0;
	n_coders = global->config.number_of_coders;
	global->dongles = malloc(sizeof(t_dongle) * n_coders);
	global->coders = malloc(sizeof(t_coder) * n_coders);
	if (global->dongles == NULL || global->coders == NULL)
		return (ft_error(5));
	while (i < n_coders)
	{
		global->dongles[i] = (t_dongle){0};
		global->dongles[i].coder_one = i;
		global->dongles[i].coder_two = i % n_coders;
		global->coders[i] = (t_coder){0};
		global->coders[i].number = i + 1;
		global->coders[i].global = global;
		i++;
	}
	return (0);
}
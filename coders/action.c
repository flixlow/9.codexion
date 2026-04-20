/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 16:47:39 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void print(pthread_mutex_t mutex, long time, int name, char *message)
{
	pthread_mutex_lock(&mutex);
	printf("%ld %d is %sing\n", time, name, message);
	pthread_mutex_unlock(&mutex);
}

void is_debugging(t_coder *coder)
{	
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"debugg"
	);
	usleep(coder->global->config.time_to_debug);
}	

void is_refactoring(t_coder *coder)
{
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"refactor"
	);
	usleep(coder->global->config.time_to_refactor);
}

void is_compiling(t_coder *coder)
{
	long	current_time;

	current_time = get_time_ms() - coder->global->start;
	print(coder->global->mutex, current_time, coder->name, "compil");
	coder->last_compilation = current_time;
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

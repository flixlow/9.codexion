/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 08:28:14 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void print(pthread_mutex_t mutex, long time, int name, char *message)
{
	pthread_mutex_lock(&mutex);
	printf("%ld %d %s\n", time, name, message);
	pthread_mutex_unlock(&mutex);
}

void is_debugging(t_coder *coder)
{	
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"is debugging"
	);
	usleep(coder->global->config.time_to_debug * 1000);
}	

void is_refactoring(t_coder *coder)
{
	print(
		coder->global->mutex,
		get_time_ms() - coder->global->start,
		coder->name,
		"is refactoring"
	);
	usleep(coder->global->config.time_to_refactor * 1000);
}

void is_compiling(t_coder *coder)
{
	long	current_time;

	current_time = get_time_ms() - coder->global->start;
	print(
		coder->global->mutex,
		current_time,
		coder->name,
		"is compiling"
	);
	coder->last_compilation = current_time;
	usleep(coder->global->config.time_to_compile * 1000);
}

void release_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->dongle_two->mutex);
	pthread_mutex_unlock(&coder->dongle_one->mutex);
}

void has_taken_a_dongle(t_coder * coder)
{
	long	time;

	if (coder->dongle_one->name < coder->dongle_two->name)
	{
		pthread_mutex_lock(&coder->dongle_one->mutex);
		time = get_time_ms() - coder->global->start;
		print(coder->global->mutex, time, coder->name, "has taken a dongle");
		pthread_mutex_lock(&coder->dongle_two->mutex);
		time = get_time_ms() - coder->global->start;
		print(coder->global->mutex, time, coder->name, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->dongle_two->mutex);
		time = get_time_ms() - coder->global->start;
		print(coder->global->mutex, time, coder->name, "has taken a dongle");
		pthread_mutex_lock(&coder->dongle_one->mutex);
		time = get_time_ms() - coder->global->start;
		print(coder->global->mutex, time, coder->name, "has taken a dongle");
	}
}

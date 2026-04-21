/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:41:45 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 08:45:16 by flauweri         ###   ########.fr       */
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

void print(pthread_mutex_t mutex, long time, int name, char *message)
{
	pthread_mutex_lock(&mutex);
	printf("%ld %d %s\n", time, name, message);
	pthread_mutex_unlock(&mutex);
}

long get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

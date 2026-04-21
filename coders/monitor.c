/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 11:33:46 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

	start(global);
	while (!global->burnout)
	{
		i = 0;
		while (i < global->config.number_of_coders)
		{
			if (get_time_ms() > global->coders[i].last_compile_start + global->config.time_to_burnout)
			{
				pthread_mutex_lock(&global->stop_mutex);
				global->burnout = 1;
				pthread_mutex_unlock(&global->stop_mutex);
				print(global, get_time_ms(), global->coders[i].name, "burned out");
				break;
			}
			i++;
		}
		usleep(1000);
	}
	i = 0;
	while (i < global->config.number_of_coders)
		pthread_join(global->coders[i++].thread, NULL);
	return (0);
}

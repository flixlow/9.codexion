/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 19:55:07 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_compile_done(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->config.n_coders)
		if (global->coders[i++].compil_counter < global->config.n_compiles)
			return (0);
	return (1);
}

void	start(t_global *global)
{
	pthread_mutex_lock(&global->mutex);
	global->start = get_time_ms();
	pthread_cond_broadcast(&global->cond);
	pthread_mutex_unlock(&global->mutex);
}

int	monitor(t_global *global)
{
	int	i;

	start(global);
	while (simulation_is_running(global))
	{
		if (is_compile_done(global))
			break ;
		i = 0;
		while (i < global->config.n_coders)
		{
			if (global->coders[i].burnout <= get_time_ms())
			{
				pthread_mutex_lock(&global->stop_mutex);
				global->burnout = 1;
				pthread_mutex_unlock(&global->stop_mutex);
				print(global, global->coders[i].name, "burned out");
				return (0);
			}
			i++;
		}
		usleep(1000);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/05/13 13:26:31 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_compil_counter(t_coder *coder)
{
	int	compil_counter;

	pthread_mutex_lock(&coder->coder_mutex);
	compil_counter = coder->compil_counter;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (compil_counter);
}

int	is_compile_done(t_global *global)
{
	int	i;
	int	n_compiles;

	i = 0;
	n_compiles = global->config.n_compiles;
	while (i < global->config.n_coders)
		if (get_compil_counter(&global->coders[i++]) < n_compiles)
			return (0);
	return (1);
}

void	start(t_global *global)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&global->start_mutex);
	global->start = get_time_ms();
	while (i < global->created)
		global->coders[i++].deadline = global->config.burnout + global->start;
	pthread_cond_broadcast(&global->cond);
	pthread_mutex_unlock(&global->start_mutex);
}

long	get_coder_deadline(t_coder *coder)
{
	long	deadline;

	pthread_mutex_lock(&coder->coder_mutex);
	deadline = coder->deadline;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (deadline);
}

int	monitor(t_global *global)
{
	int		i;

	start(global);
	while (simulation_is_running(global))
	{
		i = 0;
		while (i < global->config.n_coders)
		{
			if (get_coder_deadline(&global->coders[i]) <= get_time_ms())
			{
				pthread_mutex_lock(&global->stop_mutex);
				global->burnout = 1;
				pthread_mutex_unlock(&global->stop_mutex);
				print(global, global->coders[i].name, "burned out");
				return (0);
			}
			i++;
		}
		if (is_compile_done(global))
			break ;
		usleep(1500);
	}
	return (0);
}

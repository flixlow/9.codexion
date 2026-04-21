/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:41:45 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 11:36:30 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	simulation_stop(t_global *global)
{
	int	burnout;
	
	pthread_mutex_lock(&global->stop_mutex);
	burnout = global->burnout;
	pthread_mutex_unlock(&global->stop_mutex);
	return (burnout);
}

void print(t_global *global, long time, int name, char *message)
{
	pthread_mutex_lock(&global->print_mutex);
	if (simulation_stop(global) != 0 || strcmp(message, "burned out") == 0)
		printf("%ld %d %s\n", (time - global->start), name, message);
	pthread_mutex_unlock(&global->print_mutex);
}

long get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

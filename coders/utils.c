/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:41:45 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/23 16:09:57 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_swap(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	simulation_is_running(t_global *global)
{
	int	burnout;

	pthread_mutex_lock(&global->stop_mutex);
	burnout = global->burnout;
	pthread_mutex_unlock(&global->stop_mutex);
	return (burnout == 0);
}

void	print(t_global *global, int name, char *message)
{
	pthread_mutex_lock(&global->print_mutex);
	if (simulation_is_running(global) || strcmp(message, "burned out") == 0)
		printf("%ld %d %s\n", (get_time_ms() - global->start), ++name, message);
	pthread_mutex_unlock(&global->print_mutex);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

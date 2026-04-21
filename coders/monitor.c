/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:21:08 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 09:09:16 by flauweri         ###   ########.fr       */
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

	i = 0;
	start(global);
	while (i < global->config.number_of_coders)
		pthread_join(global->coders[i++].thread, NULL);
	pthread_mutex_destroy(&global->mutex);
	pthread_cond_destroy(&global->cond);
	free_all(global);
	return (0);
}

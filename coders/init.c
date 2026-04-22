/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:03:57 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/22 13:39:48 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_mutex_cond(t_global *global)
{
	int	i;

	i = 0;
	pthread_mutex_init(&global->mutex, NULL);
	pthread_mutex_init(&global->print_mutex, NULL);
	pthread_mutex_init(&global->stop_mutex, NULL);
	while (i < global->config.n_coders)
		pthread_mutex_init(&global->coders[i++].coder_mutex, NULL);
	pthread_cond_init(&global->cond, NULL);
	return (0);
}

int	init_thread(t_global *global)
{
	int	i;

	i = 0;
	init_mutex_cond(global);
	while (i < global->config.n_coders)
	{
		if (pthread_create(
				&global->coders[i].thread, NULL, routine, &global->coders[i]))
			return (ft_error(6));
		i++;
	}
	return (0);
}

int	init_dongles(t_global *global)
{
	int	i;

	i = 0;
	global->dongles = malloc(sizeof(t_dongle) * global->config.n_coders);
	if (global->dongles == NULL)
		return (ft_error(5));
	while (i < global->config.n_coders)
	{
		global->dongles[i].name = i;
		global->dongles[i].cooldown = global->config.cooldown + get_time_ms();
		pthread_mutex_init(&global->dongles[i].mutex, NULL);
		i++;
	}
	return (0);
}

int	init_coders(t_global *global)
{
	int	i;
	int	next;

	i = 0;
	global->coders = malloc(sizeof(t_coder) * global->config.n_coders);
	if (global->coders == NULL)
		return (ft_error(5));
	while (i < global->config.n_coders)
	{
		next = (i + 1) % global->config.n_coders;
		global->coders[i].burnout = global->config.burnout + get_time_ms();
		global->coders[i].name = i;
		global->coders[i].compil_counter = 0;
		global->coders[i].global = global;
		global->coders[i].dongle_one = &global->dongles[i];
		global->coders[i].dongle_two = &global->dongles[next];
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:03:57 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 14:09:15 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int init_thread(t_global *global)
{
	int i;

	i = 0;
	pthread_mutex_init(&global->mutex, NULL);
	pthread_cond_init(&global->cond, NULL);
	while (i < global->config.number_of_coders)
	{
		if (pthread_create(&global->coders[i].thread,
						   NULL, routine, &global->coders[i]))
			return (ft_error(6));
		i++;
	}
	monitor(global);
	return (0);
}

int init_dongles_and_coders(t_global *global)
{
	int	i;
	int	n_coders;
	int	next;

	i = 0;
	n_coders = global->config.number_of_coders;
	global->dongles = malloc(sizeof(t_dongle) * n_coders);
	global->coders = malloc(sizeof(t_coder) * n_coders);
	if (global->dongles == NULL || global->coders == NULL)
		return (ft_error(5));
	while (i < n_coders)
	{
		global->dongles[i].name = i;
		pthread_mutex_init(&global->dongles[i++].mutex, NULL);		
	}
	i = 0;
	while (i < n_coders)
	{
		next = (i + 1) % n_coders;
		global->coders[i].name = i;
		global->coders[i].global = global;
		global->coders[i].dongle_one = &global->dongles[i];
		global->coders[i++].dongle_two = &global->dongles[next];
	}
	return (0);
}

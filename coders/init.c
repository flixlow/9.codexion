/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 09:03:57 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 10:25:55 by flauweri         ###   ########.fr       */
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
	int i;
	int n_coders;

	i = 0;
	n_coders = global->config.number_of_coders;
	global->dongles = malloc(sizeof(t_dongle) * n_coders);
	global->coders = malloc(sizeof(t_coder) * n_coders);
	if (global->dongles == NULL || global->coders == NULL)
		return (ft_error(5));
	while (i < n_coders)
	{
		global->dongles[i] = (t_dongle){0};
		global->coders[i] = (t_coder){0};
		global->coders[i].number = i + 1;
		global->coders[i].global = global;
		i++;
	}
	return (0);
}
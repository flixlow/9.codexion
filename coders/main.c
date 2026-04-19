/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:13:53 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/19 18:37:12 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *routine(void *arg)
{
	t_coder	*coder;
	int		compil_counter;

	coder = (t_coder *)arg;
	compil_counter = 0;
	pthread_mutex_lock(&coder->monitor->mutex);
	while (coder->monitor->start == 0)
		pthread_cond_wait(&coder->monitor->cond, &coder->monitor->mutex);
	pthread_mutex_unlock(&coder->monitor->mutex);
	// while (compil_counter < coder->config.number_of_compiles_required)
	// 	compil_counter++;
	printf("%d ok\n", coder->number);
	return (NULL);
}

int init_thread(t_monitor *monitor)
{
	int i;
	
	i = 0;
	pthread_mutex_init(&monitor->mutex, NULL);
	pthread_cond_init(&monitor->cond, NULL);
	while (i < monitor->config.number_of_coders)
	{
		if (pthread_create(&monitor->coders[i].thread,
			NULL, routine, &monitor->coders[i]))
			return (ft_error(6));
		i++;
	}
	pthread_mutex_lock(&monitor->mutex);
	monitor->start = 1;
	pthread_cond_broadcast(&monitor->cond);
	pthread_mutex_unlock(&monitor->mutex);
	i = 0;
	while (i < monitor->config.number_of_coders)
		pthread_join(monitor->coders[i++].thread, NULL);
	pthread_mutex_destroy(&monitor->mutex);
	pthread_cond_destroy(&monitor->cond);
	free_all(monitor);
	return (0);
}

int init_dongles_and_coders(t_monitor *monitor)
{
	int i;
	int	n_coders;

	i = 0;
	n_coders = monitor->config.number_of_coders;
	monitor->dongles = malloc(sizeof(t_dongle) * n_coders);
	monitor->coders = malloc(sizeof(t_coder) * n_coders);
	if (monitor->dongles == NULL || monitor->coders == NULL)
		return (ft_error(5));
	while (i < n_coders)
	{
		monitor->coders[i] = (t_coder){0};
		monitor->dongles[i].coder_one = i;
		monitor->dongles[i].coder_two = i % n_coders;	
		monitor->coders[i].number = i + 1;
		monitor->coders[i].monitor = monitor;
		i++;
	}
	return (0);
}

int main(int ac, char **av)
{
	t_monitor	monitor;

	memset(&monitor, 0, sizeof(t_monitor));
	if (check_args(ac, av))
		return (1);
	stock_config(av, &monitor.config);
	if (init_dongles_and_coders(&monitor))
		return (free_all(&monitor));
	if (init_thread(&monitor))
		return (free_all(&monitor));
	printf("\e[1;32m[OK] End of the program.\e[0m");
	return (0);
}

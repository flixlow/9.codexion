/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:01:03 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/17 12:01:26 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_config *config)
{
	if (config != NULL)
		memset(config, 0, sizeof(t_config));
	printf("Everything has been freed !");
}

void	stock_config(char **av, t_config *config)
{
	config->number_of_coders = atoi(av[1]);
	config->time_to_burnout = atoi(av[2]);
	config->time_to_compile = atoi(av[3]);
	config->time_to_debug = atoi(av[4]);
	config->time_to_refactor = atoi(av[5]);
	config->number_of_compiles_required = atoi(av[6]);
	config->dongle_cooldown = atoi(av[7]);
	config->scheduler = av[8];
}

int	check_arg(char *arg)
{
	int	i;
	int	n;

	i = 0;
	n = strlen(arg);
	while (i < n)
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
		{
			fprintf(stderr, "[ERROR]: %s must be a valid integer >= 0.", arg);
			return (1);
		}
		i++;
	}
	if (arg[0] == '0' && !arg[1])
		return (0);
	n = atoi(arg);
	if (n == 0)
	{
		fprintf(stderr, "[ERROR]: Error occurs when trying to atoi: %s", arg);
		return (1);
	}
	if (n > 20000)
		return (1);
	return (0);
}

int	check_args(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
	{
		fprintf(stderr, "[ERROR] ./codexion <number_of_coders> ");
		fprintf(stderr, "<time_to_burnout> <time_to_compile> <time_to_debug> ");
		fprintf(stderr, "<time_to_refactor> <number_of_compiles_required> ");
		fprintf(stderr, "<dongle_cooldown> <scheduler>");
		return (1);
	}
	while (i <= 7)
	{
		if (check_arg(av[i]))
			return (1);
		i++;
	}
	if (!(strcmp(av[8], "edf") == 0 || strcmp(av[8], "fifo") == 0))
	{
		fprintf(stderr, "[ERROR]: scheduler must be 'edf' or 'fifo'.");
		return (1);
	}
	return (0);
}

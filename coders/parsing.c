/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:01:03 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/17 17:55:13 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	free_all(t_config *config, t_dongle *dongles, t_coder *coders)
{
	if (config != NULL)
		memset(config, 0, sizeof(t_config));
	if (dongles != NULL)
		free(dongles);
	if (coders != NULL)
		free(coders);
	return (1);
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

	i = 0;
	if (arg[0] == '0' && !arg[1])
		return (0);
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (ft_error(1));
		i++;
	}
	if (strlen(arg) > 4)
		return (ft_error(2));
	if (atoi(arg) == 0)
		return (ft_error(3));
	return (0);
}

int	check_args(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (ft_error(0));
	while (i <= 7)
	{
		if (check_arg(av[i++]))
			return (1);
	}
	if (!(strcmp(av[8], "edf") == 0 || strcmp(av[8], "fifo") == 0))
		return (ft_error(4));
	return (0);
}

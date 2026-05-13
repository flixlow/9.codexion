/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:01:03 by flauweri          #+#    #+#             */
/*   Updated: 2026/05/13 15:10:40 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	stock_config(char **av, t_config *config)
{
	config->n_coders = atoi(av[1]);
	config->burnout = atoi(av[2]);
	config->compile = atoi(av[3]);
	config->debug = atoi(av[4]);
	config->refactor = atoi(av[5]);
	config->n_compiles = atoi(av[6]);
	config->cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		config->scheduler = 0;
	if (strcmp(av[8], "edf") == 0)
		config->scheduler = 1;
}

int	check_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!(arg[i] >= '0' && arg[i] <= '9'))
			return (ft_error(1));
		i++;
	}
	if (strlen(arg) > 9)
		return (ft_error(2));
	if (atoi(arg) == 0)
		return (ft_error(1));
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

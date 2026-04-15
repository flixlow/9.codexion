/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:13:53 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/15 13:32:00 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// void stock_config(char **args)
// {
	
// }

int check_arg(char *arg)
{
	int i;
	int len;

	i = 0;
	len = strlen(arg);
	while (i < len)
	{
		if (i < '0' || i > '9')
			return (1);
	}
	return (0);
}

int check_args(int ac, char **av)
{
	int i;

	i = 1;
	if (ac != 9)
	{
		printf("Error: ./codexion <number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> <scheduler>");
		return (1);
	}
	while (i <= 7)
	{
		if (check_arg(av[i]))
		{
			return (1);
		}
		i++;
	}
	if (!strcmp(av[8], "edf") || !strcmp(av[8], "fifo"))
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	if (check_args(ac, av))
		return (1);
	// stock_config(av);
	return (0);
}

// number_of_coders time_to_burnout time_to_compile
// time_to_debug
// time_to_refactor number_of_compiles_required
// dongle_cooldown scheduler
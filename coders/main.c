/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:13:53 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/17 19:07:52 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongles_and_coders(t_config config, t_coder *coders, t_dongle *dongles)
{
	int	i;

	dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
	coders = malloc(sizeof(t_coder) * config.number_of_coders);
	if (dongles == NULL || coders == NULL)
		return (ft_error(5));
	i = 1;
	while (i <= config.number_of_coders)
	{
		dongles[i].coder_one = i;
		dongles[i].coder_two = i + 1;
		coders[i].number = i;
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_config	config;
	t_dongle	*dongles;
	t_coder		*coders;

	dongles = NULL;
	coders = NULL;
	if (check_args(ac, av))
		return (free_all(&config, dongles, coders));
	stock_config(av, &config);
	if (init_dongles_and_coders(config, coders, dongles))
		return (free_all(&config, dongles, coders));
	free_all(&config, dongles, coders);
	printf("\e[1;32m[OK] End of the program.\e[0m");
	return (0);
}

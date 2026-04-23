/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:13:53 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/23 15:03:46 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_all(t_global *global)
{
	if (init_dongles(global))
		return (free_all(global));
	if (init_coders(global))
		return (free_all(global));
	if (init_queue(global))
		return (free_all(global));
	if (init_thread(global))
		return (free_all(global));
	return (0);
}

int	main(int ac, char **av)
{
	t_global	global;

	memset(&global, 0, sizeof(t_global));
	if (check_args(ac, av))
		return (1);
	stock_config(av, &global.config);
	if (init_all(&global))
		return (free_all(&global));
	monitor(&global);
	destroy(&global);
	printf("\e[1;32m[OK] End of the program.\e[0m");
	return (0);
}

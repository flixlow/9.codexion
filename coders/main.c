/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:13:53 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 16:51:16 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	destroy(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->config.number_of_coders)
		pthread_join(global->coders[i++].thread, NULL);
	pthread_mutex_destroy(&global->mutex);
	pthread_mutex_destroy(&global->print_mutex);
	pthread_mutex_destroy(&global->stop_mutex);
	pthread_cond_destroy(&global->cond);
	free_all(global);
	return (0);
}

int main(int ac, char **av)
{
	t_global global;

	memset(&global, 0, sizeof(t_global));
	if (check_args(ac, av))
		return (1);
	stock_config(av, &global.config);
	if (init_dongles_and_coders(&global))
		return (free_all(&global));
	if (init_thread(&global))
		return (free_all(&global));
	monitor(&global);
	printf("\e[1;32m[OK] End of the program.\e[0m");
	return (0);
}

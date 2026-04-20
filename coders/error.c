/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:54:48 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/20 10:31:58 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	free_all(t_global *global)
{
	if (global->dongles != NULL)
		free(global->dongles);
	if (global->coders != NULL)
		free(global->coders);
	return (1);
}

int	ft_error(int error)
{
	fprintf(stderr, "\e[1;31m[ERROR]: \e[0m");
	if (error == 0)
		fprintf(stderr, "./codexion <number_of_coders> "
			"<time_to_burnout> <time_to_compile> <time_to_debug> "
			"<time_to_refactor> <number_of_compiles_required> "
			"<dongle_cooldown> <scheduler>\n");
	if (error == 1)
		fprintf(stderr, "args must be a valid integer >= 0.\n");
	if (error == 2)
		fprintf(stderr, "integer args must be inferior to 9999.\n");
	if (error == 3)
		fprintf(stderr, "occurs when trying to atoi args\n");
	if (error == 4)
		fprintf(stderr, "scheduler must be 'edf' or 'fifo'.\n");
	if (error == 5)
		fprintf(stderr, "Malloc for dongles and coders failed.\n");
	if (error == 6)
		fprintf(stderr, "Threads creation failed.\n");
	return (1);
}

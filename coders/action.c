/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:08:28 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/23 16:54:53 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	is_debugging(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is debugging");
	usleep(coder->global->config.debug * 1000);
}

void	is_refactoring(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
		return ;
	print(coder->global, coder->name, "is refactoring");
	usleep(coder->global->config.refactor * 1000);
}

int	is_compiling(t_coder *coder)
{
	if (!simulation_is_running(coder->global))
	{
		release_dongle(coder->dongle_one);
		release_dongle(coder->dongle_two);
		return (1);
	}
	print(coder->global, coder->name, "is compiling");
	pthread_mutex_lock(&coder->coder_mutex);
	coder->deadline = get_time_ms() + coder->global->config.burnout;
	pthread_mutex_unlock(&coder->coder_mutex);
	usleep(coder->global->config.compile * 1000);
	return (0);
}

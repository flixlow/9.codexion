/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 09:27:37 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/23 14:42:49 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void	edf_sort_queue(t_coder *coder)
{
	int		i;
	int		j;
	int		swap;
	t_coder *c;
	int		*q;

	i = 0;
	pthread_mutex_lock(&coder->global->scheduler_mutex);
	q = coder->global->queue;
	c = coder->global->coders;
	while (i < coder->global->config.n_coders)
	{
		j = 0;
		while (j < (coder->global->config.n_coders - 1))
		{
			if (get_coder_burnout(&c[q[j]]) > get_coder_burnout(&c[q[j + 1]]))
			{
				swap = q[j];
				q[j] = q[j + 1];
				q[j + 1] = swap;
			}
			j++;
		}
		i++;
	}
	i = 0;
	pthread_mutex_unlock(&coder->global->scheduler_mutex);
}

int	init_queue(t_global *global)
{
	int		i;
	int		j;

	global->queue = malloc(sizeof(int) * global->config.n_coders);
	if (global->queue == NULL)
		return (ft_error(5));
	i = 0;
	j = 0;
	while (j < global->config.n_coders)
	{
		global->queue[i++] = j;
		j += 2;
	}
	j = 1;
	while (j < global->config.n_coders)
	{
		global->queue[i++] = j;
		j += 2;
	}
	return (0);
}

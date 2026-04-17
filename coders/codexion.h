/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:32:14 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/17 16:19:48 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h> //printf
# include <string.h> //strlen, memset
# include <stdlib.h> //malloc, free, atoi
# include <pthread.h>

typedef struct s_config
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;	
}			t_config;

typedef struct s_coder
{
	int		number;
}			t_coder;

typedef struct s_dongle
{
	int		coder_one;
	int		coder_two;
}			t_dongle;

int		check_args(int ac, char **av);
int		check_arg(char *arg);
int		free_all(t_config *config, t_dongle *dongles, t_coder *coders);
void	stock_config(char **av, t_config *config);

#endif
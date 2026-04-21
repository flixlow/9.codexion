/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:32:14 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 23:57:59 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_global	t_global;

typedef struct s_config
{
	int		n_coders;
	int		burnout;
	int		compile;
	int		debug;
	int		refactor;
	int		n_compiles;
	int		cooldown;
	char	*scheduler;
}	t_config;

typedef struct s_dongle
{
	int				name;
	int				*queue;
	long			last_released;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int			name;
	int			compil_counter;
	long		burnout;
	t_global	*global;
	pthread_t	thread;
	t_dongle	*dongle_one;
	t_dongle	*dongle_two;
}	t_coder;

typedef struct s_global
{
	t_config		config;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor;
	pthread_mutex_t	mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_cond_t	cond;
	int				burnout;
	long			start;
}	t_global;

// action.c
void	is_debugging(t_coder *coder);
void	is_refactoring(t_coder *coder);
void	is_compiling(t_coder *coder);
int		try_to_take(t_coder *coder, t_dongle *dongle);
void	has_taken_a_dongle(t_coder *coder);
// coder.c
void	release_dongle(t_dongle *dongle, int dongle_cooldown);
void	waiting_to_start(t_global *global);
void	*routine(void *arg);
// ending_program.c
int		destroy(t_global *global);
int		free_all(t_global *global);
int		ft_error(int error);
// init.c
int		init_thread(t_global *global);
int		init_dongles(t_global *global);
int		init_coders(t_global *global);
// monitor.c
int		is_compile_done(t_global *global);
void	start(t_global *global);
int		monitor(t_global *global);
// parsing.c
void	stock_config(char **av, t_config *config);
int		check_arg(char *arg);
int		check_args(int ac, char **av);
// utils.c
int		simulation_is_running(t_global *global);
void	print(t_global *global, int name, char *message);
long	get_time_ms(void);

#endif
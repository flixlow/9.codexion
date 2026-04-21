/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:32:14 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/21 14:59:37 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>	//printf
#include <string.h> //strlen, memset
#include <stdlib.h> //malloc, free, atoi
#include <pthread.h>
#include <unistd.h>	  //usleep
#include <sys/time.h> //gettimeofday

typedef struct s_global t_global;

typedef struct s_config
{
	int number_of_coders;
	int time_to_burnout;
	int time_to_compile;
	int time_to_debug;
	int time_to_refactor;
	int number_of_compiles_required;
	int dongle_cooldown;
	char *scheduler;
} t_config;

typedef struct s_dongle
{
	int name;
	int is_available;
	int *queue;
	long last_released;
	pthread_mutex_t mutex;
} t_dongle;

typedef struct s_coder
{
	int name;
	int compil_counter;
	long burnout_timing;
	t_global *global;
	pthread_t thread;
	t_dongle *dongle_one;
	t_dongle *dongle_two;
} t_coder;

typedef struct s_global
{
	t_config config;
	t_dongle *dongles;
	t_coder *coders;
	pthread_t monitor;
	pthread_mutex_t mutex;
	pthread_mutex_t print_mutex;
	pthread_mutex_t stop_mutex;
	pthread_cond_t cond;
	int burnout;
	long start;
} t_global;

// parsing.c
int check_args(int ac, char **av);
int check_arg(char *arg);
void stock_config(char **av, t_config *config);
// error.c
int free_all(t_global *global);
int ft_error(int error);
// init.c
int init_dongles_and_coders(t_global *global);
int init_thread(t_global *global);
// monitor.c
int monitor(t_global *global);
void start(t_global *global);
// utils.c
long get_time_ms(void);
void print(t_global *global, int name, char *message);
int simulation_is_running(t_global *global);
// action.c
void is_debugging(t_coder *coder);
void is_refactoring(t_coder *coder);
void is_compiling(t_coder *coder);
void try_to_take(t_coder *coder, t_dongle *dongle);
void has_taken_a_dongle(t_coder *coder);
// coder.c
void release_dongle(t_dongle *dongle, long start);
void waiting_to_start(t_global *global);
void *routine(void *arg);

#endif
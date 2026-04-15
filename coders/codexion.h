/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flauweri <flauweri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:32:14 by flauweri          #+#    #+#             */
/*   Updated: 2026/04/15 13:10:36 by flauweri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdarg.h>
# include <stddef.h>
# include <stdio.h> //printf
# include <string.h> //strlen
# include <stdlib.h> //malloc

#endif

typedef struct s_config
{
	int		number_of_coders;
	int 	time_to_burnout;
	int 	time_to_compile;
	int 	time_to_debug;
	int 	time_to_refactor;
	int 	number_of_compiles_required;
	int 	dongle_cooldown;
	char	*scheduler;	
}			t_config;
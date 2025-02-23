/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpittigl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:59 by lpittigl          #+#    #+#             */
/*   Updated: 2025/02/22 14:11:00 by lpittigl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>  // Per la dichiarazione di open
#include <unistd.h> // Per la dichiarazione di close

typedef struct s_data
{
	size_t			time_to_eat;
	size_t			max_time;
	size_t			time_to_sleep;
	int				eating_goal;
	int				nbr_of_philo;
	pthread_mutex_t	*forks;
	int				end_simulation;
	pthread_mutex_t	printing;
	pthread_mutex_t	check_death;
	size_t			start_tm;
}	t_data;

typedef struct s_philo
{
	t_data			*info;
	pthread_t		thread;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	int				pos;
	size_t			last_meal;
	int				curr_eating;
	pthread_mutex_t	eating;
	int				nbr_eat;
	pthread_mutex_t	nbr_eaten;
}	t_philo;

void	*routine(void *philosopher);
size_t	curr_time(void);
int		term_prog(t_philo *philo, int flag, t_data *info, char *c);
void	ft_putendl_fd(char *s, int fd);
int		ft_atoi(const char *str);
int		generate_philos(t_philo **all_philo, t_data *infos);
int		init_mutex(t_data *info);
int		ft_usleep(size_t milliseconds, t_philo *philo);
int		check_and_set(char **argv, int argc, t_data *info);
int		run_threads(t_philo *all_philos, t_data *info);
int		monitor(t_philo *all_philos, t_data *info, int check);
int		better_usleep(size_t milliseconds);
int		philo_print(t_philo *philo, char *s);
int		no_one_died(t_philo *philo);
void	wait_threads(t_philo *all_philos);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpittigl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:19 by lpittigl          #+#    #+#             */
/*   Updated: 2025/02/22 14:10:19 by lpittigl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	init_mutex(t_data *info)
{
	int	i;

	i = 0;
	info->forks = malloc(sizeof(pthread_mutex_t) * info->nbr_of_philo);
	if (!info->forks)
	{
		ft_putendl_fd("Mutex array creation failure", 2);
		return (1);
	}
	while (i < info->nbr_of_philo)
		pthread_mutex_init(info->forks + i++, NULL);
	pthread_mutex_init(&info->printing, NULL);
	pthread_mutex_init(&info->check_death, NULL);
	return (0);
}

void	generate_philos(t_philo **all_philo, t_data *infos)
{
	int	i;

	i = 0;
	*all_philo = malloc(sizeof(t_philo) * (infos->nbr_of_philo));
	if (!(*all_philo))
		terminate_prog(0, 1, infos, "Malloc failure in creating philos");
	while (i < infos->nbr_of_philo)
	{
		(*all_philo)[i].info = infos;
		(*all_philo)[i].left_fork = &infos->forks[i];
		if (i)
			(*all_philo)[i].right_fork = &infos->forks[i - 1];
		else
			(*all_philo)[i].right_fork = &infos->forks[infos->nbr_of_philo - 1];
		(*all_philo)[i].position = i;
		(*all_philo)[i].nbr_eat = 0;
		(*all_philo)[i].curr_eating = 0;
		pthread_mutex_init(&(*all_philo)[i].nbr_eaten, 0);
		pthread_mutex_init(&(*all_philo)[i].eating, 0);
		i++;
	}
}

int	check_and_set(char **argv, int argc, t_data *info)
{
	int	check;

	check = 0;
	if (argc >= 7 || argc < 5)
		return (0);
	if (ft_atoi(argv[1]) > 0 && ++check)
		info->nbr_of_philo = ft_atoi(argv[1]);
	if (ft_atoi(argv[2]) > 0 && ++check)
		info->time_before_death = ft_atoi(argv[2]);
	if (ft_atoi(argv[3]) > 0 && ++check)
		info->time_to_eat = ft_atoi(argv[3]);
	if (ft_atoi(argv[4]) > 0 && ++check)
		info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6 && ft_atoi(argv[5]) >= 0 && ++check)
		info->eating_goal = ft_atoi(argv[5]);
	else
		info->eating_goal = -1;
	if (!info->eating_goal)
		exit(0);
	info->end_simulation = 0;
	if (init_mutex(info))
		exit(1);
	return (check);
}

void	run_threads(t_philo *all_philos, t_data *info)
{
	int	i;

	i = 0;
	info->beginnig_time = get_curr_time();
	while (i < info->nbr_of_philo)
	{
		all_philos[i].last_meal = get_curr_time();
		if (pthread_create(&all_philos[i].thread, 0, routine, &all_philos[i]))
			terminate_prog(all_philos, -1, info, "Failed to create thread");
		pthread_detach(all_philos[i].thread);
		i++;
	}
	monitor(all_philos, info);
}

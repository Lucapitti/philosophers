/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpittigl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:19 by lpittigl          #+#    #+#             */
/*   Updated: 2025/02/22 14:10:19 by lpittigl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	terminate_prog(t_philo *philo, int exit_code, t_data *info, char *c)
{
	int	i;

	i = 0;
	while (i < info->nbr_of_philo)
	{
		if (philo)
		{
			pthread_join(philo[i].thread, 0);
			pthread_mutex_destroy(&philo[i].eating);
			pthread_mutex_destroy(&philo[i].nbr_eaten);
		}
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->printing);
	pthread_mutex_destroy(&info->check_death);
	if (c)
		ft_putendl_fd(c, 2);
	free(philo);
	free(info->forks);
	exit(exit_code);
}

int	check_num_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->nbr_eaten);
	if (philo->nbr_eat >= philo->info->eating_goal)
	{
		pthread_mutex_unlock(&philo->nbr_eaten);
		return (1);
	}
	pthread_mutex_unlock(&philo->nbr_eaten);
	return (0);
}

void	call_death(t_data *info, int i, t_philo *all_philos)
{
	pthread_mutex_lock(&info->check_death);
	info->end_simulation = 1;
	pthread_mutex_unlock(&info->check_death);
	pthread_mutex_lock(&info->printing);
	printf("%lu %i died\n", curr_time() - info->start_tm, i + 1);
	pthread_mutex_unlock(&info->printing);
	terminate_prog(all_philos, 0, info, 0);
}

void	monitor(t_philo *all_philos, t_data *info, int check)
{
	int	i;

	while (1)
	{
		i = 0;
		check = 0;
		while (i < info->nbr_of_philo)
		{
			pthread_mutex_lock(&all_philos[i].eating);
			if (curr_time() - all_philos[i].last_meal >= info->max_time
				&& !(all_philos[i].curr_eating))
				call_death(info, i, all_philos);
			pthread_mutex_unlock(&all_philos[i].eating);
			if (info->eating_goal != -1)
				check += check_num_eaten(all_philos + i);
			i++;
		}
		if (check == info->nbr_of_philo)
		{
			pthread_mutex_lock(&info->check_death);
			info->end_simulation = 1;
			pthread_mutex_unlock(&info->check_death);
			terminate_prog(all_philos, 0, info, 0);
		}
	}
}

int	main(int argc, char **argv)
{
	t_philo	*all_philos;
	t_data	info;

	if (check_and_set(argv, argc, &info))
	{
		ft_putendl_fd("Error, invalid input, exiting ...", 2);
		return (1);
	}
	all_philos = 0;
	generate_philos(&all_philos, &info);
	run_threads(all_philos, &info);
	terminate_prog(all_philos, 0, &info, 0);
}

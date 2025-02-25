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

int	term_prog(t_philo *philo, int exit_code, t_data *info, char *c)
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
		if (i)
			pthread_mutex_destroy(&info->forks[i - 1]);
		else
			pthread_mutex_destroy(&info->forks[info->nbr_of_philo - 1]);
		i++;
	}
	pthread_mutex_destroy(&info->printing);
	pthread_mutex_destroy(&info->check_death);
	if (c)
		ft_putendl_fd(c, 2);
	free(philo);
	free(info->forks);
	return (exit_code);
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

int	call_death(t_data *info, int i)
{
	pthread_mutex_lock(&info->check_death);
	info->end_simulation = 1;
	pthread_mutex_unlock(&info->check_death);
	pthread_mutex_lock(&info->printing);
	printf("%lu %i died\n", curr_time() - info->start_tm, i + 1);
	pthread_mutex_unlock(&info->printing);
	return (0);
}

int	monitor(t_philo *all_philos, t_data *info, int check)
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
			{
				pthread_mutex_unlock(&all_philos[i].eating);
				return (call_death(info, i));
			}
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
			return (0);
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
	if (!info.eating_goal)
		return (0);
	if (init_mutex(&info))
		return (1);
	all_philos = 0;
	if (generate_philos(&all_philos, &info))
		return (term_prog(0, 1, &info, "Malloc failure"));
	if (run_threads(all_philos, &info) == 1)
		return (term_prog(all_philos, 1, &info, "Failed to create thread"));
	return (term_prog(all_philos, 0, &info, 0));
}

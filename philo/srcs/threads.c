/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpittigl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:19 by lpittigl          #+#    #+#             */
/*   Updated: 2025/02/22 14:10:19 by lpittigl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	no_one_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->check_death);
	if (philo->info->end_simulation)
	{
		pthread_mutex_unlock(&philo->info->check_death);
		return (0);
	}
	pthread_mutex_unlock(&philo->info->check_death);
	return (1);
}

int	philo_think(t_philo *philo)
{
	if (!no_one_died(philo))
		return (0);
	return (philo_print(philo, "is thinking"));
}

int	philo_sleep(t_philo *philo)
{
	if (!no_one_died(philo))
		return (0);
	if (philo_print(philo, "is sleeping") == 0)
		return (1);
	return (better_usleep(philo->info->time_to_sleep, philo));
}

int	philo_eat(t_philo *philo, int check)
{
	if (!no_one_died(philo))
		return (0);
	pthread_mutex_lock(philo->right_fork);
	if (philo_print(philo, "has taken a fork") == 1
		&& philo->info->nbr_of_philo > 1)
	{
		pthread_mutex_lock(philo->left_fork);
		if (philo_print(philo, "is eating") == 1)
		{
			pthread_mutex_lock(&philo->eating);
			philo->curr_eating = 1;
			philo->last_meal = curr_time();
			pthread_mutex_unlock(&philo->eating);
			check = better_usleep(philo->info->time_to_eat, philo);
			pthread_mutex_lock(&philo->nbr_eaten);
			philo->nbr_eat++;
			pthread_mutex_unlock(&philo->nbr_eaten);
			pthread_mutex_lock(&philo->eating);
			philo->curr_eating = 0;
		}
		pthread_mutex_unlock(&philo->eating);
		pthread_mutex_unlock(philo->left_fork);
	}
	pthread_mutex_unlock(philo->right_fork);
	return (check);
}

void	*routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	if ((philo->pos + 1) % 2)
	{
		if (!philo_think(philo))
			return (0);
		if (!philo_eat(philo, 0))
			return (0);
	}
	while (1)
	{
		if (!philo_sleep(philo))
			return (0);
		if (!philo_think(philo))
			return (0);
		if (!philo_eat(philo, 0))
			return (0);
	}
	return (0);
}

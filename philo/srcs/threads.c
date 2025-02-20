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

int	philo_print(t_philo *philo, char *s)
{
	if (!no_one_died(philo))
		return (1);
	pthread_mutex_lock(&philo->info->check_death);
	pthread_mutex_lock(&philo->info->printing);
	printf("%lu %i ", get_curr_time() - philo->info->beginnig_time, philo->position + 1);
	printf("%s\n", s);
	pthread_mutex_unlock(&philo->info->printing);
	pthread_mutex_unlock(&philo->info->check_death);
	return (0);
}

int	philo_think(t_philo *philo)
{
	if (!no_one_died(philo))
		return (0);
	philo_print(philo, "is thinking");
	return (1);
}

int	philo_sleep(t_philo *philo)
{
	if (!no_one_died(philo))
		return (0);
	philo_print(philo, "is sleeping");
	usleep(philo->info->time_to_sleep * 1000);
	return (1);
}

int	philo_eat(t_philo *philo)
{
	if (!no_one_died(philo))
		return (0);
	pthread_mutex_lock(philo->right_fork);
	philo_print(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	philo_print(philo, "is eating");
	pthread_mutex_lock(&philo->eating);
	philo->curr_eating++;
	pthread_mutex_unlock(&philo->eating);
	usleep(philo->info->time_to_eat * 1000);
	pthread_mutex_lock(&philo->nbr_eaten);
	philo->nbr_eat++;
	pthread_mutex_unlock(&philo->nbr_eaten);
	pthread_mutex_lock(&philo->eating);
	philo->curr_eating--;
	philo->last_meal = get_curr_time();
	pthread_mutex_unlock(&philo->eating);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

void	*routine(void *philosopher)
{
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	philo->last_meal = get_curr_time();
	if ((philo->position + 1) % 2)
	{
		if (!philo_think(philo))
			return(0);
		if (!philo_eat(philo))
			return(0);
	}
	while (1)
	{
		if (!philo_sleep(philo))
			return(0);
		if (!philo_think(philo))
			return(0);
		if (!philo_eat(philo))
			return(0);
	}
	return (0);
}

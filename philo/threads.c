#include "philosopher.h"

int	no_one_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->check_death);
	if (philo->info->some1_died)
	{
		pthread_mutex_unlock(&philo->info->check_death);
		return (0);
	}
	pthread_mutex_unlock(&philo->info->check_death);
	return (1);
}

void	philo_think(t_philo *philo)
{
	
}

void	*routine(t_philo *philo)
{
	



	if (philo->position % 2)
	{
		while (!no_one_died(philo->info))
		{
			philo_eat();
			philo_think(philo);
			philo_sleep();
		}
	}
	else
	{
		while (!no_one_died(philo->info))
		{
			philo_sleep();
			philo_eat();
			philo_think(philo);
		}
	}
}

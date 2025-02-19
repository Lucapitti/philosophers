#include "philosopher.h"



int	init_mutex(t_data *info)
{
	int	i;

	i = 0;
	info->forks = malloc(sizeof(pthread_mutex_t)*info->nbr_of_philo);
	if (info->forks)
	{
		printf("Mutex array creation failure\n");
		return (1);
	}
	while (i < info->nbr_of_philo)
		pthread_mutex_init(info->forks + i, NULL);
	pthread_mutex_init(&info->printing, NULL);
    pthread_mutex_init(&info->check_death, NULL);
	return (0);
}


void	generate_philos(t_philo **all_philo, t_data *infos)
{
	int	i;

	i = 0;
	*all_philo = malloc(sizeof(t_philo)*(infos->nbr_of_philo));
	if (!(*all_philo))
		end_program();//boh vedi dopo
	while (i < infos->nbr_of_philo)
	{
		(*all_philo)[i].info = infos;
		(*all_philo)[i].left_fork = &infos->forks[i];
		if (i + 1 < infos->nbr_of_philo)
			(*all_philo)[i].right_fork = &infos->forks[i + 1];
		else
			(*all_philo)[i].right_fork = &infos->forks[0];
		(*all_philo)[i].position = i;
		(*all_philo)[i].nbr_eat = 0;
	}
}
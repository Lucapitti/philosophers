#include "philosopher.h"

int	terminate_prog(t_philo *philo, int exit_code, t_data *info, char *c)
{
	int i;

	i = 0;
	pthread_mutex_destroy(&info->printing);
	pthread_mutex_destroy(&info->check_death);
	while (i < info->nbr_of_philo)
	{
		if (philo)
		{
			pthread_mutex_destroy(&philo[i].eating);
			pthread_mutex_destroy(&philo[i].nbr_eaten);
		}
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
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

void	call_death(t_data *info, int i)
{
	pthread_mutex_lock(&info->check_death);
	info->end_simulation = 1;
	pthread_mutex_unlock(&info->check_death);
	pthread_mutex_lock(&info->printing);
	printf("%lu %i died\n", get_curr_time() - info->beginnig_time, i + 1);
	pthread_mutex_unlock(&info->printing);
}

int	monitor(t_philo *all_philos, t_data *info)
{
	int	i;
	int	check;

	while (1)
	{
		i = 0;
		check = 0;
		while (i < info->nbr_of_philo)
		{
			pthread_mutex_lock(&all_philos[i].eating);
			if (get_curr_time() - all_philos[i].last_meal >= info->time_before_death && !(all_philos[i].curr_eating))
			{
				call_death(info, i);
				return (0);
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
	return (0);
}

int main(int argc, char **argv)
{
	t_philo	*all_philos;
	t_data	info;

	if (!check_and_set(argv, argc, &info))
	{
		ft_putendl_fd("Error, invalid input, exiting ...", 2);
	}
	all_philos = 0;
	generate_philos(&all_philos, &info);
	run_threads(all_philos, &info);
	terminate_prog(all_philos, 0, &info, 0);
}

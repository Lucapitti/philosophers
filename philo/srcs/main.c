#include "philosopher.h"

int	wrong_input()
{
	printf("invalid input, exiting...");
	exit (1);
}

int	ft_usleep(size_t milliseconds, t_philo *philo)
{
	size_t	start;
	size_t	time;

	start = get_curr_time();
	time = get_curr_time();
	while ((time - start) < milliseconds)
	{
		if (philo->info->end_simulation)
			return (0);
		usleep(500);
		time = get_curr_time();
	}
	return (1);
}

size_t	get_curr_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	check_and_set(char **argv, int argc, t_data *info)
{
	int check;

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
		info->eating_number = ft_atoi(argv[5]);
	else
		info->eating_number = -1;
	if (!info->eating_number)
		exit(0);
	info->end_simulation = 0;
	if (init_mutex(info))
		exit(1);
	return (check);
}

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
	if (philo->nbr_eat >= philo->info->eating_number)
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
			if (info->eating_number != -1)
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

int	run_threads(t_philo *all_philos, t_data *info)
{
	int	i;

	i = 0;
	info->beginnig_time = get_curr_time();
	while (i < info->nbr_of_philo)
	{
		all_philos[i].last_meal = get_curr_time();
		pthread_create(&all_philos[i].thread, 0, routine, &all_philos[i]);
		pthread_detach(all_philos[i].thread);
		i++;
	}
	return(monitor(all_philos, info));
}

int main(int argc, char **argv)
{
	t_philo *all_philos;
	t_data	info;

	if (!check_and_set(argv, argc, &info))
		wrong_input();
	all_philos = 0;
	generate_philos(&all_philos, &info);
	run_threads(all_philos, &info);
	terminate_prog(all_philos, 0, &info, 0);
}

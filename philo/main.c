#include "philosopher.h"

int	wrong_input()
{
	printf("invalid input, exiting...");
	exit (1);
}

size_t	get_curr_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 100 + time.tv_usec / 100);
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
		success(); // finisce il programma e printa messaggio evviva
	info->end_simulation = 0;
	info->beginnig_time = get_curr_time();
	if (init_mutex(info))
		exit (1);
	return (check);
}

int	terminate_prog(t_philo *philo, int exit_code, t_data *info, char *c)
{
	int i;

	i = 0;
	phthread_mutex_destroy(&info->printing);
	phthread_mutex_destroy(&info->check_death);
	while (i < info->nbr_of_philo)
	{
		if (philo)
			pthread_mutex_destroy(&philo[i].eating);
		phthread_mutex_destroy(&info->forks[i]);
	}
	ft_putendl_fd(c, 2);
	free(philo);
	exit (exit_code);
}


void	run_threads(t_philo *all_philos, t_data info)
{
	int	i;

	i = 0;
	//observer_tread
	while (i < info.nbr_of_philo)
	{
		pthread_create(&all_philos[i].thread, 0, routine, &all_philos[i++]);
		pthread_detach(all_philos[i].thread);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_philo *all_philos;
	t_data	info;

	if (!check_and_set(argv, argc, &info))
		wrong_input();
	all_philos = 0;
	generate_philos(&all_philos, &info);
	run_threads(all_philos, info);
}

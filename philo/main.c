#include "philosopher.h"

int	wrong_input()
{
	printf("invalid input, exiting...");
	exit (1);
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
		(*all_philo)->left_fork = &infos->forks[i];
		if (i + 1 < infos->nbr_of_philo)
			(*all_philo)->right_fork = &infos->forks[i + 1];
		else
			(*all_philo)->right_fork = &infos->forks[0];
		(*all_philo)->position = i;
	}
}

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
	return (0);
	
}

int	check_and_set(char **argv, int argc, t_data *info)
{
	int check;

	check == 0;
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
	info->some1_died = 0;
	if (init_mutex(info))
		exit (1);
	return (check);
}

int main(int argc, char **argv, char **envp)
{
	t_philo *all_philos;
	t_data	info;

	if (!check_and_set(argv, argc, &info))
		wrong_input();
	all_philos = 0;
	generate_philos(&all_philos, &info);
}
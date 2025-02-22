#include "philosopher.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	negative;
	int	rv;

	i = 0;
	negative = 0;
	rv = 0;
	while (str[i] == 32 || (str[i] > 8 && str[i] < 14))
		i++;
	if (str[i] == 45)
	{
		i++;
		negative++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= 48 && str[i] <= 57 && str[i])
	{
		rv = rv * 10 + str[i] - 48;
		i++;
	}
	if (negative % 2 == 1)
		return (rv * -1);
	return (rv);
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
		write (fd, s++, 1);
	write (fd, "\n", 1);
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

size_t	get_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000 + time.tv_usec / 1000);
}

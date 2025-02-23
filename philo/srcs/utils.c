/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpittigl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 14:10:19 by lpittigl          #+#    #+#             */
/*   Updated: 2025/02/22 14:10:19 by lpittigl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (0);
	pthread_mutex_lock(&philo->info->check_death);
	pthread_mutex_lock(&philo->info->printing);
	printf("%lu %i ", curr_time() - philo->info->start_tm, philo->pos + 1);
	printf("%s\n", s);
	pthread_mutex_unlock(&philo->info->printing);
	pthread_mutex_unlock(&philo->info->check_death);
	return (1);
}

int	better_usleep(size_t milliseconds)
{
	size_t	start;

	start = curr_time();
	while ((curr_time() - start) < milliseconds)
		usleep(500);
	return (1);
}

size_t	curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

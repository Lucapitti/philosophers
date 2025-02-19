#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_data {
	size_t			time_to_eat;
	size_t			time_before_death;
	size_t			time_to_sleep;
	int				eating_number;
	int				nbr_of_philo;
	pthread_mutex_t *forks;
	int				some1_died;
	pthread_mutex_t printing;
	pthread_mutex_t check_death;
	size_t			beginnig_time;
}	t_data;

typedef struct s_philo
{
	t_data 			*info;
	pthread_t		thread;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	int				position;
	size_t			last_meal;
	int				curr_sleeping;
	int				curr_eating;
	int				nbr_eat;
	pthread_mutex_t eating;
}	t_philo;

void	*routine(t_philo *philo);

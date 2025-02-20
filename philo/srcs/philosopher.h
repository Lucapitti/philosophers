#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#define THREAD 1
#define PROG 0

typedef struct s_data {
	size_t			time_to_eat;
	size_t			time_before_death;
	size_t			time_to_sleep;
	int				eating_number;
	int				nbr_of_philo;
	pthread_mutex_t *forks;
	int				end_simulation;
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
	pthread_mutex_t eating;
	int				nbr_eat;
	pthread_mutex_t nbr_eaten;
}	t_philo;

void	*routine(void *philosopher);
size_t	get_curr_time();
int		terminate_prog(t_philo *philo, int flag, t_data *info, char *c);
void	ft_putendl_fd(char *s, int fd);
int		ft_atoi(const char *str);
void	generate_philos(t_philo **all_philo, t_data *infos);
int		init_mutex(t_data *info);

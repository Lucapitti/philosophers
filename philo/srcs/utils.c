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

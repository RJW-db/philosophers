/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/02 14:49:10 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:43:47 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

#define BOLD_GREEN "\033[1;32m"
#define BOLD_RED "\033[1;31m"
#define RED "\033[31m"
#define RESET "\033[0m"

//	Global Functions
const char	*get_status(int nbr);
const char	*get_status_coloured(int nbr);
void		print_error(const char *msg);
void		debugger(t_data *data, ssize_t ate_enough);

const char	*get_status(int nbr)
{
	const char	*status[5] = {
	[FORK] = "has taken a fork",
	[EAT] = "is eating",
	[SLEEP] = "is sleeping",
	[THINK] = "is thinking",
	[DEAD] = "died"
	};

	return (status[nbr]);
}

const char	*get_status_coloured(int nbr)
{
	const char	*status[5] = {
	[FORK] = "\033[3;35mhas taken a fork\033[0m",
	[EAT] = "\033[32mis eating\033[0m",
	[SLEEP] = "\033[3;36mis sleeping\033[0m",
	[THINK] = "\033[3;34mis thinking\033[0m",
	[DEAD] = "\033[3;31mdied\033[0m"
	};

	return (status[nbr]);
}

void	print_error(const char *msg)
{
	char	*tmp;

	tmp = (char *)msg;
	while (*tmp != '\0')
	{
		++tmp;
	}
	if (write(STDERR_FILENO, msg, (size_t)(tmp - msg)) == -1)
	{
		
	}
}

void	debugger(t_data *data, ssize_t ate_enough)
{
	pthread_mutex_lock(&data->mtx_data);
	if (data->err_msg == NULL)
	{
		if (ate_enough == data->philo_n)
		{
			data->err_msg = BOLD_GREEN "everyone ate their belly round" RESET;
		}
		else if (ate_enough == 0)
		{
			data->err_msg = BOLD_RED "Everyone died" RESET;
		}
		else
		{
			data->err_msg = RED "Some died" RESET;
		}
	}
	pthread_mutex_unlock(&data->mtx_data);
}

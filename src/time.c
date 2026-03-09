/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 15:08:20 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:43:12 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Global	Functions
ssize_t	get_time(void);
ssize_t	get_runtime(ssize_t start_time);
bool	usleep_interval(t_philo *phil, ssize_t sleep_amount);

ssize_t	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000000L) + current_time.tv_usec);
}

ssize_t	get_runtime(ssize_t start_time)
{
	return ((get_time() - start_time) / 1000);
}

bool	usleep_interval(t_philo *phil, ssize_t sleep_amount)
{
	ssize_t	present;
	ssize_t	goal;
	ssize_t	x;

	x = 100 + phil->data->philo_n * 5;
	present = get_time();
	goal = present + sleep_amount;
	while (goal > present)
	{
		usleep((useconds_t)x);
		present = get_time();
		if (is_phil_alive(phil) == false)
		{
			return (false);
		}
	}
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   activity.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/08 14:42:18 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	grab_forks(t_philo *phil, t_mtx *left_fork, t_mtx *right_fork)
{
	pthread_mutex_lock(left_fork);
	create_msg_node(phil, FORK);
	if (is_phil_alive(phil) == true)
	{
		pthread_mutex_lock(right_fork);
		create_msg_node(phil, FORK);
		return (true);
	}
	pthread_mutex_unlock(left_fork);
	pthread_mutex_lock(&phil->mtx_bound);
	phil->status = DEAD;
	pthread_mutex_unlock(&phil->mtx_bound);
	create_msg_node(phil, DEAD);
	return (false);
}

void	return_forks(t_mtx *left_fork, t_mtx *right_fork)
{
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
}

bool	feast(t_philo *phil)
{
	if (is_phil_alive(phil) == true)
	{
		create_msg_node(phil, EAT);
		pthread_mutex_lock(&phil->mtx_bound);
		--phil->eat_num;
		if (phil->eat_num == 0)
		{
			phil->status = DONE_EATING;
			pthread_mutex_unlock(&phil->mtx_bound);
			return (false);
		}
		phil->last_meal = get_time();
		pthread_mutex_unlock(&phil->mtx_bound);
		if (usleep_interval(phil, phil->t_eat) == false)
		{
			create_msg_node(phil, DEAD);
			return (false);
		}
		return (true);
	}
	return (false);
}

void	snoozing(t_philo *phil)
{
	if (is_phil_alive(phil) == true)
	{
		create_msg_node(phil, SLEEP);
		usleep_interval(phil, phil->t_sleep);
	}
}

void	day_dreaming(t_philo *phil)
{
	if (is_phil_alive(phil) == true)
	{
		create_msg_node(phil, THINK);
		if (phil->t_think > 0)
		{
			usleep_interval(phil, phil->t_think);
		}
	}
}

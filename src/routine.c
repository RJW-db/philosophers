/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/02 18:46:39 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:42:52 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Global Functions
void		*philosopher(t_philo *phil);
bool		is_phil_alive(t_philo *phil);
//	Static Functions
static void	routine(t_philo *phil);

void	*philosopher(t_philo *phil)
{
	pthread_mutex_lock(&phil->data->mtx_sync);
	pthread_mutex_unlock(&phil->data->mtx_sync);
	pthread_mutex_lock(&phil->data->mtx_data);
	if (phil->data->check == false)
	{
		pthread_mutex_unlock(&phil->data->mtx_data);
		return (NULL);
	}
	pthread_mutex_unlock(&phil->data->mtx_data);
	if (phil->data->philo_n == 1)
	{
		pthread_mutex_lock(phil->cutlery.left_fork);
		create_msg_node(phil, FORK);
		usleep_interval(phil, (ssize_t)((float)phil->t_die * 1.1));
		pthread_mutex_unlock(phil->cutlery.left_fork);
		return (NULL);
	}
	if (phil->id % 2 == 0)
	{
		create_msg_node(phil, THINK);
		usleep((useconds_t)((float)phil->t_eat * 0.8));
	}
	routine(phil);
	return (NULL);
}

static void	routine(t_philo *phil)
{
	t_mtx	*left_fork;
	t_mtx	*right_fork;

	left_fork = phil->cutlery.left_fork;
	right_fork = phil->cutlery.right_fork;
	while (is_phil_alive(phil) == true)
	{
		if (grab_forks(phil, left_fork, right_fork) == false)
		{
			return ;
		}
		if (feast(phil) == false)
		{
			return_forks(left_fork, right_fork);
			return ;
		}
		return_forks(left_fork, right_fork);
		snoozing(phil);
		day_dreaming(phil);
	}
}

bool	is_phil_alive(t_philo *phil)
{
	pthread_mutex_lock(&phil->mtx_bound);
	if (phil->status != DEAD)
	{
		if (phil->t_die / TIME >= (get_time() - phil->last_meal) / TIME)
		{
			pthread_mutex_unlock(&phil->mtx_bound);
			return (true);
		}
		if (phil->status == DONE_EATING)
		{
			pthread_mutex_unlock(&phil->mtx_bound);
			return (false);
		}
		phil->status = DEAD;
		pthread_mutex_unlock(&phil->mtx_bound);
		create_msg_node(phil, DEAD);
		return (false);
	}
	pthread_mutex_unlock(&phil->mtx_bound);
	return (false);
}

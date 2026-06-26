/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   supervisor_thread.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/16 17:37:50 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Static Functions
static bool	check_status(t_data *data, t_philo *phil);
static bool	verify_meals(t_data *data, t_philo *phil);
static bool	is_phil_done_eating(t_philo *phil, t_mtx *bound);
static void	slay_all_phils(t_data *data, t_philo *phil);

void	supervisor(t_data *data, bool limit_less)
{
	t_philo	*phil;

	phil = data->philo;
	if (limit_less == false)
	{
		while (check_status(data, phil) == true)
		{
			usleep(500);
		}
	}
	else
	{
		while (check_status(data, phil) == true
			&& verify_meals(data, phil) == false)
		{
			usleep(500);
		}
	}
	pthread_mutex_lock(&data->mtx_data);
	data->check = false;
	pthread_mutex_unlock(&data->mtx_data);
}

static bool	check_status(t_data *data, t_philo *phil)
{
	ssize_t	philo_nbr;
	ssize_t	i;

	philo_nbr = data->philo_n;
	i = 0;
	while (i < philo_nbr)
	{
		if (is_phil_alive(&phil[i]) == false)
		{
			pthread_mutex_lock(&phil[i].mtx_bound);
			if (phil[i].status == DONE_EATING)
			{
				pthread_mutex_unlock(&phil[i].mtx_bound);
				++i;
				continue ;
			}
			pthread_mutex_unlock(&phil[i].mtx_bound);
			slay_all_phils(data, phil);
			return (false);
		}
		++i;
	}
	return (true);
}

static bool	verify_meals(t_data *data, t_philo *phil)
{
	ssize_t	philo_nbr;
	ssize_t	i;

	philo_nbr = data->philo_n;
	i = 0;
	while (i < philo_nbr)
	{
		if (is_phil_done_eating(&phil[i], &phil[i].mtx_bound) == false)
		{
			return (false);
		}
		++i;
	}
	return (true);
}

static bool	is_phil_done_eating(t_philo *phil, t_mtx *bound)
{
	pthread_mutex_lock(bound);
	if (phil->status == DONE_EATING)
	{
		pthread_mutex_unlock(bound);
		return (true);
	}
	pthread_mutex_unlock(bound);
	return (false);
}

static void	slay_all_phils(t_data *data, t_philo *phil)
{
	ssize_t	nbr;

	nbr = 0;
	usleep(100000);
	while (nbr < data->philo_n)
	{
		pthread_mutex_lock(&phil[nbr].mtx_bound);
		phil[nbr].status = DEAD;
		pthread_mutex_unlock(&phil[nbr].mtx_bound);
		++nbr;
	}
	usleep(900000);
}

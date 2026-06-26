/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_pthreads_and_mutexes.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 15:40:27 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Static Functions
static void	set_start_values_for_phils(t_data *data, t_philo *phil, ssize_t i);
static bool	init_loop_through_phils(t_data *data);
static bool	init_fork_or_bound(t_data *data, bool fork_or_bound, ssize_t *i);

bool	pthreads_creation(t_data *data, t_philo *p)
{
	ssize_t	i;

	i = 0;
	pthread_mutex_lock(&data->mtx_sync);
	while (i < data->philo_n)
	{
		if (pthread_create(&p[i].thread, NULL, (t_cast)philosopher, &p[i]) != 0)
		{
			print_error("pthread_create: philosophers\n");
			pthread_mutex_unlock(&data->mtx_sync);
			return (join_threads(data, i), false);
		}
		++i;
	}
	set_start_values_for_phils(data, p, i);
	if (pthread_create(&data->report, NULL, (t_cast)reporter, data) != 0)
	{
		pthread_mutex_unlock(&data->mtx_sync);
		return (join_threads(data, i), false);
	}
	pthread_mutex_unlock(&data->mtx_sync);
	return (true);
}

bool	initialize_mutexes(t_data *data)
{
	if (init_loop_through_phils(data) == false)
	{
		return (false);
	}
	if (pthread_mutex_init(&data->mtx_data, NULL) != 0)
	{
		print_error("pthread_mutex_init: data->mtx_data: Failed\n");
		exile_mutexes(data, STATUS_THREE, data->philo_n);
		return (false);
	}
	if (pthread_mutex_init(&data->mtx_sync, NULL) != 0)
	{
		print_error("pthread_mutex_init: data->mtx_sync: Failed\n");
		exile_mutexes(data, STATUS_FOUR, data->philo_n);
		return (false);
	}
	return (true);
}

static void	set_start_values_for_phils(t_data *data, t_philo *phil, ssize_t i)
{
	data->t_start = get_time();
	while (i > 0)
	{
		--i;
		phil[i].last_meal = data->t_start;
	}
}

static bool	init_loop_through_phils(t_data *data)
{
	ssize_t	i;

	i = 0;
	if (init_fork_or_bound(data, MTX_FORK, &i) == false)
	{
		exile_mutexes(data, STATUS_ONE, i);
		return (false);
	}
	i = 0;
	if (init_fork_or_bound(data, MTX_BOUND, &i) == false)
	{
		exile_mutexes(data, STATUS_TWO, i);
		return (false);
	}
	return (true);
}

static bool	init_fork_or_bound(t_data *data, bool fork_or_bound, ssize_t *i)
{
	char	*str_err;
	t_mtx	*tmp;

	if (fork_or_bound == true)
		str_err = "pthread_mutex_init: data->philo[i].mtx_fork: Failed\n";
	else
		str_err = "pthread_mutex_init: data->philo[i].mtx_bound: Failed\n";
	while (*i < data->philo_n)
	{
		if (fork_or_bound == true)
			tmp = &data->philo[*i].mtx_fork;
		else
			tmp = &data->philo[*i].mtx_bound;
		if (pthread_mutex_init(tmp, NULL) != 0)
		{
			print_error(str_err);
			--(*i);
			return (false);
		}
		++(*i);
	}
	return (true);
}

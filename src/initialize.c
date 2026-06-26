/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initialize.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 16:47:29 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:48:04 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Static Functions
static void	init_philosophers(t_data *data, t_philo *philo, ssize_t philo_n);
static void	distribute_forks(t_philo *philo, ssize_t philo_n);

bool	initialize(t_data *data, const bool argc, const char **argv)
{
	memset(data, 0, sizeof(t_data));
	if (rules(data, argc, argv) == false)
	{
		return (false);
	}
	data->philo = malloc(sizeof(t_philo) * (size_t)data->philo_n);
	if (data->philo == NULL)
	{
		print_error("Error: malloc failed\n");
		return (false);
	}
	init_philosophers(data, data->philo, data->philo_n);
	if (initialize_mutexes(data) == false)
	{
		return (false);
	}
	return (true);
}

static void	init_philosophers(t_data *data, t_philo *philo, ssize_t philo_n)
{
	ssize_t	i;

	i = 0;
	while (philo_n > i)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].status = START_INIT;
		philo[i].t_die = data->t_die;
		philo[i].eat_num = data->eat_num;
		philo[i].t_eat = data->t_eat;
		philo[i].t_sleep = data->t_sleep;
		if (VALGRIND_MARGIN == false)
			philo[i].t_think = (ssize_t)((float)data->t_think * 0.9);
		else
			philo[i].t_think = (ssize_t)((float)data->t_think * 0.4);
		++i;
	}
	distribute_forks(philo, philo_n);
}

static void	distribute_forks(t_philo *philo, ssize_t philo_n)
{
	ssize_t	i;

	i = 0;
	while (i < philo_n)
	{
		if (i == 0)
		{
			philo[i].cutlery.left_fork = &philo[philo_n - 1].mtx_fork;
			philo[i].cutlery.right_fork = &philo[0].mtx_fork;
		}
		else if (i % 2 == 0)
		{
			philo[i].cutlery.left_fork = &philo[i - 1].mtx_fork;
			philo[i].cutlery.right_fork = &philo[i].mtx_fork;
		}
		else
		{
			philo[i].cutlery.left_fork = &philo[i].mtx_fork;
			philo[i].cutlery.right_fork = &philo[i - 1].mtx_fork;
		}
		++i;
	}
}

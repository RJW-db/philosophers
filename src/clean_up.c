/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   clean_up.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 15:14:20 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:39:07 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#define PTHREAD_DESTROY "pthread_mutex_destroy: "
#define SYNCHRONIZE "&data->mtx_sync: Failed\n"
#define DATA "&data->mtx_data: Failed\n"

//	Static Functions
static ssize_t	check_even_nbr(t_data *data, ssize_t phil_id);
static void		exile_all_phils(t_data *data, bool fork_or_bound, ssize_t i);

void	join_threads(t_data *data, ssize_t phil_id)
{
	ssize_t	ate_enough;
	ssize_t	i;
	ssize_t	max_phils;

	max_phils = check_even_nbr(data, phil_id);
	ate_enough = 0;
	i = 0;
	while (i < max_phils)
	{
		pthread_mutex_lock(&data->philo[i].mtx_bound);
		if (data->philo[i].status == DONE_EATING)
			++ate_enough;
		pthread_mutex_unlock(&data->philo[i].mtx_bound);
		if (pthread_join(data->philo[i].thread, NULL) != 0)
		{
			printf("pthread_join: phil id %ld failed.", data->philo[i].id);
		}
		++i;
	}
	if (pthread_join(data->report, NULL) != 0)
	{
		print_error("pthread_join: data->report failed.");
	}
	if (DEBUG == 1)
		debugger(data, ate_enough);
}

void	exile_mutexes(t_data *data, int status, ssize_t philo_index)
{
	ssize_t	tmp;

	if (status >= STATUS_ONE)
	{
		tmp = philo_index;
		if (status == STATUS_TWO)
			tmp = data->philo_n;
		exile_all_phils(data, MTX_FORK, tmp);
	}
	if (status >= STATUS_TWO)
	{
		exile_all_phils(data, MTX_BOUND, philo_index);
	}
	if (status >= STATUS_THREE && pthread_mutex_destroy(&data->mtx_data) != 0)
	{
		print_error(PTHREAD_DESTROY DATA);
	}
	if (status >= STATUS_FOUR && pthread_mutex_destroy(&data->mtx_sync) != 0)
	{
		print_error(PTHREAD_DESTROY SYNCHRONIZE);
	}
}

void	free_leftover_nodes(t_list *lst)
{
	t_list	*tmp;

	while (lst != NULL)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
}

static ssize_t	check_even_nbr(t_data *data, ssize_t phil_id)
{
	ssize_t	max_phils;

	max_phils = data->philo_n;
	if (phil_id < data->philo_n)
	{
		pthread_mutex_lock(&data->mtx_data);
		data->check = false;
		pthread_mutex_unlock(&data->mtx_data);
		max_phils = phil_id;
	}
	return (max_phils);
}

static void	exile_all_phils(t_data *data, bool fork_or_bound, ssize_t i)
{
	char	*str_err;
	t_mtx	*tmp;

	if (fork_or_bound == true)
		str_err = "pthread_mutex_destroy: data->philo[i].mtx_fork: Failed\n";
	else
		str_err = "pthread_mutex_destroy: data->philo[i].mtx_bound: Failed\n";
	while (i != 0)
	{
		if (fork_or_bound == true)
			tmp = &data->philo[i - 1].mtx_fork;
		else
			tmp = &data->philo[i - 1].mtx_bound;
		if (pthread_mutex_destroy(tmp) != 0)
		{
			print_error(str_err);
		}
		--i;
	}
}

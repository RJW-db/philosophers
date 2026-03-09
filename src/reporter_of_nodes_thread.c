/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reporter_of_nodes_thread.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/23 16:07:58 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:44:39 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Global Functions
void		*reporter(t_data *data);
void		create_msg_node(t_philo *phil, t_phase phase);
//	Static Functions
static bool	report_nodes(t_data *data, t_func func_status, const char *marking);
static void	add_node_to_list(t_philo *phil, t_list *node);

void	*reporter(t_data *data)
{
	bool		continuum;
	t_func		status;
	const char	*marking;

	status = get_status;
	marking = "%zu %zu %s\n";
	if (READABLE == true)
	{
		status = get_status_coloured;
		marking = "%8zu %3zu %s\n";
	}
	continuum = true;
	while (continuum == true)
	{
		if (report_nodes(data, status, marking) == false)
		{
			break ;
		}
		usleep((useconds_t)((float)data->philo_n * 1.4));
		usleep((useconds_t)data->philo_n);
		pthread_mutex_lock(&data->mtx_data);
		continuum = data->check;
		pthread_mutex_unlock(&data->mtx_data);
	}
	return (NULL);
}

void	create_msg_node(t_philo *phil, t_phase phase)
{
	t_list			*node;

	node = malloc(sizeof(t_list) * 1);
	if (node == NULL)
	{
		pthread_mutex_lock(&phil->data->mtx_data);
		phil->data->err_msg = "Malloc failed\n";
		phil->data->err = EXIT_FAILURE;
		pthread_mutex_unlock(&phil->data->mtx_data);
		pthread_mutex_lock(&phil->mtx_bound);
		phil->status = DEAD;
		pthread_mutex_unlock(&phil->mtx_bound);
		return ;
	}
	node->phil_id = phil->id;
	node->phase = phase;
	node->next = NULL;
	add_node_to_list(phil, node);
}

static bool	report_nodes(t_data *data, t_func func_status, const char *marking)
{
	static t_phase	last_phase = START_INIT;
	t_list			*current_node;
	t_list			*tmp;

	pthread_mutex_lock(&data->mtx_data);
	current_node = data->lst;
	data->lst = NULL;
	pthread_mutex_unlock(&data->mtx_data);
	while (current_node != NULL)
	{
		if (last_phase == DEAD)
		{
			free_leftover_nodes(current_node);
			return (false);
		}
		tmp = current_node;
		last_phase = tmp->phase;
		current_node = current_node->next;
		printf(marking, \
		get_runtime(data->t_start), tmp->phil_id, func_status((int)last_phase));
		free(tmp);
	}
	return (true);
}

static void	add_node_to_list(t_philo *phil, t_list *node)
{
	static t_list	*last = NULL;

	pthread_mutex_lock(&phil->data->mtx_data);
	if (phil->data->lst != NULL)
	{
		last->next = node;
	}
	else
	{
		phil->data->lst = node;
	}
	last = node;
	pthread_mutex_unlock(&phil->data->mtx_data);
}

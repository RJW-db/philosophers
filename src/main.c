/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 15:00:08 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(const int argc, const char **argv)
{
	t_data	data;

	if (initialize(&data, (argc == 6), argv) == false)
	{
		return (EXIT_FAILURE);
	}
	if (pthreads_creation(&data, data.philo) == true)
	{
		supervisor(&data, (argc == 6));
		join_threads(&data, data.philo_n);
		free_leftover_nodes(data.lst);
	}
	else
	{
		data.err = EXIT_FAILURE;
	}
	exile_mutexes(&data, 4, data.philo_n);
	free(data.philo);
	if (DEBUG == 1)
	{
		printf("\n%s\n", data.err_msg);
	}
	if (data.err == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

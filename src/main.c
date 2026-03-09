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

// int	main(int argc, char **argv)
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

/*
	Certain edgecases
	should not die, test if the 3 groups eat in correct order.
	./philo 199 700 200 1 6
	./philo 199 309 99 9 6
	
	margin should be good enough to survive
	./philo 200 220 100 100 6
	Not mandatory but will it survive
	./philo 200 205 100 100 6
	./philo 777 310 100 100 6
	
	If only supervisor checks status, it might miss the philo died. Should die!
	./philo 200 399 200 200 6

	Should not die
	./philo 3 50 1 1 5

	Should die
	./philo 1 50 1 1 5

	
	Only if you cut-off micro to miliseconds this should be able to survive
	./philo 2 200 100 100 10
	./philo 3 300 100 100 10
*/

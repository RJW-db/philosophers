/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_rules.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/01 20:26:16 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Global Functions
bool			rules(t_data *data, const bool argc, const char **argv);

//	Static Functions
static bool		number_of_philosophers(t_data *data, const char *argv_one);
static ssize_t	return_check(const char *arg);
static ssize_t	reconsidering_life_choices(t_data *data);
static bool		explicit_rules(t_data *data);

bool	rules(t_data *data, const bool argc, const char **argv)
{
	if (number_of_philosophers(data, argv[1]) == false)
	{
		return (false);
	}
	data->t_die = return_check(argv[2]);
	data->t_eat = return_check(argv[3]);
	data->t_sleep = return_check(argv[4]);
	if (data->t_die == -1 || data->t_eat == -1 || data->t_sleep == -1)
	{
		return (false);
	}
	data->t_think = reconsidering_life_choices(data);
	data->check = true;
	if (argc == true)
	{
		data->eat_num = phil_atoui(argv[5]);
		if (data->eat_num == -1)
		{
			return (false);
		}
	}
	if (EXPLICIT_RULES == true)
		return (explicit_rules(data));
	return (true);
}

static bool	number_of_philosophers(t_data *data, const char *argv_one)
{
	data->philo_n = phil_atoui(argv_one);
	if (data->philo_n != -1 && data->philo_n <= 999)
	{
		return (true);
	}
	if (data->philo_n > 999)
	{
		print_error("No more that 999 philosophers\n");
	}
	return (false);
}

static ssize_t	return_check(const char *arg)
{
	static bool	failed = false;
	ssize_t		number;

	if (failed == false)
	{
		number = phil_atoui(arg);
		if (number != -1)
		{
			return (number * 1000);
		}
		failed = true;
	}
	return (-1);
}

static ssize_t	reconsidering_life_choices(t_data *data)
{
	ssize_t	time_for_thoughts;

	time_for_thoughts = 0;
	if (data->philo_n % 2 != 0)
	{
		time_for_thoughts = data->t_die - (data->t_eat + data->t_sleep);
	}
	if (DEBUG == 1)
		printf("pre-time thoughts %ld\n", time_for_thoughts / 1000);
	if (time_for_thoughts < 1000)
	{
		time_for_thoughts = 0;
	}
	else if (VALGRIND_MARGIN == false)
	{
		time_for_thoughts = (ssize_t)((double)time_for_thoughts * 0.9);
	}
	else
	{
		time_for_thoughts = (ssize_t)((double)time_for_thoughts * 0.5);
	}
	if (DEBUG == 1)
		printf("post-time thoughts %ld\n", time_for_thoughts / 1000);
	return (time_for_thoughts);
}

static bool	explicit_rules(t_data *data)
{
	if (data->philo_n <= 200 && data->t_eat >= 60000 && data->t_sleep >= 60000)
	{
		return (true);
	}
	if (data->philo_n > 200)
	{
		print_error("Maximum of 200 philosophers\n");
	}
	else if (data->t_eat < 60000)
	{
		print_error("Minimum of 60ms is required for eating\n");
	}
	else if (data->t_sleep < 60000)
	{
		print_error("Minimum of 60ms is required for sleeping\n");
	}
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   atoi_phil.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/15 21:02:10 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:25:42 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//	Static Functions
static bool		valid_digit_input(const char *num);
static ssize_t	between_one_and_uint_max(ssize_t nbr);

ssize_t	phil_atoui(const char *argv)
{
	ssize_t	nbr;
	ssize_t	overflow_check;

	if (valid_digit_input(argv) == false)
	{
		return (-1);
	}
	nbr = 0;
	while (argv[nbr] == '+')
	{
		++argv;
	}
	overflow_check = 0;
	while (*argv >= '0' && *argv <= '9')
	{
		nbr = nbr * 10 + (*argv++ - '0');
		if (overflow_check > nbr)
		{
			print_error("Atoui overflowed\n");
			return (-1);
		}
		overflow_check = nbr;
	}
	return (between_one_and_uint_max(nbr));
}

static bool	valid_digit_input(const char *nbr)
{
	unsigned short	i;

	i = 0;
	if (nbr == NULL || nbr[0] == '\0')
	{
		print_error("An empty line is not a positive number\n");
		return (false);
	}
	while (nbr[i] == '+')
		++i;
	if (nbr[i] == '-' || (nbr[i] < '0' && nbr[i] > '9'))
	{
		print_error("Only positive numbers allowed 1\n");
		return (false);
	}
	while (nbr[i] >= '0' && nbr[i] <= '9')
		++i;
	if (nbr[i] != '\0')
	{
		print_error("Only positive numbers allowed 2\n");
		return (false);
	}
	else
		return (true);
}

//	__uint_max__ = 4294967295
static ssize_t	between_one_and_uint_max(ssize_t nbr)
{
	ssize_t	__uint_max__;

	__uint_max__ = ((__INT_MAX__ * 2U) + 1U);
	if (nbr != 0 && nbr <= __uint_max__)
	{
		return (nbr);
	}
	if (nbr == 0)
	{
		print_error("Input has to be more than 0\n");
	}
	else
	{
		print_error("No input over UINT_MAX\n");
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rde-brui <rde-brui@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/22 15:09:39 by rde-brui      #+#    #+#                 */
/*   Updated: 2026/03/09 17:37:38 by rde-brui      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/*
** usleep() feature-test compatibility:
** - glibc >= 2.19: _DEFAULT_SOURCE
** - older glibc:   _BSD_SOURCE
** Must be defined before any system header.
** Ensures usleep() is visible on Linux/glibc systems.
** See man 3 usleep and feature_test_macros(7).
*/
# if defined(__linux__) || defined(__GLIBC__)
#  if !defined(_DEFAULT_SOURCE)
#   define _DEFAULT_SOURCE
#  endif
#  if !defined(_BSD_SOURCE)
#   define _BSD_SOURCE
#  endif
# endif

# ifndef DEBUG
#  define DEBUG false
# endif
# ifndef VALGRIND_MARGIN
#  define VALGRIND_MARGIN false
# endif

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <sys/time.h>
# include <limits.h>
# include <pthread.h>

# define MTX_FORK true
# define MTX_BOUND false

typedef void			*(*t_cast)(void *);
typedef const char		*(*t_func)(int);
typedef pthread_mutex_t	t_mtx;
typedef struct s_data	t_data;

typedef enum e_phase
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD,
	DONE_EATING,
	START_INIT
}	t_phase;

typedef enum e_create_destroy
{
	STATUS_ONE,
	STATUS_TWO,
	STATUS_THREE,
	STATUS_FOUR
}	t_create_destroy;

typedef struct s_fork
{
	t_mtx		*left_fork;
	t_mtx		*right_fork;
}	t_fork;

typedef struct s_philosopher
{
	ssize_t		id;
	ssize_t		t_die;
	ssize_t		t_eat;
	ssize_t		t_sleep;
	ssize_t		t_think;
	ssize_t		eat_num;
	ssize_t		last_meal;
	pthread_t	thread;
	t_mtx		mtx_bound;
	t_mtx		mtx_fork;
	t_fork		cutlery;
	t_data		*data;
	t_phase		status;
}	t_philo;

typedef struct s_list
{
	ssize_t			phil_id;
	t_phase			phase;
	struct s_list	*next;
}	t_list;

struct s_data
{
	ssize_t		philo_n;
	ssize_t		t_die;
	ssize_t		t_eat;
	ssize_t		t_sleep;
	ssize_t		t_think;
	ssize_t		t_start;
	ssize_t		eat_num;
	bool		check;
	int			err;
	char		*err_msg;
	t_mtx		mtx_sync;
	t_philo		*philo;
	pthread_t	report;
	t_list		*lst;
	t_mtx		mtx_data;
};

//	initialize.c
bool		initialize(t_data *data, const bool argc, const char **argv);

//	init_rules.c
bool		rules(t_data *data, const bool argc, const char **argv);

//	atoi_phil.c
// int			phil_atoui(const char *argv);
ssize_t		phil_atoui(const char *argv);

//	init_pthreads_and_mutexes.c
bool		pthreads_creation(t_data *data, t_philo *p);
bool		initialize_mutexes(t_data *data);

//	routine.c
void		*philosopher(t_philo *phil);
bool		is_phil_alive(t_philo *phil);

//	activity.c
bool		grab_forks(t_philo *phil, t_mtx *left_fork, t_mtx *right_fork);
void		return_forks(t_mtx *left_fork, t_mtx *right_fork);
bool		feast(t_philo *phil);
void		snoozing(t_philo *phil);
void		day_dreaming(t_philo *phil);

//	time.c
ssize_t		get_time(void);
ssize_t		get_runtime(ssize_t start_time);
bool		usleep_interval(t_philo *phil, ssize_t sleep_amount);

//	print.c
const char	*get_status(int nbr);
const char	*get_status_coloured(int nbr);
void		print_error(const char *msg);
void		debugger(t_data *data, ssize_t ate_enough);

//	reporter_of_nodes_thread.c
void		*reporter(t_data *data);
void		create_msg_node(t_philo *phil, t_phase phase);

//	supervisor_thread.c
void		supervisor(t_data *data, bool limit_less);

//	clean_up.c
void		exile_mutexes(t_data *data, int status, ssize_t philo_index);
void		join_threads(t_data *data, ssize_t i);
void		free_leftover_nodes(t_list *lst);
#endif

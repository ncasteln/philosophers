/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 11:46:28 by ncasteln          #+#    #+#             */
/*   Updated: 2023/09/12 15:02:52 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Divide the total philosophers into 2 groups, group [0] and [1]. The group
	[0] will start. If the total of philo is odd, one philo is "moved" to the
	group which will first usleep().
*/
static int	assign_to_group(int id, int n)
{
	if (n % 2 != 0 && id == n)
		return (1);
	if (id % 2 == 0)
		return (1);
	else
		return (0);
}

static void	*share_forks(t_philo *philo, int *forks, int n_philo)
{
	int	i;

	i = philo->id - 1;
	if (i == 0) // first philo
		philo->fork[0] = forks + (n_philo - 1);
	else
		philo->fork[0] = forks + (i - 1);
	philo->fork[1] = forks + i;
}

static void	*share_mutex(t_philo *philo, pthread_mutex_t *mutex, int n)
{
	int	i;

	i = philo->id - 1;
	if (i == 0) // first philo
		philo->mutex[0] = mutex + (n - 1);
	else
		philo->mutex[0] = mutex + (i - 1);
	philo->mutex[1] = mutex + i; // right mutex
}

static void	parse_philo(int i, t_philo *philo, t_monastery *data)
{
	philo->id = i + 1;
	philo->time = data->time;// just want info, not modify the values -- err prone ?
	share_forks(philo, data->forks, data->n_philo);
	share_mutex(philo, data->mutex, data->n_philo);
	philo->turn = assign_to_group(philo->id, data->n_philo);
}

/* Populate the monastry with correct number of philo */
static t_philo	**create_philo(t_monastery *data, char **argv)
{
	t_philo	**philo;
	int		i;

	philo = ft_calloc(data->n_philo + 1, sizeof(t_philo *)); // set +1 as size???
	if (!philo)
		return (NULL); // remember to freeee()
	i = 0;
	while (i < data->n_philo)
	{
		philo[i] = ft_calloc(1, sizeof(t_philo));
		if (!philo[i])
			return (NULL); // remember to freeee()
		parse_philo(i, philo[i], data);
		i++;
	}
	return (philo);
}

int	parse_monastery(t_monastery *data, char **argv)
{
	int	i;

	// PHILO NUMBER
	data->n_philo = ft_atoi(argv[1]);

	// MUTEXES
	data->mutex = ft_calloc(data->n_philo, sizeof(pthread_mutex_t));
	// if (!data->mutex)
	i = 0;
	while (i < data->n_philo)
	{
		if (pthread_mutex_init(data->mutex + i, NULL))
			return (error(EMUTEX, __FILE__, __LINE__));
		i++;
	}

	// TIMESTAMP
	data->time = ft_calloc(1, sizeof(t_time));
	if (!data->time)
		return (error(EMALLOC, __FILE__, __LINE__));
	data->time->die = ft_atoi(argv[2]) * 1000;
	data->time->eat = ft_atoi(argv[3]) * 1000;
	data->time->sleep = ft_atoi(argv[4]) * 1000;
	if (argv[5])
		data->time->end = ft_atoi(argv[5]); // optional

	// FORKS
	data->forks = ft_calloc(data->n_philo, sizeof(int));
	if (!data->forks)
		return (error(EMALLOC, __FILE__, __LINE__));
	i = 0;
	while (i < data->n_philo)
	{
		data->forks[i] = i; // giving an id to the forks!
		i++;
	}

	// PHILOSOPHERS
	data->philo = create_philo(data, argv);
	if (!data->philo)
		return (error(ECREATE_PHILO, __FILE__, __LINE__));

	// THREADS
	data->th = ft_calloc(data->n_philo + 1, sizeof(pthread_t));
	if (!data->th)
		return (error(EMALLOC, __FILE__, __LINE__));

	return (0);
}
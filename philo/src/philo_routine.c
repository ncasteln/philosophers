/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncasteln <ncasteln@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:36:27 by ncasteln          #+#    #+#             */
/*   Updated: 2023/10/11 13:19:12 by ncasteln         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_think(t_philo *philo)
{
	int64_t	clock_start;

	clock_start = philo->time->clock_start;
	print_tmstmp(philo, THINK, get_rel_time(clock_start));
	accurate_sleep_ms(philo->time->to_think);
}

static void	philo_eat(t_philo *philo)
{
	int64_t	clock_start;

	clock_start = philo->time->clock_start;
	print_tmstmp(philo, EAT, get_rel_time(clock_start));
	pthread_mutex_lock(philo->eat_time_lock);
	philo->last_eat_time = get_rel_time(clock_start);
	pthread_mutex_unlock(philo->eat_time_lock);
	accurate_sleep_ms(philo->time->to_eat);
	pthread_mutex_lock(philo->meals_lock);
	philo->n_meals++;
	pthread_mutex_unlock(philo->meals_lock);
}

static void	philo_sleep(t_philo *philo)
{
	int64_t	clock_start;

	clock_start = philo->time->clock_start;
	print_tmstmp(philo, SLEEP, get_rel_time(clock_start));
	accurate_sleep_ms(philo->time->to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int64_t	clock_start;

	philo = (t_philo *)arg;
	clock_start = philo->time->clock_start;
	if (!philo->starting_group)	// try to remove and start with delay
		philo_sleep(philo);
	while (1)
	{
		if (philo->id % 2 != 0)
		{
			pthread_mutex_lock(philo->r_fork);
			print_tmstmp(philo, FORK, get_rel_time(clock_start));
			pthread_mutex_lock(philo->l_fork);
			print_tmstmp(philo, FORK, get_rel_time(clock_start));
		}
		else
		{
			pthread_mutex_lock(philo->l_fork);
			print_tmstmp(philo, FORK, get_rel_time(clock_start));
			pthread_mutex_lock(philo->r_fork);
			print_tmstmp(philo, FORK, get_rel_time(clock_start));
		}



		philo_eat(philo);
		pthread_mutex_lock(philo->end_lock);
		if (*(philo->end_flag))
		{
			pthread_mutex_unlock(philo->end_lock);
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
			break ;
		}
		pthread_mutex_unlock(philo->end_lock);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);



		philo_sleep(philo);
		pthread_mutex_lock(philo->end_lock);
		if (*(philo->end_flag))
		{
			pthread_mutex_unlock(philo->end_lock);
			break ;
		}
		pthread_mutex_unlock(philo->end_lock);


		philo_think(philo);
	}
	return (NULL);
}

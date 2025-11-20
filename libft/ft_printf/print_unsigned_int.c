/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchatela <kchatela@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:06:52 by kchatela          #+#    #+#             */
/*   Updated: 2025/01/30 15:06:56 by kchatela         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_unsigned_int(unsigned int nb)
{
	unsigned int	count;

	count = 0;
	if (nb > 9)
	{
		count += print_unsigned_int(nb / 10);
		count += print_unsigned_int(nb % 10);
	}
	else
	{
		ft_putchar_fd(nb + 48, 1);
		count++;
	}
	return (count);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 16:31:27 by maboye            #+#    #+#             */
/*   Updated: 2020/08/08 16:31:36 by maboye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool			ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\t'
	|| c == '\n' || c == '\r' || c == '\v');
}
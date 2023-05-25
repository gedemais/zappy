/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 11:45:45 by maboye            #+#    #+#             */
/*   Updated: 2020/08/08 16:25:46 by maboye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool			ft_isalpha(int c)
{
	return (ft_isupper(c) || ft_islower(c));
}

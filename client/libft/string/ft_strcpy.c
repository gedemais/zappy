/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 11:46:26 by maboye            #+#    #+#             */
/*   Updated: 2020/08/10 20:28:25 by maboye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char			*ft_strcpy(char *dst, const char *src)
{
	char	*str;

	str = dst;
	while ((*str++ = *src++) != 0)
		;
	return (dst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lib2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilmira <guilmira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 10:02:57 by guilmira          #+#    #+#             */
/*   Updated: 2021/06/20 08:51:27 by guilmira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if ((char) c == s[i])
			return ((char *) &s[i]);
	}
	if (!c)
		return ((char *) &s[i]);
	return (NULL);
}

char	*ft_strchr_plus(char *str, char *letters)
{
	int i;
	int j;


	i = -1;
	while (str[++i])
	{
		j = -1;
		while (letters[++j])
		{
			if (str[i] == letters[j])
				return (&str[i]);
		}
	}
	if (!letters)
		return (&str[i]);
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*ptr;

	if (!s)
		return (0);
	if ((ft_strlen(s) + 1) < len)
		ptr = (char *) ft_calloc((ft_strlen(s) + 1), sizeof(*ptr));
	else
		ptr = (char *) ft_calloc((len + 1), sizeof(*ptr));
	if (!ptr)
		return (NULL);
	if (start > ft_strlen(s))
	{
		*ptr = 0;
		return (ptr);
	}
	i = 0;
	while (s[i] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_atoi(const char *str)
{
	unsigned int	n;
	unsigned int	i;
	int				sign;

	sign = 1;
	i = 0;
	n = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
	str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = (str[i] - '0') + n * 10;
		if (n > 2147483647 && sign == 1)
			return (-1);
		else if (n > 2147483648 && sign == -1)
			return (0);
		i++;
	}
	return (((int) n * sign));
}

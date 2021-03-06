/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilmira <guilmira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:33:29 by guilmira          #+#    #+#             */
/*   Updated: 2021/09/23 08:37:59 by guilmira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/** STRUCTURE of printf
 * %[parameter][flags][width][.precision][length]type
 * Evaluates the followinf flags:
 * '.' // '#' // '+' // '-' // ' ' // '0' // '*' */
void	get_flags(char *str, t_flag *flag, va_list x)
{
	int	i;

	i = 0;
	if (str[i] == '.')
		return ;
	if (str[i] == '0' && !flag->zerofilled && str[i + 1] != '-')
	{
		flag->zerofilled++;
		if (str[i + 1] == '*')
			flag->zerofilled_total_digits = va_arg(x, int);
		else
			flag->zerofilled_total_digits = \
			get_number_from_string(&(str)[i + 1]);
	}
	if (str[i] == ' ')
		flag->invisible_sign++;
	if (str[i] == '+')
		flag->plus_sign++;
	if (str[i] == '#')
		flag->alternative++;
}

/** PURPOSE : in case that the alignnment is < 0, check and correct. **/
static void	correct_alignment(t_flag *flag)
{
	if (flag->alignment_total_spaces < 0)
	{
		flag->alignment_total_spaces *= -1;
		flag->alignment_sign = '-';
	}
	if (flag->alignment_total_spaces == 0)
		flag->alignment = 0;
}

/** STRUCTURE of printf
 * %[parameter][flags][width][.precision][length]type
 * Width and allignment are synonyms */
void	get_allignment(char *str, t_flag *flag, va_list x)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '.')
			break ;
		if (str[i] == '-')
			flag->alignment_sign = '-';
		if (ft_isdigit(str[i]) && !(flag->alignment))
		{
			flag->alignment++;
			flag->alignment_total_spaces = get_number_from_string(&(str)[i]);
		}
		if (str[i] == '*' && !(flag->alignment))
		{
			flag->alignment++;
			flag->alignment_total_spaces = va_arg(x, int);
		}
		if (flag->alignment_total_spaces < 1)
			correct_alignment(flag);
	}
}

/** STRUCTURE of printf
 * %[parameter][flags][width][.precision][length]type
 * */
void	get_precision(char *str, t_flag *flag, va_list x)
{
	flag->precision++;
	if (ft_isdigit(str[0]))
		flag->precision_total_digits = get_number_from_string(str);
	else if (str[0] == '*')
		flag->precision_total_digits = va_arg(x, int);
	else
		flag->precision = -1;
	if (flag->precision_total_digits < 0)
		flag->precision = 0;
}

/** PURPOSE : gets a string that only contains format values.
 * Format values are contained between % and converter [i. e: d, s, i...] */
char	*get_flag_string(char *str)
{
	if (ft_strchr_plus(str, CONVERTERS))
		return (ft_substr(str, 0, ft_strchr_plus(str, CONVERTERS) - str));
	else
		return (NULL);
}

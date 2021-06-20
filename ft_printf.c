/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilmira <guilmira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/12 08:46:55 by guilmira          #+#    #+#             */
/*   Updated: 2021/06/20 10:22:03 by guilmira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/** PURPOSE : print variable on screen.
 * 1. Takes into account the details of the flag and converter.
 * 2. Prints.
 * */
static void	variable_printer(t_flag *flag, va_list x)
{
	int	lenght;

	lenght = 0;
	if (flag->signal == 'i' || flag->signal == 'd')
		print_integer(va_arg(x, int), flag);
	else if (flag->signal == 'c')
		ft_putchar_fd(va_arg(x, int), 1, flag);
	else if (flag->signal == 's')
		ft_putstr_fd(va_arg(x, char *), 1, flag);
	else if (flag->signal == 'p')
	{
		ft_putstr_fd("0x", 1, flag);
		ft_punteropositivo_fd((unsigned long long) va_arg(x, void *), "0123456789abcdef", 1, flag);
	}
	else if (flag->signal == 'u')
	{
		print_integer(va_arg(x, int), flag);
	}
	else if (flag->signal == 'x')
	{
		ft_punteropositivo_fd((unsigned long long) va_arg(x, void *), "0123456789abcdef", 1, flag);
	}
	else if (flag->signal == 'X')
	{
		ft_punteropositivo_fd((unsigned long long) va_arg(x, void *), "0123456789ABCDEF", 1, flag);
	}
	else if (flag->signal == '%') // || (flag.signal == '%' && previous == '\\')
		ft_putchar_fd('%', 1, flag);
}

void	get_flags(char *str, t_flag *flag)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '.')
			break;
		if (str[i] == '0' && !(ft_isdigit(str[i + 1])))
			flag->zerofilled = get_number_from_string(&(str)[i + 1]);
		if (str[i] == '+')
			;
		if (str[i] == '#')
			;
	}
}

void	get_allignment(char *str, t_flag *flag, va_list x)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '.')
			break;
		if (str[i] == '-')
			flag->alignment_sign = '-';
		if (ft_isdigit(str[i]) && !(flag->zerofilled_total_digits))
		{
			flag->alignment = 1;
			flag->alignment_total_spaces = get_number_from_string(&(str)[i]);
		}
		if (str[i] == '*' && !(flag->alignment))
		{
			flag->alignment = 1;
			flag->alignment_total_spaces = va_arg(x, int);
		}
	}
}

void	get_precision(char *str, t_flag *flag, va_list x)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_isdigit(str[i]) && !(flag->precision))
		{
			flag->precision = 1;
			flag->precision_total_digits = get_number_from_string(&(str)[i]);
		}
		if (str[i] == '*' && !(flag->precision))
			flag->precision_total_digits = va_arg(x, int);
	}
	if (flag->precision_total_digits < 0)
		flag->precision = 0;
}

/** PURPOSE : identifies the signal previous to the converter.
 * 1. Iterative on the flag whle is not converter (i. e: i, d, s, c ...)
 * 2. Registers in flag struct all the details of the flag.
 * */
void	identify_flag(char *str, t_flag *flag, va_list x) //creo que no necceito retorno
{
	int		i;

	i = -1;
	get_flags(str, flag);
	get_star(str, flag);
	get_allignment(str, flag);
	get_precision(str, flag);

	while (check_ifis_converter((*str)[++i]) != 1)
	{
		if ((*str)[i] == '-')
			flag->alignment_sign = '-';
		if (ft_isdigit((*str)[i]) && !(counter_allignment) && (*str)[i] != '0')
		{
			flag->total_alignment_spaces = get_number_from_string(&(*str)[i]);
			counter_allignment++;
		}
		if ((*str)[i] == '*' && !(counter_allignment))
		{
			flag->total_alignment_spaces = va_arg(x, int);
			counter_allignment++;
		}
		if ((*str)[i] == '.')
		{
			if (ft_isdigit((*str)[i + 1]) && !(counter_precision++))
				flag->precision_digits = get_number_from_string(&(*str)[i + 1]);
			else if ((*str)[i + 1] == '*')
			{
				flag->precision_digits = va_arg(x, int);
				counter_allignment++;
			}
			else
				flag->precision_digits = -1;

		}
		if ((*str)[i] == '0')
		{
			flag->zerofilled = get_number_from_string(&(*str)[i + 1]);
			if ((*str)[i + 1] == '*')
				flag->precision_digits = va_arg(x, int);
		}
		if ((*str)[i] == '+')
			flag->plus_sign = 1;
		if ((*str)[i] == ' ')
			flag->invisible_plus_sign = 1;
		aux++;
	}
	flag->signal	= (*str)[i];
	*str = ++aux;
	return (*flag);
}

void advance_string(char **str, char *new_position)
{
	if (new_position)
		*str = new_position;
}

char	*get_flag_string(char *str)
{
	if (ft_strchr_plus(str, CONVERTERS))
		return (ft_substr(str, 0, ft_strchr_plus(str, CONVERTERS) - str));
	else
		return (NULL);
}

/** PURPOSE : to read the string passed as an argument and identify flag.
 * 1. Reads the string.
 * 2. Stops at '%'
 * 3. Applies 'identify_flag'.
 * */
static t_flag	read_mainstring(char **str, t_flag *flag, va_list x)
{
	int		i;
	char *flag_string;

	flag->signal = 0;
	i = -1;
	if (!(*str)[0])
		return (*flag);
	while ((*str)[++i])
	{
		if ((*str)[i] == '%')
			break ;
		else
			ft_putchar_fd((*str)[i], 1, flag);
	}
	if ((*str)[i] == '%' || (*str)[i] == '\\' && ft_strchr_plus((*str)[i + 1], CONVERTERS))
	{
		flag_string = get_flag_string((*str)[i + 1]);
		identify_flag(flag_string, flag, x);
		free (flag_string);
		advance_string(str, (ft_strchr_plus(*str, CONVERTERS) + 1));
	}
	else
		advance_string(str, str + 1);
	return (*flag);
}

/** PURPOSE : ft_printf will recreate the behavior of printf.
 * The funct. recieves a varying number of arguments and varying types. Then prints. Iterative.
 * 1. Read argument.
 * 2. Define the flag.
 * 3. Print the argument.
 * */
int	ft_printf(const char *c, ...)
{
	va_list	x;
	t_flag	flag;
	char	*ptr;

	//flag = (*t_flag) ft_calloc(1, sizeof(*t_flag)); t_flag *flag; guardar en auxiliar, y luego lipiar aux
	init_flag(&flag);
	flag.counter = 0;
	ptr = (char *) c; //ojo, luego vasa modificar el puntero cte c. creo que no importa x ser cte.
	if (!c)
		return (0);
	va_start(x, c);
	while (flag.signal)
	{
		init_flag(&flag);
		flag = read_mainstring(&ptr, &flag, x);
		if (flag.signal)
			variable_printer(&flag, x); //x igual hay que pasarlo por referencia ( si va_arg lo ahces en dos funciones)
	}
	va_end(x);
	//free(flag);
	return (flag.counter);
}

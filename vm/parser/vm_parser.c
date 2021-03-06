/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcollett <gcollett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 20:15:21 by gcollett          #+#    #+#             */
/*   Updated: 2018/04/05 18:22:14 by ygaude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "corewar.h"
#include "libft.h"

static void				intro_champ(t_vm *env)
{
	t_header	*tmp;

	tmp = env->champion;
	if (tmp)
	{
		if (env->nb_player > 1)
			ft_printf("Introducing constestants\n");
		else
			ft_printf("The only and sad contestant is\n");
		while (tmp->next)
		{
			ft_printf("* Player %d, weighing %d bytes", \
			tmp->num, tmp->prog_size);
			ft_printf(", \"%s\" (\"%s\") !\n", tmp->prog_name, tmp->comment);
			tmp = tmp->next;
		}
		ft_printf("* Player %d, weighing %d bytes,", tmp->num, tmp->prog_size);
		ft_printf(" \"%s\" (\"%s\") !\n", tmp->prog_name, tmp->comment);
		ft_printf("Ready? Fight!\n\n");
	}
}

static int				rd(char *line, t_vm *env, unsigned long max, \
unsigned long pos)
{
	unsigned long cmp;

	cmp = -1;
	if (pos + max > MEM_SIZE)
	{
		ft_dprintf(2, "No joke, your champ is bigger than the memory size \
please don't change the header\n");
		exit(-42);
	}
	while (++cmp < max)
		env->memory[pos + cmp] = line[cmp];
	return (0);
}

static unsigned long	check_arg(int ac, char **av, t_vm *env)
{
	unsigned long	nb_champ;
	int				i;

	i = 0;
	nb_champ = 0;
	while (++i < ac)
	{
		if (av[i][0] == '-')
		{
			if (check_opt(i, av, env))
				i++;
		}
		else
			nb_champ++;
	}
	if (nb_champ > MAX_PLAYERS)
		ft_exit("Too many players\n");
	if (nb_champ == 0)
		ft_exit("No player\n");
	return (nb_champ);
}

static char				*get_header(int fd)
{
	long	taille;
	char	*line;
	int		cmp;

	if (fd <= 0)
		ft_exit("File not found\n");
	taille = PROG_NAME_LENGTH + COMMENT_LENGTH + sizeof(int) + sizeof(long);
	line = ft_memalloc(taille + 5);
	if ((cmp = read(fd, line, taille + 4)) < taille)
		ft_exit((cmp <= 0) ? "read error\n" : "file too small\n");
	return (line);
}

void					parsing(int ac, char **av, t_vm *env, int cmp)
{
	long	nm_champ;
	char	*line;
	int		fd;
	int		start;

	start = 0;
	line = ft_memalloc(CHAMP_MAX_SIZE);
	env->nb_player = check_arg(ac, av, env);
	nm_champ = -1;
	if ((ac = env->nb_player + 1))
		while (--ac)
		{
			while (av[++start][0] == '-')
				start += ((opt_have_value(start, av, search_opt(av[start], \
				env), env)) ? 1 : 0);
			fd = open(av[start], O_RDONLY);
			ft_init_header(env, get_header(fd), start, av);
			cmp = read(fd, line, CHAMP_MAX_SIZE);
			rd(line, env, cmp, ++nm_champ * (MEM_SIZE / (env->nb_player)));
			close(fd);
		}
	ft_confirm_num_player(env);
	ft_memdel((void**)&line);
	intro_champ(env);
}

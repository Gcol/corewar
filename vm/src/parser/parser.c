/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcollett <gcollett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 20:15:21 by gcollett          #+#    #+#             */
/*   Updated: 2018/03/02 16:26:46 by vsporer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int				rd(char *line, t_vm *env, unsigned long max, unsigned long pos)
{
	unsigned long cmp;

	cmp = -1;
	while (++cmp < max)
		env->memory[pos + cmp] = line[cmp];
	return (0);
}

static unsigned long	check_arg(int ac, char **av, t_vm *env)
{
	unsigned long	nb_champ;
	int				error;
	int				i;

	i = 0;
	error = 0;
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
		ft_exit("Too many player\n");
	if (nb_champ == 0)
		ft_exit("No player\n");
	return (nb_champ);
}

static char 			*get_header(int fd)
{
	long	taille;
	long	cmp;
	char	*line;


	taille = PROG_NAME_LENGTH + COMMENT_LENGTH + sizeof(int) + sizeof(long);
	line = ft_memalloc(taille + 1);
	if (read(fd, line, taille + 4) < taille)
		ft_exit((fd <= 0) ? "Fichier inacessible\n" : "Fichier trop petit\n");
	cmp = -1;
	return (line);
}

void					parsing(int ac, char **av, t_vm *env, int cmp)
{
	long	nm_champ;
	char			*line;
	int				fd;
	int				start;

	start = 0;
	line = ft_memalloc(CHAMP_MAX_SIZE);
	env->nb_player = check_arg(ac, av, env);
	nm_champ = -1;
	if (ac < 2)
		ft_exit("Usage: ./vm [champion.cor]");
	else if ((ac = env->nb_player + 1))
		while (--ac)
		{
			while (av[++start][0] == '-')
				start += ((opt_have_value(start, av, search_opt(av[start], env), env)) ? 1 : 0);
			fd = open(av[start], O_RDONLY);
			ft_init_header(env, get_header(fd), start, av);
			cmp = read(fd, line, CHAMP_MAX_SIZE);
			rd(line, env, cmp, ++nm_champ * (MEM_SIZE / (env->nb_player)));
			close(fd);
		}
	ft_memdel((void**)&line);
	ft_print_head(env, -1);
}

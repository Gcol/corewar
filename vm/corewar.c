/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsporer <vsporer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/13 18:05:59 by vsporer           #+#    #+#             */
/*   Updated: 2018/04/06 12:03:20 by vsporer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "visu.h"

static void				init(t_vm *env)
{
	env->mem_mov = 1;
	env->nb_process = 0;
	env->cycle = 0;
	env->check = 0;
	env->max_check = MAX_CHECKS;
	env->old_nbr_live = 0;
	env->nbr_live = 0;
	env->c_delta = CYCLE_DELTA;
	env->curr_c_todie = CYCLE_TO_DIE;
	env->c_todie = CYCLE_TO_DIE;
	ft_bzero((void*)env->memory, MEM_SIZE);
	env->nb_player = 0;
	env->killed_process = NULL;
	env->process = NULL;
	env->ll_champ = NULL;
	env->champion = NULL;
	env->opt = get_opt();
	env->option = (t_option*)ft_memalloc_exit(sizeof(t_option));
	env->option->dump = -1;
	env->option->dump_all = -1;
}

static void				init_op_tab(t_op *op_tab)
{
	op_tab[0] = &vm_live;
	op_tab[1] = &vm_ld;
	op_tab[2] = &vm_st;
	op_tab[3] = &vm_add;
	op_tab[4] = &vm_sub;
	op_tab[5] = &vm_and;
	op_tab[6] = &vm_or;
	op_tab[7] = &vm_xor;
	op_tab[8] = &vm_zjmp;
	op_tab[9] = &vm_ldi;
	op_tab[10] = &vm_sti;
	op_tab[11] = &vm_fork;
	op_tab[12] = &vm_lld;
	op_tab[13] = &vm_lldi;
	op_tab[14] = &vm_lfork;
	op_tab[15] = &vm_aff;
}

static void				init_process(t_header *champ, t_vm *env)
{
	unsigned int	pc;
	int				i;

	i = 0;
	pc = 0;
	while (champ)
	{
		champ->nb_live_p = 0;
		if (env->process)
			pc = (MEM_SIZE / env->nb_player) + env->process->pc;
		add_process(&env->process, new_process(env->process, pc, env));
		(env->nb_process)++;
		(champ->nb_process)++;
		env->process->champ = champ;
		env->process->reg[0] = champ->num;
		env->process->inst = env->memory[env->process->pc];
		set_cooldown(env->process, env);
		champ->index_color = i++;
		champ = champ->next;
	}
}

static int				usage(void)
{
	ft_dprintf(2, "Usage: ./corewar [-g | -f -dump N | -dump_all N -v N] \
<[-n N] champion2.cor> <...>\n");
	ft_dprintf(2, "-dump N		: Dumps memory after N cycles then exits\n");
	ft_dprintf(2, "-dump_all N	: Dumps memory and list processes after N \
cycles then exits\n");
	ft_dprintf(2, "-g		: Graphic mode\n");
	ft_dprintf(2, "-f		: Graphic mode windowed\n");
	ft_dprintf(2, "-n N		: Set next champion's ID\n");
	ft_dprintf(2, "-v N		: Verbose mode, N = verbosity level:\n");
	ft_dprintf(2, "				- 1 : Show lives\n");
	ft_dprintf(2, "				- 2 : Show cycles\n");
	ft_dprintf(2, "				- 4 : Show deaths\n");
	ft_dprintf(2, "				- 8 : Show PC movements\n");
	return (21);
}

int						main(int ac, char **av)
{
	t_vm			*env;
	t_op			*op_tab;

	if (ac < 2)
		return (usage());
	op_tab = ft_memalloc_exit(sizeof(t_op) * 16);
	env = ft_memalloc_exit(sizeof(t_vm));
	init(env);
	init_op_tab(op_tab);
	parsing(ac, av, env, 0);
	init_process(env->champion, env);
	if (env->option->visu)
		visu_init(env);
	run_cycle(op_tab, env);
	if (env->ll_champ)
		ft_printf("Player %d(%s) win !\nThe game was finish at cycle %ld.\n", \
		env->ll_champ->num, env->ll_champ->prog_name, env->cycle);
	else
		ft_putendl("\nNobody win, no live");
	if (env->option->visu)
		env->option->visu = visu();
	if (env->option->visu)
		visu_finish(env->ll_champ);
	destroy_env(env, op_tab);
	return (0);
}

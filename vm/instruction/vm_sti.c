/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_sti.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsporer <vsporer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/22 18:33:32 by vsporer           #+#    #+#             */
/*   Updated: 2018/03/23 14:56:12 by ygaude           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static char		**get_sti_perm(void)
{
	char	**perm;

	if (!(perm = (char**)ft_memalloc(sizeof(char*) * 3)))
		ft_exit(strerror(errno));
	if (!(perm[0] = ft_strnew(2)))
		ft_exit(strerror(errno));
	if (!(perm[1] = ft_strnew(2)))
		ft_exit(strerror(errno));
	if (!(perm[2] = ft_strnew(2)))
		ft_exit(strerror(errno));
	perm[0][0] = 1;
	perm[0][1] = 0;
	perm[0][2] = 0;
	perm[1][0] = 1;
	perm[1][1] = 1;
	perm[1][2] = 1;
	perm[2][0] = 1;
	perm[2][1] = 1;
	perm[2][2] = 0;
	return (perm);
}

void			vm_sti(t_process *process, t_vm *env)
{
	static char		**perm = NULL;
	t_param			prm;
	char			peb;

	peb = env->memory[(process->pc + 1) % MEM_SIZE];
	if (!perm)
		perm = get_sti_perm();
	prm.mod = IND_TARG | IMOD;
	prm.len = param_len(peb, 0, 3) + 2;
	if (!check_peb(peb, perm, 3))
	{
		get_param(&prm, process, env);
		if (!check_reg(peb, 3, &prm))
		{
			if (PARAM_TWO(peb) == REG_CODE)
				prm.two = process->reg[prm.two - 1];
			if (PARAM_THR(peb) == REG_CODE)
				prm.thr = process->reg[prm.thr - 1];
			write_memory(process->reg[prm.one - 1], \
			process->pc + ((prm.two + prm.thr) % IDX_MOD), env, process->champ);
		}
	}
	if ((env->option->verbose & SHOW_MOVE))
		show_pc_mov(process->pc, process->pc + prm.len, prm.len, env);
	process->pc += prm.len;
}

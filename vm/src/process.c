/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsporer <vsporer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/17 18:29:25 by vsporer           #+#    #+#             */
/*   Updated: 2018/02/28 15:14:50 by vsporer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_process	*new_process(t_process *src, unsigned int new_pc, t_vm *env)
{
	int		i;
	t_process	*new;

	i = -1;
	if (!(new = (t_process*)malloc(sizeof(t_process))))
		ft_exit(strerror(errno));
	if (src)
	{
		ft_memcpy((void*)new, (void*)src, sizeof(t_process));
		new->pc = new_pc % MEM_SIZE;
	}
	else
	{
		while (++i < REG_NUMBER)
			new->reg[i] = 0;
		new->champ = NULL;
		new->pc = new_pc % MEM_SIZE;
		new->carry = 0;
		new->cooldown = 0;
		new->last_live = 0;
		new->inst = env->memory[new->pc];
		new->next = NULL;
	}
	return (new);
}

void		add_process(t_process **lst, t_process *new)
{
	if (*lst)
	{
		new->next = *lst;
		*lst = new;
	}
	else
		*lst = new;
}

void		del_process(t_process **lst, t_process *todel)
{
	unsigned long	i;
	t_process		*tmp;

	i = 0;
	tmp = *lst;
	if (todel)
	{
		if (*lst == todel)
			*lst = todel->next;
		else
		{
			while (tmp && tmp->next != todel)
				tmp = tmp->next;
			if (tmp)
				tmp->next = todel->next;
		}
		ft_memdel((void**)&todel);
	}
}

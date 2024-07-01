/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/28 21:28:17 by alopez-g          #+#    #+#             */
/*   Updated: 2020/08/09 23:37:09 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_ll.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list *l;
	t_list *aux;

	l = *lst;
	while (l != 0)
	{
		aux = l->next;
		if (del)
			(del)(l->content);
		free(l);
		l = aux;
	}
	*lst = 0;
}

void	ft_lstadd_back(t_list **lst, t_list *new_el)
{
	t_list	*l;

	if (!*lst)
		*lst = new_el;
	else
	{
		l = ft_lstlast(*lst);
		l->next = new_el;
	}
}

void	ft_lstadd_front(t_list **lst, t_list *new_el)
{
	new_el->next = *lst;
	*lst = new_el;
}

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	(del)(lst->content);
	free(lst);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_list_aux;

	if (!lst)
		return (0);
	(void)del;
	new_list = ft_lstnew(f(lst->content));
	new_list_aux = new_list;
	while (lst)
	{
		new_list->next = ft_lstnew(f(lst->content));
		lst = lst->next;
		new_list = new_list->next;
	}
	return (new_list_aux);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*new_el;

	if (!(new_el = (t_list *)malloc(sizeof(t_list))))
		return (0);
	new_el->content = content;
	new_el->next = NULL;
	return (new_el);
}

int	ft_lstsize(t_list *lst)
{
	int	cnt;

	cnt = 0;
	while (lst)
	{
		lst = lst->next;
		cnt++;
	}
	return (cnt);
}

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		(f)(lst->content);
		lst = lst->next;
	}
}

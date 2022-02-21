#include "../../includes/minishell.h"
#include <string.h>


char *ft_destroy_space4(char *str, t_comm *lst)
{
    int i;
    int j;
    int len_dollar;

    len_dollar = 0;
    len_dollar = ft_dol_str(str, lst);
    lst->tmp = (char *)malloc(sizeof(char) * (len_dollar + 2));
    if (!(lst->tmp))
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '\'')
            lst->tmp = ft_one_quotes(str, lst, &i, &j);
        else if (str[i] == '\"')
            lst->tmp = ft_two_quotes(str, lst, &i, &j);
        else if (str[i] == '>' || str[i] == '<')
            lst->tmp = ft_open_file(str, &i, &j, lst);
        else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))   // (ft_isalnum(str[i + 1]) || str[i + 1] == '_')
        {
            i++;
            lst->tmp = ft_shit_dollar(str, lst, &i, &j);
        }
        // else if (str[i] == '$' && str[i + 1] == ' ')
        //     lst->tmp[j++] = str[i++];
        else if (str[i] == ' ')
        {
            while (str[i] && str[i] == ' ')
                i++;
            lst->tmp[j++] = ' ';
        }
        // else if (str[i] == '\\')  
        //     i++;
        else
            lst->tmp[j++] = str[i++];
    }
    lst->tmp[j] = '\0';
	printf("|%s|\n", lst->tmp);
    free (str);
    return (lst->tmp);
}

char *ft_new_str(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '\'')
        {
            i++;
            while (str[i] && str[i] != '\'')
            {
                if (str[i] == ' ')
                    str[i] += 62;
                i++;
            }
        }
        if (str[i] == '\"')
        {
            i++;
            while (str[i] && str[i] != '\"')
            {
                if (str[i] == ' ')
                    str[i] += 62;
                i++;
            }
        }
        i++;
    }
    return (str);
}

t_comm *ft_return_node(t_comm *lst)
{
    t_comm *head;

    head = lst;
    while (lst)
    {
        if (lst->last_str)
        {
            lst->last_str = ft_destroy_space4(lst->last_str, lst);
            lst->cmd = ft_split(lst->last_str, ' ');
            free (lst->last_str);
            lst->cmd = ft_return_space(lst->cmd);
        }
        lst = lst->next;
    }
    lst = head;
    return (lst);
}

t_comm *ft_create_nodes(t_comm *lst, char **str_tl, int count_nd, t_envp *e)
{
    t_comm *tmp;
    int c;

    c = count_nd;
	lst = NULL;
    while (count_nd-- > 0)
    {
        tmp = malloc(sizeof(t_comm));
        if (!tmp)
            return (NULL);
        ft_memset((void *)tmp, 0, sizeof(t_comm));
        tmp->last_str = ft_strdup(str_tl[count_nd]);
        tmp->last_str = ft_new_str(tmp->last_str);
        tmp->count_node = c;
        tmp->infile = -2;
        tmp->outfile = -2;
        tmp->e = e;
        tmp->next = lst;
        lst = tmp;
    }
    ft_no_malloc(str_tl);
    return (lst);
}

t_comm *ft_create_node(t_comm *lst, char *str, int c, t_envp *e)
{
    lst = malloc(sizeof(t_comm));
	if (!lst)
		return (-1);
	ft_memset((void *)lst, 0, sizeof(t_comm));
    lst->last_str = ft_strdup(str);
    lst->last_str = ft_new_str(lst->last_str);
    lst->count_node = c;
    lst->outfile = -2;
    lst->infile = -2;
    lst->e = e;
    lst->next = NULL;
    return (lst);
}

t_comm *ft_parser4(t_comm *lst, char *str, t_envp *e)
{
    t_comm  *head;
    int     count_nd;
    char    **str_tl;
    t_comm  *clean;

    count_nd = ft_count_node(str);
    if (count_nd > 1)
        str_tl = ft_split(str, '|');
    clean = lst;
    if (count_nd > 1)
        lst = ft_create_nodes(lst, str_tl, count_nd, e);
    else
        lst = ft_create_node(lst, str, count_nd, e);
    
    if (clean)
        free(clean);
    free (str);
    lst = ft_return_node(lst);
    return (lst);
}

// "$US"ER$
// $""USER$
// /Users/andreysoldatov/Desktop/wwww/Desktop/school21/2andreysoldatov 67
// andreysoldatov 14
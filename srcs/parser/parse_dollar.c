#include "../../includes/minishell.h"

int ft_iskey(char c)
{
    if (ft_isalnum(c) || c == '_')
        return (1);
    return (0);
}
// если после доллара идет пробел, ЕОФ  - это просто доллар
int ft_func(char *str, int i, t_comm *lst)
{
    int count;
    int z;
    char *tmp;
    char *tmp2;
    t_envp *head;

    lst->fl = 0;
    count = 0;
    z = (i);
    head = lst->e;
    while (str[(i)] && str[i] != '\"')
    {
        if (!(ft_iskey(str[(i)])))
            break ;
        else
            count++;
        i++;
    }
    tmp = ft_substr(str, z, count);
    while (head != NULL)
    {
        if (ft_strncmp(tmp, head->key, ft_strlen(tmp)) == 0)
        {
            lst->fl = 1;
            free (tmp);
            tmp = ft_strdup(head->value);
            lst->fl = ft_strlen(tmp);
            free (tmp);
            return (lst->fl);
        }
        head = head->next;
    }
    lst->fl = ft_strlen(tmp);
    free (tmp);
    return (lst->fl);
}

char *ft_shit_dollar(char *str, t_comm *lst, int *i, int *j)
{
    int z;
    int k;
    int c;
    int l;
    char *tmp2;
    char *tmp;
    t_envp *head;

    lst->fl = 0;
    head = lst->e;
    c = 0;
    z = (*i);
    while (str[(*i)] && str[*i] != '\"' && str[(*i)] != '\'')
    {
        if (!(ft_iskey(str[(*i)])))
            break ;
        else
            c++;
        (*i)++;
    }
    tmp = ft_substr(str, z, c);
    while (head != NULL)
    {
        if (ft_strncmp(tmp, head->key, ft_strlen(tmp)) == 0)
        {
            lst->fl = 1;
            free (tmp);
            tmp = ft_strdup(head->value);
            break;
        }
        head = head->next;
    }
    l = 0;
    if (lst->fl == 1)
    {
        while (tmp[l])
            lst->tmp[(*j)++] = tmp[l++];
    }
    free (tmp);
    return (lst->tmp);
}

int ft_dol_str(char *str, t_comm *lst)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] != '?')
        {
            i++;
            count += ft_func(str, i, lst);
        }
        else if (str[i] == '$' && str[i + 1] == '?')
            count += 3;
        else
            count++;
        i++;
    }
    return (count);
}

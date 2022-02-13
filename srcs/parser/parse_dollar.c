#include "../../includes/minishell.h"

int ft_iskey(char c)
{
    if (ft_isalnum(c) || c == '_')
        return (1);
    return (0);
}

int ft_func(char *str, int i, t_comm *lst)
{
    int count;
    int z;
    char *tmp;
    char *tmp2;
    int fl;
    t_envp *head;

    fl = 0;
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
        if (ft_strncmp(head->key, tmp, ft_strlen(head->key)) == 0)
        {
            fl = 1;
            free (tmp);
            tmp = ft_strdup(head->value);
            fl = ft_strlen(tmp);
            free (tmp);
            return (fl);
        }
        head = head->next;
    }
    fl = ft_strlen(tmp);
    free (tmp);
    return (fl);
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

    int fl = 0;
    head = lst->e;
    c = 0;
    z = (*i);
    while (str[(*i)] && str[*i] != '\"')
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
        if (ft_strncmp(head->key, tmp, ft_strlen(head->key)) == 0)
        {
            fl = 1;
            free (tmp);
            tmp = ft_strdup(head->value);
            break;
        }
        head = head->next;
    }
    l = 0;
    if (fl == 1)
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
        if (str[i] == '$')
        {
            i++;
            count += ft_func(str, i, lst);
        }
        else
            count++;
        i++;
    }
    return (count);
}

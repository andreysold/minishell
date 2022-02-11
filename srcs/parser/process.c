#include "../../includes/minishell.h"
#include <string.h>

char *ft_new_sub(int i, char *name, char *str, int begin)
{
    int k = 0;
    while (begin < i)
    {
        name[k] = str[begin];
        k++;
        begin++;
    }
    name[k] = '\0';
    return (name);
}

void    ft_skip_sp(char *str, int *i, int *begin)
{
    (*i)++;
    while (str[(*i)] == ' ')
        (*i)++;
    (*begin) = (*i);
    while (str[(*i)] && str[(*i)] != ' ')
        (*i)++;
}
char *ft_open_file(char *str, char *tmp, int *i, int *j, t_comm *lst)
{
    char *name;
    int begin;
    int end;

    end = 0;
    begin = 0;
    int k;

    while (str[(*i)])
    {
        k = 0;
        if (str[(*i)] == '>' && str[(*i) + 1] != '>')
        {
            ft_skip_sp(str, i, &begin);
            name = (char *)malloc(sizeof(char) * ((*i) - begin) + 1);
            name = ft_new_sub(*i, name, str, begin);
            lst->outfile = open(name, O_RDONLY | O_WRONLY | O_CREAT, 0644);
            free (name);
        }
        else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
        {
            (*i)++;
            ft_skip_sp(str, i, &begin);
            name = (char *)malloc(sizeof(char) * ((*i) - begin) + 1);
            name = ft_new_sub(*i, name, str, begin);
            lst->outfile = open(name, O_RDONLY | O_WRONLY | O_CREAT | O_APPEND, 0644);
            free (name);
        }
        else if (str[(*i)] == '<' && str[(*i) + 1] != '<')
        {
            ft_skip_sp(str, i, &begin);
            name = (char *)malloc(sizeof(char) * ((*i) - begin) + 1);
            name = ft_new_sub(*i, name, str, begin);
            lst->infile = open(name, O_RDONLY | O_WRONLY | O_CREAT, 0644);
            free (name);
        }
        else
            tmp[(*j)++] = str[(*i)++];
    }
    return (tmp);
}

char *ft_destroy_space4(char *str, char **env, t_comm *lst)
{
    char *tmp;
    int i;
    int j;
    int len_dollar;

    len_dollar = 0;
    len_dollar = ft_dol_str(str, env);
    tmp = (char *)malloc(sizeof(char) * (len_dollar + 2));
    if (!(tmp))
        return (NULL);
    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '\'')
            tmp = ft_one_quotes(str, tmp, &i, &j);
        else if (str[i] == '\"')
            tmp = ft_two_quotes(str, env, tmp, &i, &j);
        else if (str[i] == '>' || str[i] == '<')
            tmp = ft_open_file(str, tmp, &i, &j, lst);
        else if (str[i] == '$' && str[i + 1] != ' ')
        {
            i++;
            tmp = ft_shit_dollar(str, env, tmp, &i, &j);
        }
        else if (str[i] == ' ')
        {
            while (str[i] && str[i] == ' ')
                i++;
            tmp[j++] = ' ';
        }
        else
            tmp[j++] = str[i++];
    }
    tmp[j] = '\0';
    free (str);
    return (tmp);
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

t_comm *ft_parser4(t_comm *lst, char *str, char **env)
{
    t_comm *tmp;
    t_comm *head;
    int count_nd;
    char **str_tl;
    int c;

    count_nd = ft_count_node(str);
    c = count_nd;
    if (c > 1)
        str_tl = ft_split(str, '|');
    if (c > 1)
    {
        lst = NULL;
        while (count_nd-- > 0)
        {
            tmp = malloc(sizeof(t_comm));
            if (!tmp)
                return (NULL);
            tmp->last_str = ft_strdup(str_tl[count_nd]);
            tmp->last_str = ft_new_str(tmp->last_str);
            tmp->count_node = c;
            tmp->infile = -2;
            tmp->outfile = -2;
            tmp->next = lst;
            lst = tmp;
        }
        free (str);
        ft_no_malloc(str_tl);
    }
    else
    {
        lst->last_str = ft_strdup(str);
        lst->last_str = ft_new_str(lst->last_str);
        lst->count_node = c;
		lst->infile = -2;
		lst->outfile = -2;
        lst->next = NULL;
        free (str);
    }
    head = lst;
    while (lst)
    {
        if (lst->last_str)
        {
            lst->last_str = ft_destroy_space4(lst->last_str, env, lst);
            lst->command_str = ft_split(lst->last_str, ' ');
            free (lst->last_str);
            lst->command_str = ft_return_space(lst->command_str);
        }
        lst = lst->next;
    }
    lst = head;
    return (lst);
}

// "$US"ER$
// $""USER$
// /Users/andreysoldatov/Desktop/wwww/Desktop/school21/2andreysoldatov 67
// andreysoldatov 14
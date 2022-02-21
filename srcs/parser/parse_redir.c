#include "../../includes/minishell.h"

char *ft_new_sub(int i, t_comm *lst, char *str, int begin)
{
    int k;
    t_envp *head;
    char *tmp;
    int fl;
    int count;
    int j;
    j = 0;

    fl = 0;
    k = 0;
    count = 0;
    head = lst->e;
    while (begin < i)
    {
        if (str[begin] == '^')
            str[begin] -= 62;
        lst->name[k] = str[begin];
        k++;
        begin++;
    }
    lst->name[k] = '\0';
    k = 0;
    i = 0;
    while (lst->name[i])
    {
        if (lst->name[i] == '$' && lst->name[i + 1] != ' ')
        {
            i++;
            tmp = ft_substr(lst->name, i, ft_strlen(lst->name) - i);
            while (head)
            {
                if (ft_strncmp(head->key, tmp, ft_strlen(head->key)) == 0)
                {
                    free (tmp);
                    free (lst->name);
                    lst->name = ft_strdup(head->value);
                    break ;
                }
                head = head->next;
            }
        }
        i++;
    }
    return (lst->name);
}

void    ft_skip_sp(char *str, int *i, int *begin)
{
    (*i)++;
    while (str[(*i)] == ' ')
        (*i)++;
    (*begin) = (*i);
    while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '>' && str[(*i)] != '<')
        (*i)++;
}

int ft_one_redir(t_comm *lst, char *str, int *i, int *begin)
{
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
    lst->name = ft_new_sub(*i, lst, str, *begin);
    lst->outfile = open(lst->name,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (!lst->outfile)
        return (-1);
    if (lst->outfile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
    }
    free (lst->name);
    return (0);
}

int ft_back_redir(t_comm *lst, char *str, int *i, int *begin)
{
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin)) + 1);
    lst->name = ft_new_sub(*i, lst, str, *begin);
    lst->infile = open(lst->name, O_RDONLY , 0644);
    if (!lst->infile)
        return (-1);
    if (lst->infile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
        
    }
    free (lst->name);
    return (0);
}

int ft_add_redir(t_comm *lst, char *str, int *i, int *begin)
{
    (*i)++;
    ft_skip_sp(str, i, begin);
    lst->name = (char *)malloc(sizeof(char) * ((*i) - (*begin) + 1));
    lst->name = ft_new_sub(*i, lst, str, *begin);
    lst->outfile = open(lst->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (!lst->outfile)
        return (-1);
    if (lst->outfile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
    }
    free (lst->name);
    return (0);
}

int    ft_herdok(t_comm *lst, char *str, int *i, int *begin)
{
    (*i) += 2;
    while (str[(*i)] && str[(*i)] == ' ')
        (*i)++;
    lst->here = ft_strdup(str + (*i));
    (*i) += ft_strlen(lst->here);
    lst->infile = open(".tmp", O_TRUNC | O_WRONLY | O_CREAT, 0644);
    if (lst->infile == -1)
    {
        write(1, "bash: ", 6);
        write(2, lst->name, ft_strlen(lst->name));
        write(1, ":", 1);
        perror("");
    }
    return (0);
}

char *ft_open_file(char *str, int *i, int *j, t_comm *lst)
{
    int begin;
    int end;
    int k;
    
    end = 0;
    begin = 0;
    while (str[(*i)])
    {
        k = 0;
        if (str[(*i)] == '>' && str[(*i) + 1] != '>')
            ft_one_redir(lst, str, i, &begin);
        else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
            ft_add_redir(lst, str, i, &begin);
        else if (str[(*i)] == '<' && str[(*i) + 1] != '<')
            ft_back_redir(lst, str, i, &begin);
        else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
            ft_herdok(lst, str, i, &begin);
        else
            lst->tmp[(*j)++] = str[(*i)++];
    }
    return (lst->tmp);
}
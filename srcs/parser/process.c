#include "../../includes/minishell.h"
#include <string.h>

char *ft_destroy_space4(char *str, char **env)
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
        else if (str[i] == '$' && str[i + 1] != ' ')
        {
            i++;
            tmp = ft_shit_dollar(str, env, tmp, &i, &j);
        }
        else if (str[i] == ' ')
        {
            i++;
            while (str[i] && str[i] == ' ')
                i++;
            tmp[j++] = ' ';
        }
        else
            tmp[j++] = str[i++];
    }
    tmp[j] = '\0';
    return (tmp);
}

t_comm *ft_parser4(t_comm *lst, char *str, char **env)
{
    t_comm *tmp;
    int count_nd;
    char **str_tl;
	int c;

    count_nd = ft_count_node(str);
	c = count_nd;
    str = ft_destroy_space4(str, env);
    printf("|->%s<-|\n", str);
    str_tl = ft_split(str, '|');
    while (count_nd-- > 0)
    {
        tmp = malloc(sizeof(t_comm));
        if (!tmp)
            return (NULL);
        ft_memset((void *)tmp, 0, sizeof(t_comm));
        tmp->last_str = ft_strdup(str_tl[count_nd]);
        tmp->command_str = ft_split(tmp->last_str, ' ');
        tmp->command_str = ft_return_space(tmp->command_str);
		tmp->count_node = c;
        free (tmp->last_str);
        tmp->next = lst;
        lst = tmp;
    }
    ft_no_malloc(str_tl);
    free (str);
    return (lst);
}

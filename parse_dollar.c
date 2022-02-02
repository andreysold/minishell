#include "includes/minishell.h"

int ft_func(char *str, int *i, char **env)
{
    int z;
    int k;
    int count;
    char *tmp;
    char *tmp2;

    z = (*i);
    count = 0;
    while (str[(*i)] && str[(*i)] != ' ')
    {
        (*i)++;
        count++;
    }
    tmp = ft_substr(str, z, count);
    k = -1;
    z = 0;
    while (env[++k])
    {
        if (ft_strnstr(env[k], tmp, ft_strlen(tmp)))
        {
            while (env[k][z] && env[k][z] != '=')
                z++;
            tmp2 = ft_substr(env[k], 0, z);
            if (ft_strncmp(tmp, tmp2, ft_strlen(tmp2)) == 0)
            {
                tmp = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
                free (tmp2);
                k = ft_strlen(tmp);
                free (tmp);
                return (k);
            }
        }
    }
    free (tmp);
    return (count);
}

char *ft_shit_dollar(char *str, char **env, char *m_tmp, int *i, int *j)
{
    int z;
    int k;
    int c;
    int l;
    char *tmp2;
    char *tmp;
    int fl = 0;
    k = 0;
    z = 0;
    c = 0;
    l = 0;
    z = (*i);
    while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != '\"')
    {
        (*i)++;
        c++;
    }
    tmp = ft_substr(str, z, c);
    k = -1;
    z = 0;
    while (env[++k])
    {
        if (ft_strnstr(env[k], tmp, ft_strlen(tmp)))
        {
            while (env[k][z] && env[k][z] != '=')
                z++;
            tmp2 = ft_substr(env[k], 0, z);
            if (ft_strncmp(tmp, tmp2, ft_strlen(tmp2)) == 0)
            {
                free (tmp);
                tmp = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
                free (tmp2);
                fl = 1;
                break ;
            }
        }
    }
    if (fl == 0)
        tmp = ft_strdup("^");  // ?
        // tmp = ft_strjoin("$", tmp);
    while (tmp[l])
        m_tmp[(*j)++] = tmp[l++];
    m_tmp[(*j)] = '\0';
    free (tmp);
    return (m_tmp);
}

int ft_dol_str(char *str, char **env)
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
            if (str[i] != ' ')
            {
                count += ft_func(str, &i, env);
                count++;
            }
        }
        else
            count++;
        i++;
    }
    return (count);
}


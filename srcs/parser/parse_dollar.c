#include "../../includes/minishell.h"

int ft_iskey(char c)
{
    if (ft_isalnum(c) || c == '_')
        return (1);
    return (0);
}

int ft_func(char *str, int i, char **env)
{
    int z;
    int k;
    int count;
    char *tmp;
    char *tmp2;
    int fl = 0;

    count = 0;
    z = (i);
    while (str[(i)] && str[i] != '\"')
    {
        if (!(ft_iskey(str[(i)])))
            break ;
        else
            count++;
        i++;
    }
    tmp = ft_substr(str, z, count);
    k = -1;
    while (env[++k])
    {
        if (ft_strnstr(env[k], tmp, ft_strlen(tmp)))
        {
            z = 0;
            while (env[k][z] && env[k][z] != '=')
                z++;
            tmp2 = ft_substr(env[k], 0, z);
            if (ft_strncmp(tmp, tmp2, ft_strlen(tmp)) == 0)
            {
                fl = 1;
                free (tmp2);
                free (tmp);
                tmp = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
                k = ft_strlen(tmp);
                free (tmp);
                return (k);
            }
        }
    }
    if (fl == 0)
        k = ft_strlen(tmp);
    free (tmp);
    return (k);
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

    c = 0;
    // (*i)--;
    z = (*i);
    while (str[(*i)] && str[*i] != '\"')
    {
        if (!(ft_iskey(str[(*i)])))
            break ;
        else
            c++;
        (*i)++;
    }
    printf("|%s|\n", str);
    tmp = ft_substr(str, z, c);
    printf("%s\n", tmp);
    k = -1;
    while (env[++k])
    {
        if (ft_strnstr(env[k], tmp, ft_strlen(tmp)))
        {
            z = 0;
            while (env[k][z] && env[k][z] != '=')
                z++;
            tmp2 = ft_substr(env[k], 0, z);
            if (ft_strncmp(tmp, tmp2, ft_strlen(tmp2)) == 0)
            {
                fl = 1;
                free (tmp2);
                free (tmp);
                tmp = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
                break ;
            }
            else
                free (tmp2);
        }
    }
    if (fl != 0)
    {
        l = 0;
        while (tmp[l])
            m_tmp[(*j)++] = tmp[l++];
        free (tmp);
    }
    else
        m_tmp = ft_strdup("");
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
            count += ft_func(str, i, env);
        }
        else
            count++;
        i++;
    }
    printf("count = %d\n", count);
    return (count);
}


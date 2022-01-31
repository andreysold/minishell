#include "minishell.h"

// int is_key(char c)
// {
// 	if (c == '_' || ft_isalnum(c))
// 		return (1);
// 	return (0);
// }

// char *ft_dollar4(char *str, int *i, int *j, char **env)
// {
//     int z;
//     int k;
//     char *tmp2;
//     char *tmp3;

//     z = 0;
//     (*i)++;
//     tmp2 = ft_substr(str, (*i), ft_strlen(str) - (*i));
//     k = -1;
//     while (env[++k])
//     {
//         if (strstr(env[k], tmp2))
//         {
//             while (env[k][z] && env[k][z] != '=')
//                 z++;
//             tmp3 = ft_substr(env[k], 0, z);
//             if (strcmp(tmp2, tmp3) == 0)
//                 break ;
//         }
//     }
//     (*i) += ft_strlen(tmp2);
//     free (tmp3);
//     free (tmp2);
//     tmp2 = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
//     //printf("%s\n", tmp2);
//     return (tmp2);
// }


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
    char *tmp2;
    char *tmp;
    int fl = 0;
    k = 0;
    z = 0;
    c = 0;
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
                tmp = ft_substr(env[k], z + 1, ft_strlen(env[k]) - z);
                free (tmp2);
                fl = 1;
                break ;
            }
        }
    }
    if (fl == 0)
        tmp = ft_strjoin("$", tmp);
    int l = 0;
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


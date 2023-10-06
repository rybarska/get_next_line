/*i ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:01:49 by arybarsk          #+#    #+#             */
/*   Updated: 2023/10/06 22:14:00 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char    *read_to_keeper(int fd, char **state_keeper, ssize_t *bytes_read)
{
    char    *buffer;
    char    *temp;

    if (!*state_keeper)
        return (NULL);
    buffer = NULL;
    buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);
    while (!ft_strchr(*state_keeper, '\n') && *bytes_read != 0)
    {
        *bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (*bytes_read < 0)
        {
            free(buffer);
            return (NULL);
        }
        buffer[*bytes_read] = '\0';
        temp = ft_strjoin(*state_keeper, buffer);
        free(*state_keeper);
        *state_keeper = temp;
    }
    free(buffer);
    return (*state_keeper);
}

void	cut_line(char **line, char **state_keeper)
{
	char	*found_newline;
	int		rest_of_line;

	found_newline = NULL;
	found_newline = ft_strchr(*line, '\n');
	if (found_newline)
	{
		rest_of_line = ft_strlen(found_newline + 1);
		ft_memmove(*state_keeper, found_newline + 1, rest_of_line + 1);
		if(!*state_keeper)
			return ;
		found_newline[1] = '\0';
	}
	else
		*line[0] = '\0';
}

char    *get_next_line(int fd)
{
    static char *state_keeper;
    char        *line;
    ssize_t     bytes_read;

    line = NULL;
    bytes_read = 1;
    if (fd < -1 || BUFFER_SIZE <= 0)
        return (NULL);
    if (!state_keeper)
        state_keeper = ft_strdup("");
    read_to_keeper(fd, &state_keeper, &bytes_read);
    if (!state_keeper)
        return (NULL);
    line = ft_strdup(state_keeper);
    if (!line)
        return (NULL);
    cut_line(&line, &state_keeper);
    if (bytes_read <= 0)
    {
        free(line);
        return (NULL);
    }
    return (line);
}
/*
int	main(void)
{
    int fd;
    char *line;

    fd = open("text1.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }
	while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    if (line == NULL)
    {
        perror("Program");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}*/
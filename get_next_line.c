/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:01:49 by arybarsk          #+#    #+#             */
/*   Updated: 2023/10/09 21:58:27 by arybarsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

unsigned int	ft_strlcpy(char *dest, char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	length;

	i = 0;
	length = ft_strlen(src);
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (length);
}

ssize_t	read_to_keeper(int fd, char **state_keeper)
{
	char	*buffer;
	char	*temp;
	ssize_t	readout;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (-1);
	readout = 0;
	while (!ft_strchr(*state_keeper, '\n'))
	{
		readout = read(fd, buffer, BUFFER_SIZE);
		if (readout <= 0)
			break ;
		buffer[readout] = '\0';
		temp = ft_strjoin(*state_keeper, buffer);
		printf("temp: %s\n", temp);
		free(*state_keeper);
		*state_keeper = temp;
	}
	free(buffer);
	printf("readout: %zd\n", readout);
	return (readout);
}

char	*cut_line_from_keeper(char **state_keeper)
{
	char	*line;
	char	*found_newline;
	int		line_len;
	int		rest_of_keeper;

	if (!*state_keeper || ft_strlen(*state_keeper) < 1)
		return (NULL);
	found_newline = ft_strchr(*state_keeper, '\n');
	if (found_newline)
	{
		rest_of_keeper = ft_strlen(found_newline + 1);
		line_len = ft_strlen(*state_keeper) - rest_of_keeper;
		line = (char *)malloc(sizeof(char) * (line_len + 1));
		if (!line)
			return (NULL);
		ft_strlcpy(line, *state_keeper, line_len + 1);
		ft_memmove(*state_keeper, found_newline + 1, rest_of_keeper + 1);
	}
	else
	{
		line = ft_strdup(*state_keeper);
		if (!line)
			return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*state_keeper;
	ssize_t		result;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!state_keeper)
	{
		state_keeper = ft_strdup("");
		if (!state_keeper)
			return (NULL);
	}
	result = read_to_keeper(fd, &state_keeper);
	if (result < 0)
		return (free(state_keeper), state_keeper = NULL, NULL);
	line = cut_line_from_keeper(&state_keeper);
	if (!(ft_strchr(line, '\n')))
	{
		free(state_keeper);
		state_keeper = NULL;
	}
	return (line);
}
/*
int main()
{
    int fd = open("text5.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    char *line;
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("main: %s\n", line);
        free(line);
    }

    if (line == NULL)
    {
        perror("Program");
    }

    close(fd);
    return 0;
}*/
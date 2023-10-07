/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arybarsk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 22:01:49 by arybarsk          #+#    #+#             */
/*   Updated: 2023/10/07 21:35:22 by arybarsk         ###   ########.fr       */
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

	buffer = NULL;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	readout = 1;
	while (!ft_strchr(*state_keeper, '\n') && readout != 0)
	{
		readout = read(fd, buffer, BUFFER_SIZE);
		if (readout < 0)
		{
			free(buffer);
			return (0);
		}
		buffer[readout] = '\0';
		temp = ft_strjoin(*state_keeper, buffer);
		free(*state_keeper);
		*state_keeper = temp;
	}
	free(buffer);
	return (readout);
}

void	cut_line_from_keeper(char **line, char **state_keeper)
{
	char	*found_newline;
	int		line_len;
	int		rest_of_keeper;

	if (!*state_keeper || ft_strlen(*state_keeper) < 1)
		return ;
	found_newline = ft_strchr(*state_keeper, '\n');
	if (found_newline)
	{
		rest_of_keeper = ft_strlen(found_newline + 1);
		line_len = ft_strlen(*state_keeper) - rest_of_keeper;
		*line = (char *)malloc(sizeof(char) * (line_len + 1));
		ft_strlcpy(*line, *state_keeper, line_len + 1);
		ft_memmove(*state_keeper, found_newline + 1, rest_of_keeper + 1);
	}
	else
	{
		line_len = ft_strlen(*state_keeper);
		*line = (char *)malloc(sizeof(char) * (line_len + 1));
		ft_strlcpy(*line, *state_keeper, line_len + 1);
		free(*state_keeper);
		*state_keeper = ft_strdup("");
		if (!*state_keeper)
			return ;
	}
}

char	*get_next_line(int fd)
{
	static char	*state_keeper;
	char		*line;

	line = NULL;
	if (fd < -1 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!state_keeper)
	{
		state_keeper = ft_strdup("");
		if (!state_keeper)
			return (NULL);
	}
	read_to_keeper(fd, &state_keeper);
	if (ft_strlen(state_keeper) < 1)
		return (NULL);
	cut_line_from_keeper(&line, &state_keeper);
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
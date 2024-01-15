#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

int	ft_strlen(const char *str)
{
	int	count = 0;
	
	if (!str)
		return (0);
	while (*str++)
		count++;
	return (count);
}

void	*ft_memmove(void *to, const void *from, size_t n)
{
	char		*t = (char *) to;
	const char	*f = (const char *) from;
	size_t		i = 0;
	
	if (t == f)
		return (to);
	if (t < f || t >= (f + n))
	{
		while (i < n)
		{
			t[i] = f[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i-- > 0)
			t[i] = f[i];
	}
	return (to);
}

char	*ft_strdup(const char *s)
{
	int		s_size;
	char	*pass_string;
	char	*new_string;
	char	*new_string_start;

	pass_string = (char *) s;
	s_size = ft_strlen(pass_string);
	new_string = (char *)malloc(s_size + 1);
	if (new_string == NULL)
		return (NULL);
	new_string_start = new_string;
	while (*s)
		*new_string++ = *s++;
	*new_string = '\0';
	return (new_string_start);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i = 0;
	int		s1_size = ft_strlen((char *) s1);
	int		s2_size = ft_strlen((char *) s2);
	char	*joined_string;

	joined_string = malloc(sizeof(char) * (s1_size + s2_size + 1));
	if (joined_string == NULL)
		return (NULL);
	while (i < s1_size)
	{
		joined_string[i] = s1[i];
		i++;
	}
	while (i < (s1_size + s2_size))
	{
		joined_string[i] = s2[i - s1_size];
		i++;
	}
	joined_string[i] = '\0';
	return (joined_string);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (unsigned char) c)
		s++;
	if (*s == (unsigned char) c)
		return ((char *) s);
	return (NULL);
}

unsigned int	ft_strlcpy(char *dest, char *src, unsigned int size)
{
	unsigned int	i = 0;
	unsigned int	length = ft_strlen(src);
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

ssize_t	read_to_keeper(int fd, char **keeper)
{
	char	*buffer;
	char	*temp;
	ssize_t	readout = 0;
	
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	while (!ft_strchr(*keeper, '\n'))
	{
		readout = read(fd, buffer, BUFFER_SIZE);
		if (readout <= 0)
			break ;
		buffer[readout] = '\0';
		temp = ft_strjoin(*keeper, buffer);
		free(*keeper);
		*keeper = temp;
	}
	free(buffer);
	return (readout);
}

void	cut_line_from_keeper(char **line, char **keeper)
{
	char	*found_newline;
	int		size = ft_strlen(*keeper);
	int		rest_of_keeper;
	int		line_len;
	
	found_newline = ft_strchr(*keeper, '\n');
	if (found_newline)
	{
		rest_of_keeper = ft_strlen(found_newline + 1);
		line_len = size - rest_of_keeper;
		*line = (char *)malloc(sizeof(char) * (line_len) + 1);
		if (!*line)
			return (free(*keeper));
		ft_strlcpy(*line, *keeper, line_len + 1);
		ft_memmove(*keeper, found_newline + 1, rest_of_keeper + 1);
	}
	else
	{
		*line = ft_strdup(*keeper);
		if (!*line)
			return (free(*keeper));
		free(*keeper);
		*keeper = ft_strdup("");
	}
}

char	*get_next_line(int fd)
{
	static char	*keeper;
	char	*line;
	ssize_t	readout;
	
	line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (!keeper)
	{
		keeper = ft_strdup("");
		if (!keeper)
			return (NULL);
	}
	readout = read_to_keeper(fd, &keeper);
	if (readout < 0)
	{
		free(keeper);
		keeper = NULL;
		return (NULL);
	}
	if (ft_strlen(keeper) < 1)
		return (NULL);
	cut_line_from_keeper(&line, &keeper);
	if (!ft_strchr(line, '\n'))
	{
		free(keeper);
		keeper = NULL;
	}
	return (line);
}

int	main(void)
{
	int fd = open("text1.txt", O_RDONLY);
	char	*line;
	while ((line = get_next_line(fd)) != 0)
	{
		printf("%s\n", line);
		free(line);
	}
	close(fd);
	return (0);
}

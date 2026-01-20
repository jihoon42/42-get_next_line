/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 02:46:51 by jkim2             #+#    #+#             */
/*   Updated: 2026/01/21 05:55:34 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	append_buffer(t_stash *stash, char *buffer, size_t n)
{
	size_t	i;

	if (stash_reserve(stash, n))
		return (1);
	i = 0;
	while (i < n)
	{
		stash->content[stash->size + i] = buffer[i];
		i++;
	}
	stash->size += n;
	return (0);
}

static void	stash_trim(t_stash *stash, size_t newline_idx)
{
	size_t	start;
	size_t	i;

	if (newline_idx >= stash->size)
	{
		stash_clear(stash);
		return ;
	}
	start = newline_idx + 1;
	i = 0;
	while (start < stash->size)
	{
		stash->content[i] = stash->content[start];
		i++;
		start++;
	}
	stash->size = i;
	if (stash->size == 0)
		stash_clear(stash);
}

static char	*extract_line(t_stash *stash)
{
	char	*line;
	size_t	idx;
	size_t	len;
	size_t	i;

	idx = newline_index(stash);
	len = stash->size;
	if (idx < stash->size)
		len = idx + 1;
	line = malloc(len + 1);
	if (!line)
		return (stash_clear(stash), NULL);
	i = 0;
	while (i < len)
	{
		line[i] = stash->content[i];
		i++;
	}
	line[len] = '\0';
	stash_trim(stash, idx);
	return (line);
}

static int	fill_stash(int fd, t_stash *stash, char *buffer)
{
	ssize_t	br;
	ssize_t	i;

	br = 1;
	while (br > 0)
	{
		br = read(fd, buffer, BUFFER_SIZE);
		if (br < 0)
			return (-1);
		if (br == 0)
			return (0);
		if (append_buffer(stash, buffer, (size_t)br))
			return (-1);
		i = 0;
		while (i < br)
		{
			if (buffer[i] == '\n')
				return (0);
			i++;
		}
	}
	return (0);
}

char	*get_next_line(int fd)
{
	static t_stash	stash;
	static int		last_fd = -1;
	char			*buffer;
	int				ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (last_fd != fd)
	{
		stash_clear(&stash);
		last_fd = fd;
	}
	buffer = malloc(BUFFER_SIZE);
	if (!buffer)
		return (stash_clear(&stash), NULL);
	if (newline_index(&stash) == stash.size)
	{
		ret = fill_stash(fd, &stash, buffer);
		if (ret < 0)
			return (stash_clear(&stash), free(buffer), NULL);
	}
	free(buffer);
	if (stash.size == 0)
		return (stash_clear(&stash), NULL);
	return (extract_line(&stash));
}

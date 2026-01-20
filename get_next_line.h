/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 02:45:14 by jkim2             #+#    #+#             */
/*   Updated: 2026/01/21 05:55:04 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_stash
{
	char	*content;
	size_t	size;
	size_t	cap;
}	t_stash;

char	*get_next_line(int fd);

size_t	newline_index(t_stash *stash);
void	stash_clear(t_stash *stash);
int		stash_reserve(t_stash *stash, size_t add);

#endif

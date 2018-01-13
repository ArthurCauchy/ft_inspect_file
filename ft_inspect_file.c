/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_inspect_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acauchy <acauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/13 10:36:33 by acauchy           #+#    #+#             */
/*   Updated: 2018/01/13 14:59:37 by acauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "libft.h"

static char		get_filetype(mode_t file_mode)
{
	if (S_ISREG(file_mode))
		return ('-');
	else if (S_ISCHR(file_mode))
		return ('c');
	else if (S_ISBLK(file_mode))
		return ('b');
	else if (S_ISBLK(file_mode))
		return ('b');
	else if (S_ISDIR(file_mode))
		return ('d');
	else if (S_ISFIFO(file_mode))
		return ('p');
	else if (S_ISLNK(file_mode))
		return ('l');
	else if (S_ISSOCK(file_mode))
		return ('s');
	else
		return ('u');
}

static char		*mode_to_str(mode_t file_mode)
{
	char	*str;

	if (!(str = (char*)malloc(11)))
		return (NULL);
	str[0] = get_filetype(file_mode);
	str[1] = file_mode & S_IRUSR ? 'r' : '-';
	str[2] = file_mode & S_IWUSR ? 'w' : '-';
	str[3] = file_mode & S_IXUSR ? 'x' : '-';
	if (file_mode & S_ISUID)
		str[3] = str[3] == 'x' ? 's' : 'S';
	str[4] = file_mode & S_IRGRP ? 'r' : '-';
	str[5] = file_mode & S_IWGRP ? 'w' : '-';
	str[6] = file_mode & S_IXGRP ? 'x' : '-';
	if (file_mode & S_ISGID)
		str[6] = str[6] == 'x' ? 's' : 'S';
	str[7] = file_mode & S_IROTH ? 'r' : '-';
	str[8] = file_mode & S_IWOTH ? 'w' : '-';
	str[9] = file_mode & S_IXOTH ? 'x' : '-';
	if (file_mode & S_ISVTX)
		str[9] = str[9] == 'x' ? 't' : 'T';
	str[10] = '\0';
	return (str);
}

static char		*symlink_target_name(char *link_path, off_t file_size)
{
	size_t	buffer_size;
	char	*target_path;
	ssize_t	read_size;

	buffer_size = (file_size > 0 ? file_size : PATH_MAX) + 1;
	if (!(target_path = (char*)malloc(buffer_size)))
		return (NULL);
	read_size = readlink(link_path, target_path, buffer_size);
	if (read_size < 0)
		return (NULL);
	target_path[read_size] = '\0';
	return (target_path);
}

int				main(int argc, char **argv)
{
	char			*file_path;
	struct stat		file_info;
	dev_t			file_rdev;
	mode_t			file_mode;
	nlink_t			file_nlink;
	uid_t			file_uid;
	gid_t			file_gid;
	off_t			file_size;
	blkcnt_t		file_blocks;
	struct timespec	file_mtimespec;

	if (argc != 2)
	{
		ft_putendl("usage: ./ft_inspect_file <directory>");
		return (0);
	}
	else
	{
		file_path = argv[1];
		if (lstat(argv[1], &file_info) >= 0)
		{
			file_rdev = file_info.st_rdev;
			file_mode = file_info.st_mode;
			file_nlink = file_info.st_nlink;
			file_uid = file_info.st_uid;
			file_gid = file_info.st_gid;
			file_size = file_info.st_size;
			file_blocks = file_info.st_blocks;
			file_mtimespec = file_info.st_mtimespec; // MAC : st_mtimespec | LINUX : st_mtim

			ft_putstr(file_path);
			ft_putendl(":\n");

			if (get_filetype(file_mode) == 'l') // TODO a refaire
			{
				char *symlink_target = symlink_target_name(file_path, file_size); // TODO check not null
				ft_putstr("symlink target : ");
				ft_putendl(symlink_target);
			}
			
			ft_putstr("mode : ");
			ft_putendl(mode_to_str(file_mode));

			ft_putstr("nlink : ");
			ft_putnbr(file_nlink);
			ft_putchar('\n');
			
			ft_putstr("uid : ");
			ft_putnbr(file_uid);
			ft_putchar('\n');

			ft_putstr("owner name : ");
			ft_putstr(getpwuid(file_uid) ? getpwuid(file_uid)->pw_name : "(null)");
			ft_putchar('\n');

			ft_putstr("gid : ");
			ft_putnbr(file_gid);
			ft_putchar('\n');

			ft_putstr("group name : ");
			ft_putstr(getgrgid(file_gid) ? getgrgid(file_gid)->gr_name : "(null)");
			ft_putchar('\n');
		
			ft_putstr("size : ");
			ft_putnbr(file_size);
			ft_putchar('\n');

			ft_putstr("rdev major, minor : "); // a faire a la place de size si filetype = c ou b
			ft_putnbr(major(file_rdev));
			ft_putstr(", ");
			ft_putnbr(minor(file_rdev));
			ft_putchar('\n');

			ft_putstr("blocks : ");
			ft_putnbr(file_blocks);
			ft_putchar('\n');

			ft_putstr("mtimespec : ");
			ft_putnbr(file_mtimespec.tv_sec);
			ft_putchar('\n');

			ft_putstr("mtime converted : ");
			ft_putstr(ctime(&file_mtimespec.tv_sec));
		}
		else
			perror(ft_strjoin(ft_strjoin(argv[0],": "), argv[1]));
	}
	return (0);
}

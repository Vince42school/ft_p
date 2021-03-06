/////////// SERVER, RECOIT /////////

#include "server.h"

char	*get_path(char *str)
{
	char	*ret;

	if (ft_strrchr(str, '/'))
	{
		ret = ft_strrchr(str, '/');
		ret++;
	}
	else
		ret = str;
	return (ret);
}

int		put_choose_stuff(char *path, int fd, int socket)
{
	char	buff[256];
	int		ret;

	send(socket, "Q", 1, 0);
	ret = recv(socket, buff, 255, 0);
	buff[ret] = '\0';
	if (ft_toupper(buff[0]) == 'R')
	{
		ret = recv(socket, buff, 255, 0);
		buff[ret] = '\0';
		close(fd);
		return (put_create_file(buff, socket));
	}
	else if (ft_toupper(buff[0]) == 'O')
	{
		close(fd);
		fd = open(path, O_TRUNC | O_WRONLY);
		return (fd);
	}
	else
	{
		send(socket, "Put stopped\n", 12, 0);
		close(fd);
		return (-1);
	}
}

int		put_create_file(char *path, int socket)
{
	char	*good_path;
	int		fd;

	good_path = get_path(path);
	ft_putendl(good_path);
	if ((fd = open(good_path, O_RDONLY)) != -1)
		return (put_choose_stuff(good_path, fd, socket));
	else
	{
		fd = open(good_path, O_CREAT | O_WRONLY);
		return (fd);
	}
}

static void	read_file_put(int socket, int fd, int size)
{
	char	buff[1024];
	int		ret;

	while (size > 0)
	{
		ret = recv(socket, buff, 1023, 0);
		buff[ret] = '\0';
		write(fd, buff, ret);
		size -= ret;
		send(socket, "", 1, 0);
	}
}

int			ask_put(char **path, int socket)
{
	int		ret;
	char	buff[1024];
	int		fd;
	int		size;
	char	*good_path;

	send(socket, "", 1, 0);
	ret = recv(socket, buff, 1023, 0);
	buff[ret] = '\0';
	if (ft_strncmp(buff, "ER1", 3) == 0)
		return (server_report(1, socket));
	else if (ft_strncmp(buff, "ER2", 3) == 0)
		return (server_report(5, socket));
	size = ft_atoi(buff);
	good_path = path[1];
	fd = put_create_file(good_path, socket);
	if (fd != -1)
	{
		send(socket, "K", 1, 0);
		read_file_put(socket, fd, size);
		close(fd);
	}
	return (0);
}

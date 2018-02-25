/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/07 18:50:22 by omiroshn          #+#    #+#             */
/*   Updated: 2018/02/07 18:50:24 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	draw(t_wolf *w)
{
	int x;

	x = w->start - 1;
	while (++x < w->end)
	{
		draw_camera(&w->map, x);
		perform_dda(&w->map);
		draw_wall(&w->map, x, w->map.karta.data);
		draw_floor(&w->map, w->map.karta.data);
		draw_cursor(&w->map);
	}
}

void	threads_create(SDL_Surface *screen, t_map map)
{
	SDL_Thread	*threads[THREADS];
	t_wolf		data[THREADS];
	double		coef;
	int			i;

	ft_bzero(screen->pixels, WIDTH * HEIGHT * 4);
	coef = WIDTH / THREADS;
	i = -1;
	while (++i < THREADS)
	{
		data[i] = (t_wolf){map, (int)i * coef, (int)(i + 1) * coef};
		threads[i] = SDL_CreateThread((int(*)())draw, NULL, &data[i]);
	}
	i = -1;
	while (++i < THREADS)
		SDL_WaitThread(threads[i], NULL);
}

void	put_error(const char *msg)
{
	if (!msg)
		exit(-1);
	printf("Error: %s\n", msg);
	exit(-1);
}

void	put_usage(void)
{
	ft_putendl("Usage: ./wolf3d resources/maps/map1.map");
	exit(-1);
}

void	load_textures_wolf(t_map *m)
{
	if (!(m->w_t[0] = IMG_Load("./resources/images/eagle.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[1] = IMG_Load("./resources/images/redbrick.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[2] = IMG_Load("./resources/images/purplestone.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[3] = IMG_Load("./resources/images/greystone.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[4] = IMG_Load("./resources/images/colorstone.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[5] = IMG_Load("./resources/images/mossy.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[6] = IMG_Load("./resources/images/wood.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[7] = IMG_Load("./resources/images/hitler.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[8] = IMG_Load("./resources/images/torch.png")))
		put_error(IMG_GetError());
}

void	load_textures_anime(t_map *m)
{
	if (!(m->w_t[9] = IMG_Load("./resources/images/sand.jpg")))
		put_error(IMG_GetError());
	if (!(m->w_t[10] = IMG_Load("./resources/images/uganda.jpg")))
		put_error(IMG_GetError());
	if (!(m->w_t[11] = IMG_Load("./resources/images/akatski.jpg")))
		put_error(IMG_GetError());
	if (!(m->w_t[12] = IMG_Load("./resources/images/anime.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[13] = IMG_Load("./resources/images/pain.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[14] = IMG_Load("./resources/images/kakashi.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[15] = IMG_Load("./resources/images/narutoramen.jpg")))
		put_error(IMG_GetError());
	if (!(m->w_t[16] = IMG_Load("./resources/images/narutopixel.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[17] = IMG_Load("./resources/images/ugandaflag.png")))
		put_error(IMG_GetError());
	if (!(m->w_t[18] = IMG_Load("./resources/images/eye.png")))
		put_error(IMG_GetError());
}

void	lsync(void)
{
	int					delay;
	static unsigned int	time;
	static char			vsync = 1;

	vsync == 1 ? SDL_GL_SetSwapInterval(1) : 0;
	if (vsync)
	{
		delay = 17 - (SDL_GetTicks() - time);
		if (delay < 0)
			SDL_Delay(0);
		else
			SDL_Delay(delay);
		time = SDL_GetTicks();
	}
}

void	display_fps(SDL_Surface *surface)
{
	static unsigned int	frame_rate;
	static unsigned int	time_current;
	static unsigned int	time_past;
	static unsigned int	ticks;

	time_current = time(NULL);
	if (time_current - time_past && (time_past = time_current))
	{
		frame_rate = SDL_GetTicks() - ticks;
		printf("FPS: %d\n", (int)(1000.0 / frame_rate));
	}
	ticks = SDL_GetTicks();
}

void	init(t_map *m)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m->window = SDL_CreateWindow("Wolf3d",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	if (!m->window)
		exit(1);
	m->screen = SDL_GetWindowSurface(m->window);
	m->image = m->screen->pixels;
	m->dir.x = -1;
	m->dir.y = 0;
	m->plane.x = 0;
	m->plane.y = 0.66;
	m->move = 0;
	m->rotate = 0;
	m->mov_speed = 0.08;
	m->rot_speed = 0.05;
}

int		ft_countwords(char *str, char c)
{
	int i;
	int result;

	i = 0;
	result = 0;
	while (str[i] && str[i] == c)
		i++;
	while (str[i])
	{
		while (str[i] && str[i] != c)
			i++;
		result++;
		while (str[i] && str[i] == c)
			i++;
	}
	return (result);
}

void	alloc_map(t_map *m, int fd, char *lol, char **numbers)
{
	int		i;
	int		j;

	m->karta.data = ft_memalloc(sizeof(int *) * m->karta.rows);
	i = -1;
	while (++i < m->karta.cols)
		m->karta.data[i] = ft_memalloc(sizeof(int) * m->karta.cols);
	i = -1;
	while (get_next_line(fd, &lol) > 0 && ++i < m->karta.rows && (j = -1))
	{
		if (ft_countwords(lol, ' ') != m->karta.cols)
			put_error("opening file.");
		numbers = ft_strsplit(lol, ' ');
		while (++j < m->karta.cols)
			ft_atoi_base(numbers[j], 16) >= 0
					? m->karta.data[i][j] = ft_atoi_base(numbers[j], 16)
					: put_error("map invalid.");
		free(lol);
	}
	close(fd);
	// if (m->karta.data[(int)m->pos.x][(int)m->pos.y] != 0)
	// 	put_error("invalid player position.");
}

void	check_textures(t_map *m, t_uint **data)
{
	int		i;
	int		j;

	i = -1;
	while (++i < m->karta.rows && (j = -1))
		while (++j < m->karta.cols)
			if (IS_WALL(data[i][j]) || i == 0 || j == 0
				|| i == m->karta.rows - 1 || j == m->karta.cols - 1)
			{
				!(data[i][j] & 0xFL) ? data[i][j] |= 0x4L : 0;
				!(data[i][j] & 0xF0L) ? data[i][j] |= 0x40L : 0;
				!(data[i][j] & 0xF00L) ? data[i][j] |= 0x400L : 0;
				!(data[i][j] & 0xF000L) ? data[i][j] |= 0x4000L : 0;
			}
			else
			{
				!(data[i][j] & 0xF0000L) ? data[i][j] |= 0x30000L : 0;
				!(data[i][j] & 0xF00000L) ? data[i][j] |= 0x300000L : 0;
			}
}

void	read_map(t_map *m, char *filename)
{
	int		i;
	int		j;
	int		fd;
	char	*lol;
	char	**numbers;

	lol = NULL;
	if ((fd = open(filename, O_RDONLY)) < 0 || read(fd, NULL, 0) < 0)
		put_error("opening file.");
	if (get_next_line(fd, &lol) != 1 || ft_countwords(lol, ' ') != 2)
		put_error("map invalid.");
	numbers = ft_strsplit(lol, ' ');
	m->karta.rows = ft_atoi(numbers[0]);
	m->karta.cols = ft_atoi(numbers[1]);
	if (get_next_line(fd, &lol) != 1 || ft_countwords(lol, ' ') != 2)
		put_error("map invalid.");
	numbers = ft_strsplit(lol, ' ');
	m->pos.x = ft_atoi(numbers[1]) - 0.5;
	m->pos.y = ft_atoi(numbers[0]) - 0.5;
	if (m->pos.x <= 1 || m->pos.x > m->karta.rows - 1
		|| m->pos.y <= 1 || m->pos.y > m->karta.cols - 1)
		put_error("invalid player position.");
	alloc_map(m, fd, lol, numbers);
	check_textures(m, m->karta.data);
}

int		main(int argc, char **argv)
{
	t_map		map;
	static int	running = 1;

	if (argc != 2)
		put_usage();
	read_map(&map, argv[1]);
	init(&map);
	load_textures_wolf(&map);
	load_textures_anime(&map);
	while (running)
	{
		if (!key_function(&map))
			break ;
		map.move ? scan_ws(&map, map.move * map.mov_speed) : 0;
		map.rotate ? scan_ad(&map, map.rotate * map.rot_speed) : 0;
		threads_create(map.screen, map);
		// display_fps(map.screen);
		lsync();
		SDL_UpdateWindowSurface(map.window);
	}
	SDL_DestroyWindow(map.window);
	SDL_Quit();
	return (0);
}

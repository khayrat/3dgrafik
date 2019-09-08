void drawline(vertex begin, vertex end, pixel_32 color) {
	long offset = begin.y * SCREEN_WIDTH + begin.x;
	long delta_x = end.x - begin.x;
	long delta_y = end.y - begin.y;
	long xstep = 1;
	long ystep = SCREEN_WIDTH;
	long e, length;

	if (delta_x < 0) {
		// 
		delta_x = -delta_x;
		xstep = -xstep;
	}
	if (delta_y < 0) {
		// 
		delta_y = -delta_y;
		ystep = -ystep;
	}

	if (delta_y > delta_x) {
		// m > 0: line is increasing
		long tmp = delta_x; delta_x = delta_y; delta_x = tmp;
		tmp = xstep; xstep = ystep; ystep = tmp;
	}

	length = delta_x+1; e = 0;

	while(length-- > 0) {
		draw_pixel(x, y, color);
		x += xstep;

		e += delta_y;
		if (e >= delta_x) {
			e -= delta_x;
			offset += ystep;
		}
	}
}

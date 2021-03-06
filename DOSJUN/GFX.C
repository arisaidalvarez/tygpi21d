/* I N C L U D E S /////////////////////////////////////////////////////// */

#include "gamelib.h"
#include "dosjun.h"

/* F U N C T I O N S ///////////////////////////////////////////////////// */

void Draw_Line_DB(int xo, int yo, int x1, int y1, colour col)
{
	/* This function uses Bresenham's algorithm (IBM 1965) to draw a line
	from (xo,yo) - (x1,y1). */

	int dx,					/* x difference */
		dy,					/* y difference */
		x_inc,				/* x increment */
		y_inc,				/* y increment */
		error = 0,			/* discriminant */
		index;

	/* Access the video buffer directly for speed. */
	unsigned char far *vb_start = double_buffer;

	/* Precompute the first pixel address in the video buffer. Use shifts for
	multiplication. */
	vb_start += ((unsigned int)yo << 6) + ((unsigned int)yo << 8) +
		(unsigned int)xo;

	/* Compute the deltas. */
	dx = x1 - xo;
	dy = y1 - yo;

	/* Test which direction the line is going in; i.e. the slope angle. */
	if (dx >= 0) {
		x_inc = 1;
	} else {
		x_inc = -1;
		dx = -dx;
	}

	/* Test the y component of the slope. */
	if (dy >= 0) {
		y_inc = SCREEN_WIDTH;
	} else {
		y_inc = -SCREEN_WIDTH;
		dy = -dy;
	}

	/* Now, based on which delta is greater, we can draw the line. */
	if (dx > dy) {
		/* Draw the line. */
		for (index = 0; index <= dx; index++) {
			/* Set the pixel */
			*vb_start = col;

			/* Adjust the disciminant. */
			error += dy;

			/* Test if error overflowed. */
			if (error > dx) {
				error -= dx;

				/* Move to the next line. */
				vb_start += y_inc;
			}

			vb_start += x_inc;
		}
	} else {
		for (index = 0; index <= dy; index++) {
			*vb_start = col;

			error += dx;

			if (error > dy) {
				error -= dy;

				vb_start += x_inc;
			}

			vb_start += y_inc;
		}
	}
}

void Draw_Diagonal_DB(int xo, int yo, int x1, int y1, colour col)
{
	int x, y, xd, yd;

	x = xo;
	y = yo;
	if (x1 < xo) xd = -1;
	else if (x1 > xo) xd = 1;
	if (y1 < yo) yd = -1;
	else if (y1 > yo) yd = 1;

	while (x != x1 && y != y1) {
		Plot_Pixel_Fast_DB(x, y, col);

		x += xd;
		y += yd;
	}
}

void Draw_Bounded_String(int x, int y, int w, int h, colour col, char *string, bool trans_flag)
{
	int row = 0,
		column = 0,
		i = 0;
	char ch;

	if (string == null) return;

	while (string[i]) {
		if (row >= h) return;

		ch = string[i++];
		if (ch == ' ' && column == 0) continue;
		if (ch == '\n') {
			column = 0;
			row++;
			continue;
		}

		Blit_Char_DB(x + column*8, y + row*8, ch, col, trans_flag);

		column++;
		if (column >= w) {
			column = 0;
			row++;
		}
	}
}

void Draw_HorzTrapezium_DB(colour col, int x0, int x1, int x2, int x3, int y0, int y1, bool filled)
{
	int xa = x0,
		xb = x1,
		y = y0,
		ye = y1,
		m;

	if (filled) {
		if (y1 < y0) {
			m = -1;
		} else {
			m = 1;
		}

		while (y != ye) {
			Draw_Line_DB(xa, y, xb, y, col);

			xa++;
			xb--;
			y += m;
		}
	} else {
		Draw_Line_DB(x0, y0, x1, y0, col);
		Draw_Diagonal_DB(x0, y0, x2, y1, col);
		Draw_Diagonal_DB(x1, y0, x3, y1, col);
		Draw_Line_DB(x2, y1, x3, y1, col);
	}
}

void Draw_VertTrapezium_DB(colour col, int x0, int x1, int y0, int y1, int y2, int y3, bool filled)
{
	int x = x0,
		xe = x1,
		ya = y0,
		yb = y1,
		m;

	if (filled) {
		if (x1 < x0) {
			m = -1;
		} else {
			m = 1;
		}

		while (x != xe) {
			Draw_Line_DB(x, ya, x, yb, col);

			ya++;
			yb--;
			x += m;
		}
	} else {
		Draw_Line_DB(x0, y0, x0, y1, col);
		Draw_Diagonal_DB(x0, y0, x1, y2, col);
		Draw_Diagonal_DB(x0, y1, x1, y3, col);
		Draw_Line_DB(x1, y2 - 1, x1, y3 + 1, col);
	}
}

void Draw_Square_DB(colour col, int x0, int y0, int x1, int y1, bool filled)
{
	int y;

	if (x0 < 0 || x0 >= SCREEN_WIDTH)  dief("Draw_Square_DB: x0 not in screen (%d)", x0);
	if (y0 < 0 || y0 >= SCREEN_HEIGHT) dief("Draw_Square_DB: y0 not in screen (%d)", y0);
	if (x1 < 0 || x1 >= SCREEN_WIDTH)  dief("Draw_Square_DB: x1 not in screen (%d)", x1);
	if (y1 < 0 || y1 >= SCREEN_HEIGHT) dief("Draw_Square_DB: y1 not in screen (%d)", y1);

	if (x1 < x0) dief("Draw_Square_DB: x1 < x0 (%d < %d)", x1, x0);
	if (y1 < y0) dief("Draw_Square_DB: y1 < y0 (%d < %d)", y1, y0);

	if (filled) {
		for (y = y0; y <= y1; y++) {
			Draw_Line_DB(x0, y, x1, y, col);
		}
	} else {
		Draw_Line_DB(x0, y0, x1, y0, col);
		Draw_Line_DB(x0, y1, x1, y1, col);
		Draw_Line_DB(x0, y0, x0, y1, col);
		Draw_Line_DB(x1, y0, x1, y1, col);
	}
}

bool Read_Palette(FILE *fp, palette *p)
{
	fread(p->colours, sizeof(RGB_color), PALETTE_SIZE, fp);

	return true;
}

void Apply_Palette(palette *p)
{
	int i;

	for (i = 0; i < PALETTE_SIZE; i++) {
		Set_Palette_Register(i, &p->colours[i]);
	}
}

noexport RGB_color_ptr Mix(RGB_color_ptr a, RGB_color_ptr b, int percentage)
{
	RGB_color mixed;
	double ratio = percentage / 100.0;
	double oppos = 1 - ratio;

	mixed.red = a->red * oppos + b->red * ratio;
	mixed.blue = a->blue * oppos + b->blue * ratio;
	mixed.green = a->green * oppos + b->green * ratio;

	return &mixed;
}

void Fade_To(palette *p, RGB_color_ptr c, int delay)
{
	int percentage, i;

	for (percentage = 1; percentage <= 100; percentage++) {
		for (i = 0; i < PALETTE_SIZE; i++) {
			Set_Palette_Register(i, Mix(&p->colours[i], c, percentage));
		}

		Delay(delay);
	}
}

void Fade_From(palette *p, RGB_color_ptr c, int delay)
{
	int percentage, i;

	for (percentage = 1; percentage <= 100; percentage++) {
		for (i = 0; i < PALETTE_SIZE; i++) {
			Set_Palette_Register(i, Mix(c, &p->colours[i], percentage));
		}

		Delay(delay);
	}
}

void Fill_Palette(RGB_color_ptr c)
{
	int i;

	for (i = 0; i < PALETTE_SIZE; i++) {
		Set_Palette_Register(i, c);
	}
}

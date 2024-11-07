#include <stdbool.h>	/* true, false */
#include <stddef.h>
#include <stdio.h>		/* printf() */
#include <string.h>		/* strlen() */
#include <sys/ioctl.h>	/* ioctl() */
#include <termios.h>	/* tcgetattr(), tcsetattr() */
#include <unistd.h>		/* read(), ICANON, ECHO, ... */

#include "../../WiTesting/wiAssert.h" 	/* wiAssert() */

/* This file implemenets wi_render_frame, wi_show_session */
#include "../include/wiTUI.h"

#include <time.h>

typedef struct terminal_size {
	int rows;
	int cols;
} terminal_size;

/* Get 1 key-press from the user */
char get_char() {
	char buf = 0;
	struct termios old = {0};
	/* Save old settings */
	wiAssert(tcgetattr(0, &old) >= 0, "tcsetattr()");

	/* Set to raw mode */
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	wiAssert(tcsetattr(0, TCSANOW, &old) >= 0, "tcsetattr ICANON");

	long read_result = read(STDIN_FILENO, &buf, 1);
	wiAssert(read_result >= 0, "Error reading key" );

	/* Set back to normal mode */
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	wiAssert(tcsetattr(0, TCSADRAIN, &old) >= 0, "tcsetattr ~ICANON");

	return buf;
}

void clear_screen() {
	printf("\033[1;1H\033[2J");
}

/*
 * Move the terminal cursor up or down.
 * Positive x means down, negative x means up.
 */
void cursor_move_vertical(int x) {
	if (x > 0) {
		printf("\033[%dA", x);
	} else if (x < 0) {
		x = -x;
		printf("\033[%dB", x);
	}
}

/*
 * Move the terminal cursor left or right.
 * Positive y means right, negative y means left.
 */
void cursor_move_horizontal(int y) {
	if (y > 0) {
		printf("\033[%dC", y);
	} else if (y < 0) {
		y = -y;
		printf("\033[%dD", y);
	}
}

/*
 * Move the cursor to an absolute position in the terminal.
 *
 * NOTE: currently unused
 */
void cursor_go_to(int x, int y) {
	printf("\033[%d;%dH", y, x);
}

/*
 * Move the cursor to an absolute row in the terminal.
 *
 * NOTE: currently unused
 */
void cursor_go_to_row(int row) {
	printf("\033[%dH", row);
}

/*
 * Get the current terminal size as a struct containing .rows and .cols.
 * The size is expressed in amount of characters.
 *
 * @returns: struct with current terminal size
 */
terminal_size get_terminal_size() {
	struct winsize max;
	ioctl(0, TIOCGWINSZ, &max);
	return (terminal_size) { max.ws_row, max.ws_col };
}

/*
 * Per row, calculate the rendered width for each window.
 * This only changes when the normal width is set to -1.
 * When multiple windows have their width set to -1, the available space
 * will be distributed equally between them.
 *
 * @returns: updated session
 */
wi_session* calculate_window_dimension(wi_session* session) {
	const terminal_size terminal_size = get_terminal_size();

	wi_window* window;
	int window_width;

	int min_row_height = terminal_size.rows;
	/* TODO: keep track of min sizes and assign the window positions */

	/* Step 1: check how wide each window will be */
	/* Step 2: assign max height to the windows */
	/* Step 3: calculate the positions for each window (don't factor overlap) */
	/* Step 4: detect overlap and resolve */

	/* Step 1 and 2 */
	for (int row = 0; row < session->_internal_amount_rows; row++) {
		wi_window* flex_windows[session->_internal_amount_cols[row]];
		int amount_to_compute = 0;
		int static_occupied_width = 0;

		for (int col = 0; col < session->_internal_amount_cols[row]; col++) {
			window = session->windows[row][col];

			if (window->size.is_flex_width) {
				flex_windows[amount_to_compute] = window;
				amount_to_compute++;
			} else if (window->size.is_perc_width) {
				wiAssert(window->size.width.percentage_width >= 0 && window->size.width.percentage_width <= 100);
				int width =
					terminal_size.cols * window->size.width.percentage_width / 100;
				window->_internal_rendered_width = width;
				static_occupied_width += width;
			} else {
				int width = window->size.width.fixed_width;
				window->_internal_rendered_width = width;
				static_occupied_width += width;
			}
		}
		/* TODO: do something with the flexis */
	}

	return session;
}

/*
 * Calculate the amount of characters to print on a line,
 * while preventing to wrap inside a word.
 *
 * This will make sure that `content_pointer[wrap]`
 * (`wrap` being the return value)
 * is either a newline, nullbyte, space or '-'.
 *
 * When there is a single word on the line that is too long to fit, the word
 * will be split at `width`.
 *
 * @returns: index on which to wrap the string
 */
int characters_until_wrap(char* content_pointer, int width) {
	int wrap = 0;
	char c;

	for (int i = 0; i < width; i++) {
		c = content_pointer[i];
		if (c == '\0' || c == '\n') {
			wrap = i;
			return wrap;
		} else if (c == ' ' || c == '-') {
			if (i + 1 < width) {
				wrap = i + 1;
			} else {
				wrap = i;
			}
		}
	}

	if (wrap == 0) {
		return width;
	}

	return wrap;
}

/*
 * Calculate how a content will be rendered in the context of the given window,
 * so accounting for:
 * 	- window._internal_rendered_width/height,
 * 	- window.wrapText
 * 	- window.cursor_rendering
 * 	- window.cursor_position
 *
 * The content will (if needed) be wrapped or shifted according to the cursor
 * position, and the cursor position will be highlighted if needed.
 *
 * The returned char** is allocated on the heap, and needs to be manually freed.
 * Each element (row) has to be freed, and the array itself too.
 *
 * @returns: string-array with the contents of each line
 */
char** calculate_contents(
	const wi_window* window,
	char* content_pointer,
	wi_cursor_rendering cursor_rendering
) {
	const int width = window->_internal_rendered_width;
	const int height = window->_internal_rendered_height;

	const char filler = ' ';

	const char cursor_on[] = "\033[7m";
	const char cursor_off[] = "\033[0m";
	const size_t cursor_on_length = strlen(cursor_on);
	const size_t cursor_off_length = strlen(cursor_off);

	const bool do_cursor_render = window->_internal_currently_focussed
		&& cursor_rendering != INVISIBLE;
	const bool do_line_render = window->_internal_currently_focussed
		&& cursor_rendering == LINEBASED;
	const bool do_point_render = window->_internal_currently_focussed
		&& cursor_rendering == POINTBASED;

	const int cursor_row = window->_internal_last_cursor_position.row;
	const int cursor_col = window->_internal_last_cursor_position.col;

	size_t amount_to_alloc;
	int offset;

	char** rendered_content = (char**) malloc(height * sizeof(char*));

	for (int current_height = 0; current_height < height; current_height++) {
		offset = 0;

		amount_to_alloc = width * sizeof(char) + 1; /* +1 for '\0' */
		if (do_cursor_render && current_height == cursor_row) {
			amount_to_alloc += cursor_on_length + cursor_off_length;
		}
		rendered_content[current_height] = (char*) malloc(amount_to_alloc);

		if (do_line_render && current_height == cursor_row) {
			/* memcpy instead of strcpy because we know the length */
			memcpy(rendered_content[current_height], cursor_on, cursor_on_length);
			offset = cursor_on_length;
		}

		int chars_until_wrap = characters_until_wrap(content_pointer, width);

		for (int i = 0; i < chars_until_wrap; i++) {
			/* Render cursor-point if needed */
			bool do_cursor_render_now =
				do_point_render && current_height == cursor_row
				&& (i == cursor_col || i - 1 == cursor_col);

			if (do_cursor_render_now) {
				const char* effect = i == cursor_col ? cursor_on : cursor_off;
				const size_t jump = i == cursor_col ? cursor_on_length : cursor_off_length;

			/* memcpy instead of strcpy because we know the length */
				memcpy(rendered_content[current_height] + offset + i, effect, jump);
				offset += jump;
			}

			rendered_content[current_height][offset + i] = *content_pointer;
			content_pointer++;
		}
		if (*content_pointer == '\n') {
			content_pointer++;
		}
		for (int i = chars_until_wrap; i < width; i++) {
			/* Render cursor-point if needed */
			bool do_cursor_render_now =
				do_point_render && current_height == cursor_row
				&& (i == cursor_col || i - 1 == cursor_col);

			if (do_cursor_render_now) {
				const char* effect = i == cursor_col ? cursor_on : cursor_off;
				const size_t jump = i == cursor_col ? cursor_on_length : cursor_off_length;

				memcpy(rendered_content[current_height] + offset + i, effect, jump);
				offset += jump;
			}

			rendered_content[current_height][offset + i] = filler;
		}

		if (do_line_render && current_height == cursor_row) {
			memcpy(rendered_content[current_height] + offset + width, cursor_off, cursor_off_length);
			offset += cursor_off_length;
		}

		rendered_content[current_height][offset + width] = '\0';
	}

	return rendered_content;
}

/*
 * Render a horizontal border with info (title/footer) in it.
 * Does not set any effect.
 * Does not jump horizontally.
 * Does not close any effect.
 * Does not place a newline.
 * Just renders the given elements, and only does that.
 *
 * The only magick happening, is the alignment.
 */
void render_window_border(
	const char* left, const char* mid, const char* right,
	const wi_info_alignment alignment, const char* info, const int width
) {
	int info_length = info == NULL ? 0 : strlen(info);
	int left_pad = 0;
	int right_pad = 0;

	if (info_length > width) {
		info_length = width;
	}

	switch (alignment) {
		case LEFT:
			right_pad = width - info_length;
			break;

		case CENTER:
			right_pad = (width - info_length) / 2;
			left_pad = width - info_length - right_pad;
			break;

		case RIGHT:
			left_pad = width - info_length;
			break;
	}

	printf("%s", left);
	for (int _ = 0; _ < left_pad; _++) {
		printf("%s", mid);
	}

	/* Restrain info-length if necessary (can't be longer then window-width) */
	printf("%.*s", info_length, info);

	for (int _ = 0; _ < right_pad; _++) {
		printf("%s", mid);
	}

	printf("%s", right);
}

/*
 * Render a window at the given `horizontal_offset`.
 * This assumes that the cursor already is at the right vertical space.
 */
void render_window(const wi_window* window, int horizontal_offset) {
	wi_border border = window->border;
	char* effect = "";

	if (border.corner_bottom_left != NULL) {
		if (window->_internal_currently_focussed) {
			effect = border.focussed_colour;
		} else {
			effect = border.unfocussed_colour;
		}

		cursor_move_horizontal(horizontal_offset);
		printf("%s", effect);
		render_window_border(
			border.corner_top_left, border.side_top, border.corner_top_right,
			window->title_alignment, window->title, window->_internal_rendered_width
		);
		printf("\033[0m\n");
	}

	/* Don't forget to free this one ;-) */
	char** contents = calculate_contents(window, window->contents[0][0], window->cursor_rendering);

	/* Print rows of content with border surrounding it */
	for (int i = 0; i < window->_internal_rendered_height; i++) {
		cursor_move_horizontal(horizontal_offset);

		if (border.corner_bottom_left != NULL) {
			printf("%s%s\033[0m", effect, border.side_left);
		}

		printf("%s", contents[i]);
		free(contents[i]);

		if (border.corner_bottom_left != NULL) {
			printf("\033[0m%s%s\033[0m", effect, border.side_right);
		}
		printf("\033[0m\n");
	}

	free(contents);

	if (border.corner_bottom_left != NULL) {
		cursor_move_horizontal(horizontal_offset);
		printf("%s", effect);
		render_window_border(
			border.corner_bottom_left, border.side_bottom, border.corner_bottom_right,
			window->footer_alignment, window->footer, window->_internal_rendered_width
		);
		printf("\033[0m\n");
	}
}

int wi_render_frame(wi_session* session) {
	int accumulated_row_width;
	int max_row_height;
	int accumulated_height = 0;

	wi_window* window;

	calculate_window_dimension(session);

	for (int row = 0; row < session->_internal_amount_rows; row++) {
		accumulated_row_width = 0;
		max_row_height = 0;

		for (int col = 0; col < session->_internal_amount_cols[row]; col++) {
			window = session->windows[row][col];

			render_window(window, accumulated_row_width);

			cursor_move_vertical(window->_internal_rendered_height);
			if (window->border.corner_bottom_left != NULL) {
				cursor_move_vertical(2);
			}

			accumulated_row_width += window->_internal_rendered_width + 2;
			if (window->_internal_rendered_height + 2 > max_row_height) {
				max_row_height = window->_internal_rendered_height + 2;
			}
		}
		cursor_move_vertical(-max_row_height);

		accumulated_height += max_row_height;
	}

	return accumulated_height;
}

/*
 * Convert a character that's in potentially a weird range, to one
 * in the range 'a-z', according to the modifier.
 * If the character is not in its expected range for the given modifier,
 * it is returned as-is.
 */
char normalised_key(char c, wi_modifier modifier) {
	switch (modifier) {
		case CTRL:
			if (c > 0 && c <= 26) {
				return c + 'a' - 1;
			}
			break;
		case SHIFT:
			if (c >= 'A' && c <= 'Z') {
				return c - 'A' + 'a';
			}
			break;
		default:
			break;
	}
	return 0;
}

/*
 * Handle off the key-press.
 * This can move the cursor-position between windows, and inside windows.
 */
void handle(char c, wi_session* session) {
	wi_window* focussed_window = session->windows[session->cursor_start.row][session->cursor_start.col];
	focussed_window->_internal_currently_focussed = false;

	wi_movement_keys m_keys = session->movement_keys;

	/* First check for ALT, because that's a 2-key combo */
	if (m_keys.modifier_key == ALT && c == 27) {
		c = get_char();
		if (c == m_keys.left && session->cursor_start.col > 0) {
			session->cursor_start.col--;
		} else if (c == m_keys.right && session->cursor_start.col + 1 < session->_internal_amount_cols[session->cursor_start.row]) {
			session->cursor_start.col++;
		} else if (c == m_keys.up && session->cursor_start.row > 0) {
			session->cursor_start.row--;
		} else if (c == m_keys.down && session->cursor_start.row + 1 < session->_internal_amount_rows) {
			session->cursor_start.row++;
		}

		/* Then check for normal keys without modifier */
	} else if (c == m_keys.left && focussed_window->_internal_last_cursor_position.col > 0) {
		focussed_window->_internal_last_cursor_position.col--;
	} else if (c == m_keys.right && focussed_window->_internal_last_cursor_position.col + 1 < focussed_window->_internal_rendered_width) {
		focussed_window->_internal_last_cursor_position.col++;
	} else if (c == m_keys.up && focussed_window->_internal_last_cursor_position.row > 0) {
		focussed_window->_internal_last_cursor_position.row--;
	} else if (c == m_keys.down && focussed_window->_internal_last_cursor_position.row + 1 < focussed_window->_internal_rendered_height) {
		focussed_window->_internal_last_cursor_position.row++;

		/* And then check for keys + modifier that produce a single char */
	} else {
		c = normalised_key(c, m_keys.modifier_key);

		if (c == m_keys.left && session->cursor_start.col > 0) {
			session->cursor_start.col--;
		} else if (c == m_keys.right && session->cursor_start.col + 1 < session->_internal_amount_cols[session->cursor_start.row]) {
			session->cursor_start.col++;
		} else if (c == m_keys.up && session->cursor_start.row > 0) {
			session->cursor_start.row--;
		} else if (c == m_keys.down && session->cursor_start.row + 1 < session->_internal_amount_rows) {
			session->cursor_start.row++;
		}
	}

	/* Sanitize the col-number, because it can be too large now */
	if (session->cursor_start.col + 1 >= session->_internal_amount_cols[session->cursor_start.row]) {
		session->cursor_start.col = session->_internal_amount_cols[session->cursor_start.row] - 1;
	}

	session->windows[session->cursor_start.row][session->cursor_start.col]->_internal_currently_focussed = true;
}

wi_result wi_show_session(wi_session* session) {
	wi_result cursor_position = (wi_result) {
		(wi_position) { 0, 0 },
		(wi_position) { 0, 0 }
	};

	int focus_row = session->cursor_start.row;
	int focus_col = session->cursor_start.col;
	wiAssert(
		session->cursor_start.row < session->_internal_amount_rows,
		"Can not focus on non-existing window."
	);
	wiAssert(
		session->cursor_start.col < session->_internal_amount_cols[focus_col],
		"Can not focus on non-existing window."
	);

	session->windows[focus_row][focus_col]->_internal_currently_focussed = true;

	if (session->full_screen) {
		clear_screen();
	}

	/* TEMP */
	double time_rendering = 0.0;
	int amount_rendered = 0;
	double current_time;
	double min_time = 1000000;
	double max_time = 0;


	clock_t begin = clock();
	int printed_height = wi_render_frame(session);
	current_time = (double) (clock() - begin) * 1000.0 / CLOCKS_PER_SEC;
	time_rendering += current_time;
	if (current_time < min_time) {
		min_time = current_time;
	}
	if (current_time > max_time) {
		max_time = current_time;
	}
	amount_rendered++;

	int c = get_char();
	while (c != session->movement_keys.quit) {
		handle(c, session);

		if (session->full_screen) {
			clear_screen();
		} else {
			cursor_move_vertical(printed_height);
		}

		begin = clock();
		printed_height = wi_render_frame(session);
		current_time = (double) (clock() - begin) * 1000.0 / CLOCKS_PER_SEC;
		time_rendering += current_time;
		if (current_time < min_time) {
			min_time = current_time;
		}
		if (current_time > max_time) {
			max_time = current_time;
		}
		amount_rendered++;

		c = get_char();
	}

	printf("Time spent rendering: 		%f ms\n", time_rendering);
	printf("Amount of rendering calls: 	%d\n", amount_rendered);
	printf("Average rendering-time: 	%f ms\n", time_rendering / amount_rendered);
	printf("Max rendering time: 		%f ms\n", max_time);
	printf("Min rendering time: 		%f ms\n", min_time);

	return cursor_position;
}

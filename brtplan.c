#include <ncurses.h>
#include <locale.h>
#include <string.h>

typedef struct region {
	int r, c, y, x;
} region;

WINDOW* newwin_reg(region reg)
{
	return newwin(reg.r, reg.c, reg.y, reg.x);
}

void wmovres_reg(WINDOW* win, region reg)
{
	wresize(win, reg.r, reg.c);
	mvwin(win, reg.y, reg.x);
}

enum brtwin {
	BRT_WEEK, BRT_EXERCISES, BRT_SETS, BRT_STATUS
};

void update_region(region scr, region* reg, int win)
{
#define STATUS_H scr.r - 3
#define MIDDLE_W scr.c - 10 - 5
	switch (win)
	{
		case BRT_WEEK:      *reg = (region){ scr.r    , 5       , 0        , 0          }; return;
		case BRT_EXERCISES: *reg = (region){ STATUS_H , MIDDLE_W, 0        , 5          }; return;
		case BRT_SETS:      *reg = (region){ scr.r    , 10      , 0        , scr.c - 10 }; return;
		case BRT_STATUS:    *reg = (region){ 3        , MIDDLE_W, STATUS_H , 5          }; return;
	}
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    /*
    region scr;
    region week_reg;
    region exercises_reg;
    region sets_reg;
    region status_reg;
    */
    int rows, cols
    getmaxyx(stdscr, rows, cols);

    const int orig_x = 12;
    const int keyboard_y = 9;
    const int menu_x = 20;
    WINDOW* win_orig = newwin(rows, orig_x, 0, 0);
    WINDOW* win_keyboard = newwin(keyboard_y, cols - orig_x, 0, orig_x);
    WINDOW* win_input = newwin(rows - keyboard_y, cols - orig_x - menu_x, keyboard_y, orig_x);
    WINDOW* win_menu = newwin(rows - keyboard_y, menu_x, keyboard_y, cols - menu_x);
    refresh();

    /*
    update_region(scr, &week_reg, BRT_WEEK);
    update_region(scr, &exercises_reg, BRT_EXERCISES);
    update_region(scr, &sets_reg, BRT_SETS);
    update_region(scr, &status_reg, BRT_STATUS);
    WINDOW* week_win      = newwin_reg(week_reg);
    WINDOW* exercises_win = newwin_reg(exercises_reg);
    WINDOW* sets_win      = newwin_reg(sets_reg);
    WINDOW* status_win    = newwin_reg(status_reg);
    */

    refresh();

    char input[80];
    int c = 0;
    while (c != 'q')
    {
	    if (c == KEY_RESIZE)
	    {
                    getmaxyx(stdscr, rows, cols);
	            wresize(win_orig, rows, orig_x );
	            wresize(win_keyboard, keyboard_y, cols - orig_x );
	            wresize(win_input, rows - keyboard_y, cols - orig_x - menu_x);
	            wresize(win_menu, rows - keyboard_y, menu_x);
	            mvwin(win_menu, keyboard_y, cols - menu_x);

	            wclear(win_orig);
	            wclear(win_keyboard);
	            wclear(win_menu);
	            wclear(win_input);
	    }

            print_original(win_orig, quiz_flags);
	    print_keyboard(win_keyboard);
	    print_menu(win_menu, (quiz_flags & QUIZ_FLAGS_SINGLE_KEY_MODE_BIT) >> 2);
	    print_input(win_input, vowel_index_selected, quiz_flags);

	    c = wgetch(win_input);

	    refresh();
    }

    delwin(week_win);
    delwin(exercises_win);
    delwin(sets_win);
    delwin(status_win);
    endwin();

    return 0;
}

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
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    const int weekday_x = 5;
    const int status_y = 3;
    const int exercises_x = 15;
    WINDOW* win_weekday = newwin(rows, weekday_x, 0, 0);
    WINDOW* win_exercise = newwin(status_y, cols - weekday_x, 0, weekday_x);
    WINDOW* win_status = newwin(rows - status_y, cols - weekday_x - exercises_x, status_y, weekday_x);
    WINDOW* win_sets = newwin(rows - status_y, exercises_x, status_y, cols - exercises_x);
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
	            wresize(win_weekday, rows, weekday_x );
	            wresize(win_exercise, status_y, cols - weekday_x );
	            wresize(win_status, rows - status_y, cols - weekday_x - exercises_x);
	            wresize(win_sets, rows - status_y, exercises_x);
	            mvwin(win_sets, status_y, cols - exercises_x);


	            wclear(win_weekday);
	            wclear(win_exercise);
	            wclear(win_sets);
	            wclear(win_status);
	    }

	    box(win_weekday, 0, 0);
	    wrefresh(win_weekday);

	    box(win_exercise, 0, 0);
	    wrefresh(win_exercise);
	    
	    box(win_status, 0, 0);
	    wrefresh(win_status);

	    box(win_sets, 0, 0);
	    wrefresh(win_sets);

	    c = wgetch(win_status);

	    refresh();
    }

    delwin(win_weekday);
    delwin(win_exercise);
    delwin(win_status);
    delwin(win_sets);
    endwin();

    return 0;
}

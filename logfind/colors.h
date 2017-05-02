#ifndef _colors_h_
#define _colors_h_

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define SET_COLOR(color, s) color s ANSI_COLOR_RESET

#define SET_RED(s) SET_COLOR(ANSI_COLOR_RED, s)
#define SET_GREEN(s) SET_COLOR(ANSI_COLOR_GREEN, s)
#define SET_YELLOW(s) SET_COLOR(ANSI_COLOR_YELLOW, s)
#define SET_BLUE(s) SET_COLOR(ANSI_COLOR_BLUE, s)
#define SET_MAGENTA(s) SET_COLOR(ANSI_COLOR_MAGENTA, s)
#define SET_CYAN(s) SET_COLOR(ANSI_COLOR_CYAN, s)

#endif

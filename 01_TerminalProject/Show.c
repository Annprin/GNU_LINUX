#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DX 7
#define DY 3

typedef struct {
    char **lines;
    int total_lines;
    int current_line;
} FileContent;

int main(int argc, char *argv[]) {
    WINDOW *frame, *win;
    int c = 0;
    FileContent content = {0};

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return 1;
    }

    char buffer[1024];
    content.lines = malloc(sizeof(char*) * 1000);
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        content.lines[content.total_lines] = strdup(buffer);
        content.total_lines++;
    }
    fclose(file);

    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    refresh();

    frame = newwin(LINES - 2*DY, COLS - 2*DX, DY, DX);
    box(frame, 0, 0);
    
    char title[256];
    snprintf(title, sizeof(title), " %s ", argv[1]);
    int title_len = strlen(title);
    int title_pos = (COLS - 2*DX - title_len) / 2;
    if (title_pos < 1) title_pos = 1;
    mvwprintw(frame, 0, title_pos, "%s", title);
    
    wrefresh(frame);

    win = newwin(LINES - 2*DY - 2, COLS - 2*DX - 2, DY + 1, DX + 1);
    keypad(win, TRUE);

    void redraw_content() {
        werase(win);
        int visible_lines = LINES - 2*DY - 2;
        
        for (int i = 0; i < visible_lines; i++) {
            int line_num = content.current_line + i;
            if (line_num < content.total_lines) {
                mvwprintw(win, i, 0, "%-*s", COLS - 2*DX - 3, content.lines[line_num]);
            }
        }
        wrefresh(win);
    }

    redraw_content();

    while ((c = getch()) != 27) {
        switch (c) {
            case ' ':
            case KEY_DOWN:
                if (content.current_line < content.total_lines - (LINES - 2*DY - 2)) {
                    content.current_line++;
                    redraw_content();
                }
                break;
            case KEY_UP:
                if (content.current_line > 0) {
                    content.current_line--;
                    redraw_content();
                }
                break;
            case KEY_NPAGE:
                content.current_line += LINES - 2*DY - 2;
                if (content.current_line > content.total_lines - (LINES - 2*DY - 2)) {
                    content.current_line = content.total_lines - (LINES - 2*DY - 2);
                }
                redraw_content();
                break;
            case KEY_PPAGE:
                content.current_line -= LINES - 2*DY - 2;
                if (content.current_line < 0) content.current_line = 0;
                redraw_content();
                break;
        }
    }

    for (int i = 0; i < content.total_lines; i++) {
        free(content.lines[i]);
    }
    free(content.lines);
    
    delwin(win);
    delwin(frame);
    endwin();
    return 0;
}

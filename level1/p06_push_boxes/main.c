#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define MAX_LEVELS 10
#define MAX_ROW 30
#define MAX_COL 50
#define MAP_FILE "map.txt"
#define SCORE_FILE "scores.txt"

// map element
#define WALL '#'
#define SPACE ' '
#define TARGET 'T'
#define BOX 'B'
#define BOX_ON_TARGET 'b'
#define PLAYER 'a'
#define PLAYER_ON_TARGET 'A'

// arrow key prefix in win32 console
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

char levels_maps[MAX_LEVELS][MAX_ROW][MAX_COL];
int level_row_cnt[MAX_LEVELS];
int level_col_cnt[MAX_LEVELS];
char current_map[MAX_ROW][MAX_COL];
int total_levels = 0;
int current_level = 0;
int map_rows = 0;
int map_cols = 0;
int player_x = 0, player_y = 0;
int steps = 0;
int total_targets = 0;
int total_boxes = 0;

void load_levels();
void load_current_level();
void draw_map();
int try_move(int dx, int dy);
int check_win();
void save_score();
void init_console();

int main()
{
    init_console();
    load_levels();

    if (total_levels == 0)
    {
        printf("\n[ERROR] Map data not found or format is incorrect!\n");
        printf("Please ensure '%s' exists and format is correct.\n\n", MAP_FILE);
        system("pause");
        return 1;
    }

    for (current_level = 0; current_level < total_levels; current_level++)
    {
        load_current_level();

        if (total_boxes != total_targets)
        {
            printf("[WARN] Level %d: Boxes (%d) and Targets (%d) count mismatch!\n",
                current_level + 1, total_boxes, total_targets);
            printf("Press any key to continue...\n");
            _getch();
        }

        int win = 0;
        while (!win)
        {
            draw_map();
            int ch = _getch();

            if (ch == KEY_ESC)
            {
                printf("\nGame Exited.\n");
                return 0;
            }
            if (ch == 'r' || ch == 'R')
            {
                load_current_level();
                continue;
            }

            int moved = 0;
            if (ch == 0 || ch == 224)
            {
                int move = _getch();
                switch (move)
                {
                case KEY_UP:    moved = try_move(0, -1); break;
                case KEY_DOWN:  moved = try_move(0, 1); break;
                case KEY_LEFT:  moved = try_move(-1, 0); break;
                case KEY_RIGHT: moved = try_move(1, 0); break;
                default: break;
                }
            }

            if (moved)
            {
                steps++;
                win = check_win();
            }
        }

        draw_map();
        printf("\n===== Congratulations! Level %d cleared =====\n", current_level + 1);
        printf("Steps used: %d\n", steps);
        save_score();
        printf("\nPress any key for next level...\n");
        _getch();
    }

    printf("\n\n===== Congratulations! You cleared all %d levels! =====\n", total_levels);
    system("pause");
    return 0;
}

// set console code page and window title
void init_console()
{
    system("chcp 65001 > nul");
    system("title Push Box Game");
}

// read all levels from map file
void load_levels()
{
    FILE* fp = fopen(MAP_FILE, "r");
    if (!fp)
        return;

    total_levels = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0)
            continue;

        int rows, cols;
        char name[50];
        if (sscanf(line, "%d %d %49s", &rows, &cols, name) == 3)
        {
            if (rows >= MAX_ROW || cols >= MAX_COL || rows <= 0 || cols <= 0)
            {
                printf("[WARN] Level %d size exceeds limits. Skipping.\n", total_levels + 1);
                break;
            }

            int valid = 1;
            for (int i = 0; i < rows; i++)
            {
                if (fgets(levels_maps[total_levels][i], MAX_COL, fp) == NULL)
                {
                    valid = 0;
                    break;
                }
                levels_maps[total_levels][i][strcspn(levels_maps[total_levels][i], "\r\n")] = '\0';
                int l = (int)strlen(levels_maps[total_levels][i]);
                if (l > cols)
                {
                    valid = 0;
                }
                // fill space to fixed cols
                for (int p = l; p < cols; p++)
                {
                    levels_maps[total_levels][i][p] = ' ';
                }
                levels_maps[total_levels][i][cols] = '\0';
            }

            if (valid)
            {
                level_row_cnt[total_levels] = rows;
                level_col_cnt[total_levels] = cols;
                total_levels++;
            }
            else
            {
                printf("[WARN] Level %d map is incomplete. Stopped reading.\n", total_levels + 1);
                break;
            }
        }
    }
    fclose(fp);
}

// copy selected level into runtime map, reset status
void load_current_level()
{
    steps = 0;
    total_targets = 0;
    total_boxes = 0;
    map_rows = level_row_cnt[current_level];
    map_cols = level_col_cnt[current_level];

    for (int i = 0; i < map_rows; i++)
    {
        strcpy(current_map[i], levels_maps[current_level][i]);
        int len = (int)strlen(current_map[i]);

        for (int j = 0; j < len; j++)
        {
            if (current_map[i][j] == PLAYER || current_map[i][j] == PLAYER_ON_TARGET)
            {
                player_y = i;
                player_x = j;
            }
            if (current_map[i][j] == TARGET || current_map[i][j] == BOX_ON_TARGET || current_map[i][j] == PLAYER_ON_TARGET)
            {
                total_targets++;
            }
            if (current_map[i][j] == BOX || current_map[i][j] == BOX_ON_TARGET)
            {
                total_boxes++;
            }
        }
    }
}

// render game map and HUD
void draw_map()
{
    system("cls");
    printf("================ PUSH BOX ================\n");
    printf(" Level: %d / %d  |  Steps: %d\n", current_level + 1, total_levels, steps);
    printf(" Controls: Arrow Keys to Move\n");
    printf("           [R] Reset Level  |  [ESC] Quit\n");
    printf(" Legend: '#' Wall, 'a' Player, 'B' Box, 'T' Target, 'b' Box on Target\n");
    printf("==========================================\n\n");
    for (int i = 0; i < map_rows; i++)
    {
        printf("%s\n", current_map[i]);
    }
}

// try move player, dx horizontal, dy vertical
// return 1 if moved successfully, else 0
int try_move(int dx, int dy)
{
    int next_x = player_x + dx;
    int next_y = player_y + dy;

    if (next_y < 0 || next_y >= map_rows)
        return 0;
    int row_len = map_cols;
    if (next_x < 0 || next_x >= row_len)
        return 0;

    char next_cell = current_map[next_y][next_x];
    if (next_cell == WALL || next_cell == '\0')
        return 0;

    // push box
    if (next_cell == BOX || next_cell == BOX_ON_TARGET)
    {
        int next_box_x = next_x + dx;
        int next_box_y = next_y + dy;

        if (next_box_y < 0 || next_box_y >= map_rows)
            return 0;
        int box_row_len = map_cols;
        if (next_box_x < 0 || next_box_x >= box_row_len)
            return 0;

        char box_next_cell = current_map[next_box_y][next_box_x];
        if (box_next_cell == WALL || box_next_cell == BOX || box_next_cell == BOX_ON_TARGET || box_next_cell == '\0')
        {
            return 0;
        }

        // leave old position
        current_map[player_y][player_x] = (current_map[player_y][player_x] == PLAYER_ON_TARGET) ? TARGET : SPACE;
        // move player to box old pos
        current_map[next_y][next_x] = (next_cell == BOX_ON_TARGET) ? PLAYER_ON_TARGET : PLAYER;
        // move box
        current_map[next_box_y][next_box_x] = (box_next_cell == TARGET) ? BOX_ON_TARGET : BOX;

        player_x = next_x;
        player_y = next_y;
        return 1;
    }

    // normal move to empty space or target
    if (next_cell == SPACE || next_cell == TARGET)
    {
        current_map[player_y][player_x] = (current_map[player_y][player_x] == PLAYER_ON_TARGET) ? TARGET : SPACE;
        current_map[next_y][next_x] = (next_cell == TARGET) ? PLAYER_ON_TARGET : PLAYER;

        player_x = next_x;
        player_y = next_y;
        return 1;
    }
    return 0;
}

// win check: all boxes are on target
int check_win()
{
    int boxes_on_target = 0;
    for (int i = 0; i < map_rows; i++)
    {
        int len = map_cols;
        for (int j = 0; j < len; j++)
        {
            if (current_map[i][j] == BOX_ON_TARGET)
            {
                boxes_on_target++;
            }
        }
    }
    return (boxes_on_target == total_boxes && total_boxes > 0);
}

// append score record to file
void save_score()
{
    FILE* fp = fopen(SCORE_FILE, "a");
    if (fp)
    {
        time_t t = time(NULL);
        struct tm* tm_info = localtime(&t);
        char time_str[30];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(fp, "[%s] Level %d: %d steps\n", time_str, current_level + 1, steps);
        fclose(fp);
    }
    else
    {
        printf("[ERROR] Could not save score to %s.\n", SCORE_FILE);
    }
}

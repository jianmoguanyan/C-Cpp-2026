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

// 地图元素定义
#define WALL '#'
#define SPACE ' '
#define TARGET 'T'
#define BOX 'B'
#define BOX_ON_TARGET 'b'
#define PLAYER 'a'
#define PLAYER_ON_TARGET 'A'

// Win32控制台方向键按键码
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

char levels_maps[MAX_LEVELS][MAX_ROW][MAX_COL];
int level_row_cnt[MAX_LEVELS];  // 保存每一关的行数
char current_map[MAX_ROW][MAX_COL];
int total_levels = 0;
int current_level = 0;
int map_rows = 0;
int player_x = 0, player_y = 0;
int steps = 0;
int total_targets = 0;
int total_boxes = 0;

void load_levels();
int load_current_level();
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
        // 地图校验失败的关卡直接跳过
        if (!load_current_level())
        {
            printf("Press any key to continue...\n");
            _getch();
            continue;
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

// 设置控制台编码与窗口标题
void init_console()
{
    system("chcp 65001 > nul");
    system("title Push Box Game");
}

// 从地图文件读取全部关卡
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
            // 达到关卡上限，不再读取后续关卡，防止数组越界
            if (total_levels >= MAX_LEVELS)
            {
                printf("[WARN] Reached the maximum level limit (%d). Extra levels ignored.\n", MAX_LEVELS);
                break;
            }
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

                if ((int)strlen(levels_maps[total_levels][i]) > cols)
                {
                    valid = 0;
                }
            }

            if (valid)
            {
                level_row_cnt[total_levels] = rows; // 记录当前关卡行数
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

// 加载选中关卡到运行地图，重置游戏状态
// 返回1代表关卡合法，返回0代表需要跳过该关卡
int load_current_level()
{
    steps = 0;
    total_targets = 0;
    total_boxes = 0;
    map_rows = level_row_cnt[current_level]; // 使用保存好的关卡行数

    memset(current_map, 0, sizeof(current_map)); // 清空上一关残留数据

    int player_count = 0;
    for (int i = 0; i < map_rows; i++)
    {
        strcpy(current_map[i], levels_maps[current_level][i]);
        int len = (int)strlen(current_map[i]);

        for (int j = 0; j < len; j++)
        {
            if (current_map[i][j] == PLAYER || current_map[i][j] == PLAYER_ON_TARGET)
            {
                player_x = j;
                player_y = i;
                player_count++;
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

    // 地图校验：必须有且仅有一个玩家
    if (player_count != 1)
    {
        printf("[ERROR] Level %d: Player count = %d (exactly 1 required).\n", current_level + 1, player_count);
        return 0;
    }
    // 地图校验：箱子数量必须等于目标点数量，否则无法通关
    if (total_boxes != total_targets || total_boxes == 0)
    {
        printf("[ERROR] Level %d: Boxes (%d) / Targets (%d) count mismatch.\n", current_level + 1, total_boxes, total_targets);
        return 0;
    }

    return 1;
}

// 绘制游戏地图与界面信息
void draw_map()
{
    system("cls");
    printf("================ PUSH BOX ================\n");
    printf(" Level: %d / %d | Steps: %d\n", current_level + 1, total_levels, steps);
    printf(" Controls: Arrow Keys to Move\n");
    printf(" [R] Reset Level | [ESC] Quit\n");
    printf(" Legend: '#' Wall, 'a' Player, 'B' Box, 'T' Target, 'b' Box on Target\n");
    printf("==========================================\n\n");
    for (int i = 0; i < map_rows; i++)
    {
        printf("%s\n", current_map[i]);
    }
}

// 尝试移动玩家，dx横向偏移，dy纵向偏移
// 移动成功返回1，失败返回0
int try_move(int dx, int dy)
{
    int next_x = player_x + dx;
    int next_y = player_y + dy;

    if (next_y < 0 || next_y >= map_rows)
        return 0;
    int row_len = (int)strlen(current_map[next_y]);
    if (next_x < 0 || next_x >= row_len)
        return 0;

    char next_cell = current_map[next_y][next_x];
    if (next_cell == WALL || next_cell == '\0')
        return 0;

    // 遇到箱子，执行推箱子逻辑
    if (next_cell == BOX || next_cell == BOX_ON_TARGET)
    {
        int next_box_x = next_x + dx;
        int next_box_y = next_y + dy;

        if (next_box_y < 0 || next_box_y >= map_rows)
            return 0;
        int box_row_len = (int)strlen(current_map[next_box_y]);
        if (next_box_x < 0 || next_box_x >= box_row_len)
            return 0;

        char box_next_cell = current_map[next_box_y][next_box_x];
        if (box_next_cell == WALL || box_next_cell == BOX || box_next_cell == BOX_ON_TARGET || box_next_cell == '\0')
        {
            return 0;
        }

        // 玩家离开原来的位置
        current_map[player_y][player_x] = (current_map[player_y][player_x] == PLAYER_ON_TARGET) ? TARGET : SPACE;
        // 玩家移动到箱子原来的位置
        current_map[next_y][next_x] = (next_cell == BOX_ON_TARGET) ? PLAYER_ON_TARGET : PLAYER;
        // 箱子移动到下一格
        current_map[next_box_y][next_box_x] = (box_next_cell == TARGET) ? BOX_ON_TARGET : BOX;

        player_x = next_x;
        player_y = next_y;
        return 1;
    }

    // 普通移动：走到空地或者目标点
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

// 判断游戏是否胜利：所有箱子都放在目标点上
int check_win()
{
    int boxes_on_target = 0;
    for (int i = 0; i < map_rows; i++)
    {
        int len = (int)strlen(current_map[i]);
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

// 将当前关卡成绩追加写入分数文件
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

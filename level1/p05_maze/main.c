#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define MAP_ROW 10
#define MAP_COL 50
#define WALL '#'
#define PLAYER 'a'
#define EXIT 'E'

void drawmap();

// 地图数据
char map[MAP_ROW][MAP_COL] =
{
    "#########################",
    "#a #  #           #     #",
    "#  #  #  #######  #  #  #",
    "#  #  #  #        #  #  #",
    "#  #  ####  ####  #  #  #",
    "#  #        #     #  #  #",
    "#  #  ####  #  #     #  #",
    "#  #  #  #  #  #######  #",
    "#     #     #        #  E",
    "#########################",
};

int nx = 1;   // nx：玩家所在列
int ny = 1;   // ny：玩家所在行
int px = 1;   // px：移动前列坐标
int py = 1;   // py：移动前行坐标
char move;

int main()
{
    while (1)
    {
        drawmap();
        move = _getch();

        // 保存移动前坐标
        px = nx;
        py = ny;
        int isMoveOk = 1;

        // 先预判目标坐标，增加边界范围判断
        switch (move)
        {
        case 'w':
            // 向上，行-1，必须 ny>0 才允许尝试移动
            if (ny > 0)
            {
                ny -= 1;
                if (map[ny][nx] == WALL)
                {
                    ny += 1;
                    isMoveOk = 0;
                }
            }
            else
            {
                // 已经到地图最上边，不能移动
                isMoveOk = 0;
            }
            break;
        case 'a':
            // 向左，列-1，nx>0
            if (nx > 0)
            {
                nx -= 1;
                if (map[ny][nx] == WALL)
                {
                    nx += 1;
                    isMoveOk = 0;
                }
            }
            else
            {
                isMoveOk = 0;
            }
            break;
        case 's':
            // 向下，行+1，ny < MAP_ROW-1
            if (ny < MAP_ROW - 1)
            {
                ny += 1;
                if (map[ny][nx] == WALL)
                {
                    ny -= 1;
                    isMoveOk = 0;
                }
            }
            else
            {
                isMoveOk = 0;
            }
            break;
        case 'd':
            // 向右，列+1，nx < MAP_COL -1
            if (nx < MAP_COL - 1)
            {
                nx += 1;
                if (map[ny][nx] == WALL)
                {
                    nx -= 1;
                    isMoveOk = 0;
                }
            }
            else
            {
                isMoveOk = 0;
            }
            break;
        default:
            // 其他按键：直接忽略，不执行移动
            isMoveOk = 0;
            break;
        }

        // 判断到达出口
        if (map[ny][nx] == EXIT)
        {
            map[py][px] = ' ';
            map[ny][nx] = PLAYER;
            drawmap();
            printf("\n===== 恭喜！游戏胜利 =====\n");
            goto end;
        }

        // 移动成功才更新地图
        if (isMoveOk == 1)
        {
            map[py][px] = ' ';
            map[ny][nx] = PLAYER;
        }
    }

end:
    printf("\n游戏结束\n");
    return 0;
}

//绘制迷宫地图，清屏后打印提示和迷宫

void drawmap()
{
    system("cls");
    printf("【操作说明】w上  s下  a左  d右\n");
    for (int i = 0;i < MAP_ROW;i++)
    {
        printf("%s\n", map[i]);
    }
}

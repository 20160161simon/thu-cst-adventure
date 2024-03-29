#ifndef PUBLIC_H_
#define PUBLIC_H_

// 全局变量头文件

// 位置类型，可用来表达机器人或灯所在位置
struct Position{
    int x, y; // x 表示列号，y 表示行号
};
// 方向枚举类型，可用来表达机器人朝向，只有四种取值
enum Direction{
    UP, DOWN, LEFT, RIGHT
};
// 机器人类型
struct Robot{
    Position pos; // 机器人位置
    Direction dir; // 机器人朝向
};
// 灯类型
struct Light{
    Position pos; // 灯位置
    bool lighten; // 1 表示灯被点亮， 0 表示灯被熄灭
};
// 单元格类型
struct Cell{
    int height; // 高度
    int light_id; // 灯标识, -1 表示该单元格没有灯
    int lighten; // 灯标识, 1/0 分别表示开灯/没开灯
    bool robot; // true/false 分别表示机器人在/不在该单元格上
};
// 指令类型
enum OpType{
    TL, TR, MOV, JMP, LIT, CALL
};
// TL为左转, TR为右转, MOV为向前行走, JMP为跳跃, LIT为点亮灯;
// 使用CALL表示调用MAIN，CALL + 1表示调用P1，以此类推。
// 过程类型
const int MAX_OPS = 100;
struct Proc{
    OpType ops[MAX_OPS]; // 指令记录
    int count; // 有效指令数
};
// 指令序列类型
const int MAX_PROCS = 1e3;
struct OpSeq{
    // 过程记录，MAX_PROCS为合理常数
    // procs[0]为MAIN过程，procs[1]为P1过程，以此类推
    Proc procs[MAX_PROCS];
    int count; // 有效过程数
};
// 地图状态类型
const int MAX_ROW = 100, MAX_COL = 100, MAX_LIT = 1e3;
struct Map{
    // 单元格组成二维数组，MAX_ROW、MAX_COL为合理常数
    Cell cells[MAX_ROW][MAX_COL];
    int row, col; // 有效行数、有效列数
    // 灯记录，MAX_LIT为合理常数
    Light lights[MAX_LIT];
    int num_lights; // 有效灯数
    // 地图上同时只有一个机器人
    Robot robot;
    // 每个过程的指令数限制
    int num_procs;
    int op_limit[MAX_PROCS];
};
// 游戏状态类型
const int MAX_PATH_LEN = 500;
struct Game{
    char map_name[MAX_PATH_LEN]; // 当前地图的文件路径名
    Map map_init; // 地图初始状态
    Map map_run; // 指令执行过程中的地图状态
    // 自动保存的文件路径名，MAX_PATH_LEN为合理常数
    char save_path[MAX_PATH_LEN];
    int auto_save_id; // 自动保存标识
    int limit; // 执行指令上限（用来避免无限递归）
};
Game game; // 全局唯一的Game变量

#endif

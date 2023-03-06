#ifndef plot_H_
#define plot_H_

#include "public.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

void save(const char * addr);
void auto_save();

#define PI 3.1415926

#pragma pack(1)
struct BMPFileHeader{
    char id[2];
    int size;
    int res;
    int offset;
};
#pragma pack()

#pragma pack(1)
struct DIBHeader{
    int size;
    int width;
    int height;
    short res1;
    short res2;
    int res3;
    int pixel_size;
    int res[4];
};
#pragma pack()

#pragma pack(1)
struct color{
    unsigned char b,g,r;
    bool operator == (const color x) const{
        return (r == x.r) && (g == x.g) && (b == x.b);
    }
};
#pragma pack()

const color black = {0, 0, 0};
const color blue = {240, 10, 9};
const color gray0 = {195, 195, 195};
const color gray1 = {178, 178, 178};
const color gray2 = {161, 161, 161};
const color purple = {164, 73, 163};
const color green = {19, 128, 20};
const color yellow = {0, 255, 255};

const int MAX_PIC = 2e3;
color pixel[MAX_PIC][MAX_PIC];

// 起点为 (x,y) 的 30 度直线函数在 x = x0 处的函数值 
int fun_line_30(int x, int y, int x0){
    // a 是斜率, 为 tan 30
    double a = tan(30 * PI / 180.0f);
    return a*x0 - a*x + y;
}

// 起点为 (x,y) 的 -30 度直线函数在 x = x0 处的函数值 
int fun_line_330(int x, int y, int x0){
    // a 是斜率, 为 tan 30
    double a = tan((-30) * PI / 180.0f);
    return a*x0 - a*x + y;
}

// 绘制方块函数
// 输入上方菱形左端点, 是否亮灯, 下方是否有方块
// 在 pixel[][] 中绘制宽度为 80 的菱形
void draw_block(int x,int y,int lighten,bool below){

    // 绘制竖线, 若下方有方块则要覆盖
    if(below){
        for(int i = 0; i < 40; i++){
            for(int j = 0; j < 10; j++){
                pixel[x + i][fun_line_30(x, y, x + i) + j] = gray1;
            }
        }
        for(int i = 40; i < 80; i++){
            for(int j = 0; j < 10; j++){
                pixel[x + i][fun_line_330(x + 79, y, x + i) + j] = gray2;
            }
        }
    }
    for(int j = 0; j < 10; j++){
        pixel[x][y + j] = black;
        pixel[x + 1][y + j] = black;
        pixel[x + 39][fun_line_30(x, y, x + 39) + j] = black;
        pixel[x + 39 + 1][fun_line_30(x, y, x + 39) + j] = black;
        pixel[x + 79][y + j] = black;
        pixel[x + 79 + 1][y + j] = black;
    }

    // 绘制左半边菱形
    for(int i = 0; i < 40; i++){

        for(int j = fun_line_30(x, y, x + i); j >= fun_line_330(x, y, x + i); j--){
            if(lighten == 1) pixel[x + i][j] = yellow; 
            else if(lighten == 2) pixel[x + i][j] = blue;
            else pixel[x + i][j] = gray0;
        }
        pixel[x + i][fun_line_30(x, y, x + i)] = black;
        pixel[x + i][fun_line_30(x, y, x + i) + 1] = black;
        pixel[x + i][fun_line_330(x, y, x + i)] = black;
        pixel[x + i][fun_line_330(x, y, x + i) + 1] = black;
    }

    // 绘制右半边菱形
    for(int i = 40; i < 80; i++){

        for(int j = fun_line_330(x + 79, y, x + i); j >= fun_line_30(x + 79, y, x + i); j--){
            if(lighten == 1) pixel[x + i][j] = yellow;
            else if(lighten == 2) pixel[x + i][j] = blue;
            else pixel[x + i][j] = gray0;
        }
        pixel[x + i][fun_line_30(x + 79, y, x + i)] = black;
        pixel[x + i][fun_line_30(x + 79, y, x + i) + 1] = black;
        pixel[x + i][fun_line_330(x + 79, y, x + i)] = black;
        pixel[x + i][fun_line_330(x + 79, y, x + i) + 1] = black;
        
    }

}

// 绘制机器人于方块上
// 输入方块左端点, 机器人方向

color robot_pixcel[50][100];

// 检查是否为紫色色系
bool check_purple(color x){
    if((purple.r - 50 < x.r && x.r < purple.r + 50) && (purple.g - 50 < x.g && x.g < purple.g + 50) && (purple.b - 50 < x.b && x.b < purple.b + 50)){
        return 1;
    }
    else return 0;
}

void draw_robot(int x, int y, Direction dir){
    // 若没有读取过 robot 图片
    // 则读取 robot.bmp 进入 robot_pixcel[][]
    if(robot_pixcel[0][0] == (color){0, 0, 0}){
        ifstream pic("./robot.bmp", ios::binary);
        BMPFileHeader bfh;
        DIBHeader dib;
        pic.read((char *)&bfh, sizeof(bfh));
        pic.read((char *)&dib, sizeof(dib));
        for(int y = dib.height - 1; y >= 0; y--){
            if(y != dib.height - 1){
                char temp[2];
                pic.read((char *)temp, sizeof(temp));
            }
            for(int x = 0; x < dib.width; x++){
                color p;
                pic.read((char *)&p, sizeof(p));
                robot_pixcel[x][y] = p;
            }
        }

        pic.close();
    }

    // 根据方向选择哪一块 robot_pixcel 进行写入
    int start_x, start_y;
    if(dir == UP) start_x = 25, start_y = 0;
    if(dir == DOWN) start_x = 0, start_y = 50;
    if(dir == LEFT) start_x = 0, start_y = 0;
    if(dir == RIGHT) start_x = 25, start_y = 50;
    
    // 写入 robot 到 pixcel
    for(int i = 0; i < 25; i++){
        for(int j = 0; j < 50; j++){
            if(check_purple(robot_pixcel[start_x + i][start_y + j])) continue;
            pixel[x + 28 + i][y - 45 + j] = robot_pixcel[start_x + i][start_y + j];
        }
    }
}

// 图像外围正方形框架
struct frame{
    int up,down,left,right;  
};

void save(const char *addr){

    // 计算整体框架大小
    frame overall_frama = {0, 0, 0, 0};
    for(int i = 0; i < game.map_run.row; i++){
        frame block[100];
        int start_y[100];
        for(int j = 0; j < game.map_run.col; j++){
            if(!j){
                block[j].left = (-40) * i;
                start_y[j] = fun_line_330(0, 0, block[j].left);
            }
            else{
                block[j].left = block[j - 1].left + 39;
                start_y[j] = fun_line_30(block[0].left, start_y[0], block[j].left);
            }
            block[j].up = fun_line_330(block[j].left, start_y[j] - 10 * game.map_run.cells[i][j].height, block[j].left + 39);
            block[j].right = block[j].left + 79;
            block[j].down = fun_line_30(block[j].left, start_y[j] + 10, block[j].left + 39);
            // cout << block[j].up << " " << block[j].down << " " << block[j].left << " " << block[j].right << endl;
            overall_frama.up = min(overall_frama.up, block[j].up);
            overall_frama.down = max(overall_frama.down, block[j].down);
            overall_frama.left = min(overall_frama.left, block[j].left);
            overall_frama.right = max(overall_frama.right, block[j].right);
        }
    }

    int width = overall_frama.right - overall_frama.left + 1 + 100;
    int height = overall_frama.down - overall_frama.up + 1 + 100;

    // 将背景设置成绿色
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            pixel[x][y] = green;
        }
    }

    // 起点偏移值
    int offset_value_x = 50 - overall_frama.left;
    int offset_value_y = 50 - overall_frama.up;

    // 绘制图像
    for(int i = 0; i < game.map_run.row; i++){
        frame block[100];
        int start_y[100];
        for(int j = 0; j < game.map_run.col; j++){
            if(!j){
                block[j].left = (-40) * i + offset_value_x;
                start_y[j] = fun_line_330(offset_value_x, offset_value_y, block[j].left);
            }
            else{
                block[j].left = block[j - 1].left + 39;
                start_y[j] = fun_line_30(block[0].left, start_y[0], block[j].left);
            }
            
            for(int k = 0; k < game.map_run.cells[i][j].height; k++){
                if(!k) draw_block(block[j].left, start_y[j], 0, 0);
                else draw_block(block[j].left, start_y[j] - k * 10, 0, 1);
            }
            if(game.map_run.cells[i][j].light_id != -1){
                if(game.map_run.cells[i][j].lighten) draw_block(block[j].left, start_y[j] - (game.map_run.cells[i][j].height - 1) * 10, 1, 0);
                else draw_block(block[j].left, start_y[j] - (game.map_run.cells[i][j].height - 1) * 10, 2, 0);
            }
            if(game.map_run.cells[i][j].robot){
                draw_robot(block[j].left, start_y[j] - (game.map_run.cells[i][j].height - 1) * 10, game.map_run.robot.dir);
            }
        }
    }

    // bmp 头文件输入
    ofstream fout(addr, ios::binary);

    int padding = (4 - ((width * 3) % 4)) % 4;


    BMPFileHeader bfh = {{'B', 'M'}, (width * 3 + padding) * height + 0x36, 0, 0x36};
    DIBHeader dib = {0x28, width, height, 1, 24, 0, (width * 3 + padding) * height, {0}};

    fout.write((char *)&bfh, sizeof(bfh));
    fout.write((char *)&dib, sizeof(dib));

    // bmp rgb 颜色写入
    for(int y = height - 1; y >= 0; y--){
        for(int x = 0; x < width; x++){
            fout.write((char *)&pixel[x][y], sizeof(pixel[x][y]));
        }
        unsigned char pad;
        pad = 0;
        for(int x = 0; x < padding; x++){
            fout.write((char *)&pad, sizeof(pad));
        }
    }

    fout.close();
    return;
}

void auto_save(){
    if(!strcmp(game.save_path, "OFF")) return;
    if(strlen(game.save_path) == 0){
        cout << "WARNING : SAVE PATH IS EMPTY !" << endl;
        return;
    }
    char addr[100];
    string temp = game.save_path;
    for(int i = 0; i < (int)temp.size(); i++){
        if(temp[i] == '%' && temp[i + 1] <= '9' && temp[i + 1] >= '0' && temp[i + 2] == 'd'){
            temp.insert(i + 1, ".");
            i = i + 3;
        }
    }
    sprintf(addr, temp.c_str(), game.auto_save_id);
    save(addr);
    game.auto_save_id ++;
}

#endif
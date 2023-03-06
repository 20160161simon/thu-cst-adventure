#ifndef RUN_H_
#define RUN_H_

#include "public.h"
#include "plot.h"
#include "interact.h"
#include <iostream>
#include <fstream>

using namespace std;

enum ResultType{
    UNKNOWN,
    LIGHT,
    LIMIT,
    DARK
};

struct Result{
    int steps;
    ResultType result;
};
Result robot_run(const char * path);

int light = 0;//点亮的灯数
int steps = 0;

OpType trans(char op[]){
    if(!strcmp(op, "TL")){
        return TL;
    }else if(!strcmp(op, "TR")){
        return TR;
    }else if(!strcmp(op, "MOV")){
        return MOV;
    }else if(!strcmp(op, "JMP")){
        return JMP;
    }else if(!strcmp(op, "LIT")){
        return LIT;
    }else if(!strcmp(op, "MAIN")){
        return CALL;
    }else if(op[0]=='P'){
        int temp = 0;
        for(int i = 1; i < (int)strlen(op); i++){
            temp *= 10;
            temp += op[i] - '0';
        }
        return OpType(CALL + temp);
    }else{
        return OpType(100);
    }
}

OpSeq P;

Result Operation(int Proc_Num , Result R){
    for(int i = 0 ; i < P.procs[Proc_Num].count ; i++){
        steps++;
        R.steps = steps;
        if(steps >= game.limit){
            R.result = LIMIT;
            light = 0;
            return R;
        }
        int X = game.map_run.robot.pos.x;
        int Y = game.map_run.robot.pos.y;
        int auto_save_flag = 1;
        Direction DIR = game.map_run.robot.dir;
        switch(P.procs[Proc_Num].ops[i]){
            case TL:
                switch(DIR){
                    case UP:
                        game.map_run.robot.dir = LEFT;
                        break;
                    case LEFT:
                        game.map_run.robot.dir = DOWN;
                        break;
                    case DOWN:
                        game.map_run.robot.dir = RIGHT;
                        break;
                    case RIGHT:
                        game.map_run.robot.dir = UP;
                        break;
                }
                break;
            case TR:
                switch(DIR){
                    case UP:
                        game.map_run.robot.dir = RIGHT;
                        break;
                    case LEFT:
                        game.map_run.robot.dir = UP;
                        break;
                    case DOWN:
                        game.map_run.robot.dir = LEFT;
                        break;
                    case RIGHT:
                        game.map_run.robot.dir = DOWN;
                        break;
                }
                break;
            case MOV:
                switch(DIR){
                    case UP:
                        if(Y == 0){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(game.map_run.cells[Y - 1][X].height != game.map_run.cells[Y][X].height){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y - 1][X].robot = 1;
                            game.map_run.robot.pos.y--;
                        }
                        break;
                    case LEFT:
                        if(X == 0){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(game.map_run.cells[Y][X - 1].height != game.map_run.cells[Y][X].height){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y][X - 1].robot = 1;
                            game.map_run.robot.pos.x--;
                        }
                        break;
                    case DOWN:
                        if(Y == game.map_run.row - 1){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(game.map_run.cells[Y + 1][X].height != game.map_run.cells[Y][X].height){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y + 1][X].robot = 1;
                            game.map_run.robot.pos.y++;
                        }
                        break;
                    case RIGHT:
                        if(X == game.map_run.col - 1){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(game.map_run.cells[Y][X + 1].height != game.map_run.cells[Y][X].height){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y][X + 1].robot = 1;
                            game.map_run.robot.pos.x++;
                        }
                        break;
                }
                break;
            case JMP:
                switch(DIR){
                    case UP:
                        if(Y == 0){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(!(game.map_run.cells[Y - 1][X].height == game.map_run.cells[Y][X].height + 1 || game.map_run.cells[Y - 1][X].height == game.map_run.cells[Y][X].height - 1)){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y - 1][X].robot = 1;
                            game.map_run.robot.pos.y--;
                        }
                        break;
                    case LEFT:
                        if(X == 0){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(!(game.map_run.cells[Y][X - 1].height == game.map_run.cells[Y][X].height + 1 || game.map_run.cells[Y][X - 1].height == game.map_run.cells[Y][X].height - 1)){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y][X - 1].robot = 1;
                            game.map_run.robot.pos.x--;
                        }
                        break;
                    case DOWN:
                        if(Y == game.map_run.row - 1){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(!(game.map_run.cells[Y + 1][X].height == game.map_run.cells[Y][X].height + 1 || game.map_run.cells[Y + 1][X].height == game.map_run.cells[Y][X].height - 1)){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y + 1][X].robot = 1;
                            game.map_run.robot.pos.y++;
                        }
                        break;
                    case RIGHT:
                        if(X == game.map_run.col - 1){
                            cout << "WARNING : OUT OF MAP" << endl;
                        }
                        else if(!(game.map_run.cells[Y][X + 1].height == game.map_run.cells[Y][X].height + 1 || game.map_run.cells[Y][X + 1].height == game.map_run.cells[Y][X].height - 1)){
                            cout << "WARNING : WRONG RELATIVE HEIGHT" << endl;
                        }
                        else{
                            game.map_run.cells[Y][X].robot = 0;
                            game.map_run.cells[Y][X + 1].robot = 1;
                            game.map_run.robot.pos.x++;
                        }
                        break;
                }
                break;
            case LIT:
                if(game.map_run.cells[Y][X].light_id != -1){
                    if(!game.map_run.cells[Y][X].lighten){
                        game.map_run.cells[Y][X].lighten = 1;
                        light++;
                    }
                    else{
                        cout << "WARNING : THE LIGHT ON YOUR POSITION HAS BEEN LIGHTEN" << endl;
                    }
                }
                else if(game.map_run.cells[Y][X].light_id == -1){
                    cout << "WARNING : THERE IS NOT A LIGHT ON YOUR POSITION" << endl;
                }
                break;
            default://调用情况
                R = Operation(P.procs[Proc_Num].ops[i] - CALL, R);
                auto_save_flag = 0;
        }
        if(auto_save_flag) auto_save();
        //结束条件
        if(R.result == UNKNOWN && light == game.map_run.num_lights){
            R.result = LIGHT;
            light = 0;
            return R;
        }
        else if(R.result == UNKNOWN && Proc_Num == 0 && i + 1 == P.procs[0].count){
            R.result = DARK;
            light = 0;
            return R;
        }
    }
    return R;
}

Result robot_run(char * path){
    char op_[100];
    int tmp = 0;
    Result res = {0 , ResultType::UNKNOWN};
    OpSeq opseq;
    game.map_run = game.map_init;
    steps = 0;
    // auto_save();
    print_map();
    //输入
    ifstream fin(path);
    if (!fin.fail()){
        fin>>opseq.count;
        for(int i=0;i<opseq.count;i++)
        {
            fin >> opseq.procs[i].count;
            if(opseq.procs[i].count > game.map_run.op_limit[i]){
                cout << "WARNNING : THE NUMBER OF PROCESS IS OUT OF LIMIT!" << endl;
            }
            for(int j=0;j<opseq.procs[i].count;j++)
            {
                // 读入指令
                fin>>op_;
                opseq.procs[i].ops[j]=trans(op_);
                if(opseq.procs[i].ops[i]!=100)tmp++;
            }
        }
    }
    else cout<<"File failed!"<<endl;
    fin.close();
    P = opseq;
    return Operation(0 , res);//返回结束原因
}

#endif
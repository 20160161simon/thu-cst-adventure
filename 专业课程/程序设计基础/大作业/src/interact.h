#ifndef INTERACT_H_
#define INTERACT_H_

#include "public.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string.h>

using namespace std;

void init(){
    game.limit=100;
    strcpy(game.map_name,"");
    return ;
}

void load_map_file(ifstream & infile){
    Map *mp=&(game.map_init);
    infile>>mp->row>>mp->col>>mp->num_lights>>mp->num_procs;
    for(int i=0;i<mp->row;i++)
    {
        for(int j=0;j<mp->col;j++)
        {
            infile>>mp->cells[i][j].height;
            mp->cells[i][j].light_id=-1;
            mp->cells[i][j].robot=false;
            mp->cells[i][j].lighten=false;
        }
    }
    for(int i=0;i<mp->num_lights;i++)
    {
        int x,y;
        infile>>x>>y;
        mp->cells[y][x].light_id=i;
    }
    for(int i=0;i<mp->num_procs;i++)
    {
        infile>>mp->op_limit[i];
    }
    int x,y,d;
    infile>>x>>y>>d;
    mp->robot.pos.x=x;
    mp->robot.pos.y=y;
    mp->robot.dir=(Direction)d;
    mp->cells[y][x].robot=true;
    game.map_run=game.map_init;

    return ;
}

void print_map(){
    Map *mp=&game.map_run;
    for(int i=0;i<mp->row;i++)
    {
        for(int j=0;j<mp->col;j++)
        {
            if(mp->cells[i][j].height==0)
            {
                cout<<"\e[0m";
                cout<<" ";
            }
            else 
            {
                string format="\e[";
                if(mp->cells[i][j].robot==true)format+="91;";
                else format+="92;";
                
                if(mp->cells[i][j].light_id==-1)format+="100;";
                else if(mp->cells[i][j].lighten==false)format+="104;";
                else format+="103;";

                format+="1m";
                format+=(char)(mp->cells[i][j].height+'0');
                cout<<format;
            }
        }
        cout<<"\e[0m"<<endl;
    }
}

void stat()
{
    cout<<"Map Name: "<<game.map_name<<endl;
    cout<<"Autosave: "<<game.save_path<<endl;
    cout<<"Step Limit: "<<game.limit<<endl;

    print_map();

    Map *mp=&game.map_init;

    cout<<"Robot is facing ";
    if(mp->robot.dir==0)cout<<"UP"<<endl;
    if(mp->robot.dir==1)cout<<"DOWN"<<endl;
    if(mp->robot.dir==2)cout<<"LEFT"<<endl;
    if(mp->robot.dir==3)cout<<"RIGHT"<<endl;

    cout<<"Proc Limit: [";
    for(int i=0;i<mp->num_procs;i++)
    {
        cout<<mp->op_limit[i];
        if(i!=mp->num_procs-1)cout<<", ";
    }
    cout<<"]";
    cout<<endl;
}

#endif
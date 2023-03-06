#include "public.h"
#include "plot.h"
#include "run.h"
#include "interact.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <string.h>

using namespace std;

int main(){
    init();
    ifstream infile;
    ofstream outfile;
    string command;
    cin>>command;
    while(command!="EXIT")
    {
        if(command=="LOAD")
        {
            cin>>game.map_name;
            infile.open(game.map_name);
            load_map_file(infile);
            infile.close();
            // Map *p=&game.map_init;
        }
        else if(command=="AUTOSAVE")
        {
            char path[500];
            cin >> path;
            if(!strcmp(path, "OFF"))
            {
                strcpy(game.save_path,"");
            }
            else strcpy(game.save_path,path);
            auto_save();
        }
        else if(command=="LIMIT")
        {
            int limit;
            cin>>limit;
            game.limit=limit;
        }
        else if(command=="STATUS")
        {
            stat();
        }
        else if(command=="OP")
        {
            string path;
            cin>>path;
            // cout<<path<<endl;
            outfile.open(path);
            int T;
            cin>>T;
            outfile<<T<<endl;
            while(T--)
            {
                long long int num=0;
                cin>>num;
                outfile<<num<<" ";
                for(int i=1;i<=num;i++)
                {
                    string s;
                    cin>>s;
                    outfile<<s<<" ";
                }
                outfile<<endl;
            }
            outfile.close();
        }
        else if(command=="RUN") 
        {
            char path[100];
            cin>>path;
            infile.open(path);
            if(!infile)
            {
                cout<<"ERROR";
            }
            else 
            {
                Result result=robot_run(path);
                cout<<"Run "<<path<<", result: ";
                if(result.result == LIGHT)cout<<"LIGHT"<<endl;
                else if(result.result == LIMIT)cout<<"LIMIT"<<endl;
                else if(result.result == DARK)cout<<"DARK"<<endl;
                cout<<"Step(s) used "<<result.steps<<endl;
                print_map();
                cout<<"Robot is facing ";
                if(game.map_run.robot.dir==0) cout<<"UP"<<endl;
                if(game.map_run.robot.dir==1) cout<<"DOWN"<<endl;
                if(game.map_run.robot.dir==2) cout<<"LEFT"<<endl;
                if(game.map_run.robot.dir==3) cout<<"RIGHT"<<endl;
            }
        }
        cin>>command;
    }
    return 0;
}
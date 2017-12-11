/**************************************************
** Copyright (c) 2017 BIGABLLON All rights reserved.
** Q Learning [3x3]Maze Demo
** 
**   's' -> start 'e' -> end '-' -> road
**
**   |-|-|-|        |-|s|-|
**   |-|-|-|   ->   |-|-|-|
**   |-|-|-|        |-|-|e|
**
***************************************************/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <ctime>

#ifdef __linux__
    #include <unistd.h>
#else
    #include <windows.h>
#endif

using namespace std;

const int sleep_time = 200;         // sleep time for printing maze grid 
const double gamma = 0.8;           // learning rate

int reward[9][9];                   // reward table
int start, target;                  // start/target position
int visited;                        // count the number of states reached target
bool is_end[9];                     // mark the state which reached target
bool show_action = true;            // true for printing maze grid         
char maze[3][3];                    // maze grid 
double Q[9][9];                     // Q valube table

void print_reward_table();
void print_Q_table();
void print_maze();
void do_maze(int from,int to, bool show);
void show_delay(const unsigned int milliseconds);
void initialization();
int get_action(int state);
double get_max_Q(int state);

int main(){

    srand((unsigned)time(NULL));
    initialization();
    printf("===== STATR TRAINING! ===== \n\n");

    // ================ Q learning algorithm ================
    int eps = 1;                                // episode number
    while( eps++ ){
        if (visited >= 8) break;                // if all the states can reach target, break
        int state = rand() % 9;        
        if (is_end[state]) continue;            // choose the state which hasn't visit

        printf("\n===== START EPISODE #%d: ===== \n",eps);
        
        int step = 0;
        while( state != target ){

            int action = get_action(state);                                         // select an possible action           
            Q[state][action] = reward[state][action] + gamma * get_max_Q(action);   // update Q table 
                                                                                    // base on Q(st,ac) = R + lr * Max(Q(st',all action))
            do_maze(state,action,show_action);  
            step++;                             // calculate current steps
            state = action;                     // s = s'
        }
        is_end[state] = true, visited++;        // mark the state & add the reach number
        printf("===== FINISHED EPISODE #%d , TOTAL STEP: %d ===== \n\n", eps, step);
    }
    printf("===== FINISHED TRAINING , Q_TABLE ===== \n");
    print_Q_table();
    return 0;
}

void initialization(){
    
    printf("===== INITIAL PARAMETERS! =====\n");    
    memset(Q,0,sizeof(Q));
    memset(is_end,false,sizeof(is_end));
    memset(maze,'-',sizeof(maze));
    visited = 0;

    printf("===== RANDOM INITIAL MAZE! =====\n");
    start = rand() % 9;
    do{
        target = rand() % 9;
    } while (target == start );
    maze[start/3][start%3] = 's';
    maze[target/3][target%3] = 't';

    print_maze();

    printf("===== SET REWARD TABLE =====\n");
    memset(reward,-1,sizeof(reward));
    for (int i = 0; i < 9; ++i){
        int x = i / 3;
        int y = i % 3;
        if(y + 1 < 3) reward[i][i+1] = -2;
        if(y - 1 >= 0 ) reward[i][i-1] = -2;
        if(x + 1 < 3) reward[i][i+3] = -2;
        if(x - 1 >= 0 ) reward[i][i-3] = -2;
    }
    int ex = target / 3;
    int ey = target % 3;
    if(ey + 1 < 3)   reward[target+1][target] = 100;
    if(ey - 1 >= 0 ) reward[target-1][target] = 100;
    if(ex + 1 < 3)   reward[target+3][target] = 100;
    if(ex - 1 >= 0 ) reward[target-3][target] = 100;
    print_reward_table();
}

int get_action(int state){
    int ac = 0;
    do{
        ac = rand() % 9;
    } while ( reward[state][ac] == -1 );
    return ac;
} 

double get_max_Q(int state){
    double max_q = 0.0;
    for (int i = 0; i < 9; ++i){
        if (reward[state][i] > max_q){
            max_q = reward[state][i];
        }
    }
    return max_q;
}

void show_delay(const unsigned int milliseconds)
{
#ifdef __linux__
    usleep(milliseconds * 1000);
#else
    Sleep(milliseconds);
#endif
}

void print_maze(){
    for(int i = 0; i < 3; ++i){
        printf("     ");
        for(int j = 0; j < 3; ++j){
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
    show_delay(sleep_time);
}

void print_reward_table(){

    printf("  | ");
    for(int i = 0; i < 9; ++i){
        printf("%3d ", i);
    }
    printf("\n   -------------------------------------\n");
    for(int i = 0; i < 9; ++i){
        printf("%d | ", i);
        for(int j = 0; j < 9; ++j){
            printf("%3d ", reward[i][j]);
        }
        printf("| \n");
    }
}

void print_Q_table(){

    printf("  | ");
    for(int i = 0; i < 9; ++i){
        printf("%3d ", i);
    }
    printf("\n   -------------------------------------\n");
    for(int i = 0; i < 9; ++i){
        printf("%d | ", i);
        for(int j = 0; j < 9; ++j){
            printf("%3.0f ", Q[i][j]);
        }
        printf("| \n");
    }
}

void do_maze(int from,int to, bool show){
    printf("from: %d, to: %d \n", from, to);
    memset(maze,'-',sizeof(maze));
    maze[from/3][from%3] = '-';
    maze[to/3][to%3] = 's';
    maze[target/3][target%3] = 't';
    if(show){
        print_maze();
    }
    
}
#include "const_global.h"
#include "engine.h"
#include "utils.h"
#include "game.h"

#include <stdio.h>
#include <random>
#include <bits/stdc++.h>

void init(){
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            id_board[i][j] = -1;

    std::mt19937_64 rnd(20201);
    
    for (int i = 0; i<8; i++)
        for(int j = 0; j<8; j++)
            for(int k=0; k<1200; k++)
                zboard[i][j][k]= std::abs((long long)rnd());
    
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++){
            zlightning[i][j]=std::abs((long long)rnd());;
            zmeteor[i][j]=std::abs((long long)rnd());;
        }
    
    for(int i=0; i<200; i++){
        znulled[i]=rnd();
        zvoided[i]=rnd();
        zstatus_id[i]=std::abs((long long)rnd())%(1LL<<30);
    }

    for(int i=0; i<250; i++){
        zmorale[0][i]=std::abs((long long)rnd());;
        zmorale[1][i]=std::abs((long long)rnd());;
    }

    zblack=std::abs((long long)rnd());

    last_moved[0]=-1;
}

const bool enterPos=true;
int main()
{
    init();
    hash=1;
    int side=0;
    int depth=0;
    if(enterPos){
        std::string position;
        std::cin>>position;
        loadPosition(position);
        std::cin>>side;
        std::cin>>depth;
    }

    printState();
    while(true){
        auto start = std::chrono::system_clock::now();
        for(int i=2; i<depth; i++)
            evaluate(-1000000, 1000000, i, side);    
        std::cout<<"evaluation: "<<evaluate(-1000000, 1000000, depth, side)<<std::endl;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "elapsed time: " << elapsed_seconds.count() << "s"
        << std::endl;
        std::cout << "normal nodes: "<<nodes<<" quiesent nodes: "<<quiesent_nodes<<std::endl;
        printState();
        printChosenMove();
        makeChosenMove();
        endOfTurnTriggers(side);

        std::cout<<"OPERATIONS: "<<std::endl;
        for(int i=0; i<undo_pnt[turn]; i++)
            std::cout<<undostack[turn][i][0]<<" "<<undostack[turn][i][1]<<" "<<undostack[turn][i][2]<<" "<<undostack[turn][i][3]<<" "<<undostack[turn][i][4]<<" "<<undostack[turn][i][5]<<std::endl;
        
        start_turn++;
        turn++;
        printState();
        printChosenMove();

        int xx, yy, id, mt;

        while(true){
            int mode;
            std::cin>>mode;
            if(mode==-1){
                std::string option;
                std::cin>>option;
                if(option=="depth"){
                    std::cin >> depth;
                    std::cout << "NEW DEPTH: "<<depth <<std::endl;
                }
                if(option=="undo"){
                    turn--;
                    start_turn--;
                    unmakeMoves(turn);
                    turn--;
                    start_turn--;
                    unmakeMoves(turn);
                    printState();
                }
            }
            else{
                xx=mode;
                break;
            }
        }
        std::cin>>yy>>id>>mt;
        makeMove(xx, yy, id, mt);
        endOfTurnTriggers(1-side);
        turn++;
        start_turn++;
    }
    return 1;
}


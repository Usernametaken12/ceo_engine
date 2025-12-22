#include "const_global.h"
#include "engine.h"
#include "utils.h"
#include "game.h"

#include <bits/stdc++.h>

long double time_limit=0;

int predictPVT(int pc, int x, int y, int xx, int yy){
    return piece_square_tables[piece_type[pc/2]][pc&1 ? yy : 7-yy][xx] - piece_square_tables[piece_type[pc/2]][pc&1 ? y : 7-y][x];
}

const int agrobonus=10000;
int moveScore(int mnum){
    int xx = candidateMoveStack[turn][mnum][0];
    int yy = candidateMoveStack[turn][mnum][1];
    int pc_id = candidateMoveStack[turn][mnum][2];
    switch(candidateMoveStack[turn][mnum][3]){
        case 1: //move
            return predictPVT(pieces[pc_id], px[pc_id], py[pc_id], xx, yy);
        case 2: //attack
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 3: //swap
            return -1;
        case 4:
            return -1;
        case 5:
            return 2;
        case 6:
            return 3;
        case 7:
            return agrobonus + pmorale[id_board[xx][yy]]*100-300 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 8:
            return agrobonus + pmorale[id_board[xx][yy]]*100-300 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 9:
            return agrobonus + pmorale[id_board[xx][yy]]*100-100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 10:
            return 1;
        case 11:
            return 4;
        case 12:
            return 1;
        case 13:
            return 2;
        case 14:
            return agrobonus + pmorale[id_board[xx][yy]]*100+100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 15:
            return agrobonus + pmorale[id_board[xx][yy]]*100+150 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 16:
            return agrobonus + pmorale[id_board[xx][yy]]*100+200 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 17:
            return agrobonus + pmorale[id_board[xx][yy]]*100+300 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 18:
            return -2;
        case 19:
            return -3;
        case 20:
            return -500;
        case 21:
            return -100;
        case 22:
            return -50;
        case 23:
            return -50;
        case 24:
            return 200;
        case 25:
            return -300;
        case 26:
            return agrobonus + (pmorale[id_board[xx][yy]] - pmorale[pc_id])*100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 27:
            return -10;
        case 28: 
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 29:
            return agrobonus +  200*pmorale[id_board[xx][yy]];
        case 30:
            return agrobonus + (pmorale[id_board[xx][yy]] - pmorale[pc_id])*100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        case 31:
            return -100;
        case 32:
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100 + agrobonus*(id_board[xx][yy]==last_moved[turn]);
        default:
            throw std::invalid_argument( "unexpected move type" ); 
    }
    return -1;
}

int selectionSortArray[200][200] ={};

void initSelectionSort(){
    for(int i=0; i<candidate_pointer[turn]; i++)
        selectionSortArray[turn][i]= moveScore(i);
}

int selectionSort(){
    int best = -1;
    int score = -1000000;
    for(int i=0; i<candidate_pointer[turn]; i++){
        if(selectionSortArray[turn][i]>score){
            best = i;
            score = selectionSortArray[turn][i];
        }
    }
    if(best==-1)
        throw std::logic_error("selection sort went out of bounds");
    return best;
}

/*int quicksortArray[200][200] = {};
void quickSort(){
    for(int i=0; i<candidate_pointer[turn]; i++){
        quicksortArray[turn][i] = ((moveScore(i)+agrobonus)<<8)+i;
    }
    
    std::sort(std::begin(quicksortArray[turn]), quicksortArray[turn] + candidate_pointer[turn], std::greater<int>());
}*/

int static_evaluation(int side){
    return (morale[0]-morale[1])*100 + (position_bonus[0]-position_bonus[1]);
}

int quiesence(int alpha, int beta, int side)
{
    if(DEBUG)
        printState();
    ++quiesent_nodes;
    if (morale[0] <= 0)
        return morale[1] <= 0 ? 0 : -1000000;
    if (morale[1] <= 0)
        return 1000000;

    if(side==0&&static_evaluation(side)>=beta)
        return beta;
    else if(side==1&&static_evaluation(side)<=alpha)
        return alpha;
    

    int best = 1000000*(-1+2*side);

    int tail = (hash^(side ? zblack : 0))&hash_mask;
    if(zobrist[tail]==(hash^(side ? zblack : 0))){
        if(nodeType[tail]==0)
            return nodeEval[tail];
        if(side==0){
            if(nodeType[tail]==1){
                if(nodeEval[tail]>=beta)
                    return nodeEval[tail];
            }
            else{
                if(nodeEval[tail]<=alpha)
                    return nodeEval[tail];
            }
        }
        else{
            if(nodeType[tail]==1){
                if(nodeEval[tail]>=beta)
                    return nodeEval[tail];
            }
            else{
                if(nodeEval[tail]<=alpha)
                    return nodeEval[tail];
            }
        }
    
        //try saved move
        /*
        int eval;
        makeMove(nodeMove[tail][0], nodeMove[tail][1], id_board[nodeMove[tail][2]][nodeMove[tail][3]], nodeMove[tail][4]);
        endOfTurnTriggers(side);    
        ++turn;
        if(side)
            eval = quiesence(alpha, beta, side^1);
        else
            eval = quiesence(alpha, beta, side^1);        
        --turn;
        unmakeMoves(turn);

        if(side==0){
            if(eval>=beta)
                return eval;
        }
        else{
            if(eval<=alpha)
                return eval;
        }

        best=eval;
        */
    }


    candidate_pointer[turn]=0;
    for(int i=0; i<pc_cnt; i++){
        if((pieces[i]&1)==side&&!death[i])
            generateMoves(i, px[i], py[i]);
    }


    int bm =-1;
    int res=-1;
    short cutAll;
    initSelectionSort();
    for(int j=0; j<candidate_pointer[turn]; j++){
        
        
        int i = selectionSort();

        if(selectionSortArray[turn][i]<agrobonus-3000){
            break;
        }

        selectionSortArray[turn][i]=-10*agrobonus;
        
        if(DEBUG)
            std::cout<<"QMOVE "<<candidateMoveStack[turn][i][0]<<" "<<candidateMoveStack[turn][i][1]<<" "<<candidateMoveStack[turn][i][2]<<" "<<candidateMoveStack[turn][i][3]<<std::endl;
        
        makeMove(candidateMoveStack[turn][i][0], candidateMoveStack[turn][i][1], candidateMoveStack[turn][i][2], candidateMoveStack[turn][i][3]);
        endOfTurnTriggers(side);
        
        if(DEBUG)
            printState();
        
        ++turn;

        if(side)
            res = quiesence(alpha, std::min(best, beta), side^1);
        else
            res = quiesence(std::max(alpha, best), beta, side^1);
    
        --turn;

        if(DEBUG){
            std::cout<<"Q UNMAKE "<<std::endl;
        }
        unmakeMoves(turn);
        if(DEBUG){
            printState();
        }

        if(!side){
            if(res>best)
                bm=i;
            best = std::max(best, res);
            if(best>=beta){
                cutAll=1;
                break;
            }
        }
        else{
            if(res<best){
                bm=i;
            }
            best = std::min(best, res);
            if(best<=alpha){
                cutAll=-1;
                break;
            }
        }
    }


    if(bm==-1){
        return static_evaluation(side);
    }

    zobrist[tail]=(hash^(side ? zblack : 0));
    nodeEval[tail]=best;
    nodeMove[tail][0]=candidateMoveStack[turn][bm][0];
    nodeMove[tail][1]=candidateMoveStack[turn][bm][1];
    nodeMove[tail][2]=px[candidateMoveStack[turn][bm][2]];
    nodeMove[tail][3]=py[candidateMoveStack[turn][bm][2]];
    nodeMove[tail][4]=candidateMoveStack[turn][bm][3];
    nodeType[tail]=cutAll;
    nodeDepth[tail]=0;

    return best;
}

const int null_move_reduction = 2;

int evaluate(int alpha, int beta, int mdepth, int side)
{
    if((hash^(side ? zblack : 0))==6036037833291321808)
        printState();

    if(mdepth>=3){
        if(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())>time_limit)
            throw "Exceeded time limit";
    }

    ++nodes;
    long long hashl=hash;
    int evall=static_evaluation(side);
    if (morale[0] <= 0)
        return morale[1] <= 0 ? 0 : -1000000;
    if (morale[1] <= 0)
        return 1000000;

    if (0 >= mdepth){
        if(DEBUG)
            std::cout<<"QUIESENSE BEGIN current state:"<<std::endl;
        int res = quiesence(alpha, beta, side);
        if(DEBUG)
            std::cout<<"QUIESENSE EXIT current state:"<<std::endl;
        return res;
    }

    int best = 1000000*(-1+2*side);

    int tail = (hash^(side ? zblack : 0))&hash_mask;
    if(zobrist[tail]==(hash^(side ? zblack : 0))){
        if(nodeDepth[tail]>=mdepth){
            if(nodeType[tail]==0)
                return nodeEval[tail];
            if(side==0){
                if(nodeType[tail]==1){
                    if(nodeEval[tail]>=beta)
                        return nodeEval[tail];
                }
                else{
                    if(nodeEval[tail]<=alpha)
                        return nodeEval[tail];
                }
            }
            else{
                if(nodeType[tail]==1){
                    if(nodeEval[tail]>=beta)
                        return nodeEval[tail];
                }
                else{
                    if(nodeEval[tail]<=alpha)
                        return nodeEval[tail];
                }
            }
        }
        
        if(DEBUG){
            std::cout<<"CACHED MOVE:"<<std::endl;
            std::cout<<nodeMove[tail][0]<<" "<<nodeMove[tail][1]<<" "<<id_board[nodeMove[tail][2]][nodeMove[tail][3]]<<" "<<nodeMove[tail][4]<<std::endl;
            std::cout<<tail<<" "<<zobrist[tail]<<" "<<side<<" "<<(hash^zblack)<<std::endl;
            printState();
        }

        //try saved move
        int eval;
        if(id_board[nodeMove[tail][2]][nodeMove[tail][3]]!=-1)
            makeMove(nodeMove[tail][0], nodeMove[tail][1], id_board[nodeMove[tail][2]][nodeMove[tail][3]], nodeMove[tail][4]);
        endOfTurnTriggers(side);    
        ++turn;
        try{
            if(side)
                eval = evaluate(alpha, beta, mdepth-1, side^1);
            else
                eval = evaluate(alpha, beta, mdepth-1, side^1);
        }
        catch(...){
            --turn; 
            unmakeMoves(turn);
            throw;
        }
        
        --turn;
        unmakeMoves(turn);
        if(static_evaluation(side)!=evall)
            throw std::logic_error("unmake failed");
        if(side==0){
            if(eval>=beta)
                return eval;
        }
        else{
            if(eval<=alpha)
                return eval;
        }

        best=eval;
    }

    candidate_pointer[turn]=0;
    for(int i=0; i<pc_cnt; i++){
        if((pieces[i]&1)==side&&!death[i])
            generateMoves(i, px[i], py[i]);
    }


    //null move
    //std::cout<<"NMR"<<std::endl;
    turn++;
    int null_move_value = evaluate(alpha, beta, mdepth-1-null_move_reduction, side^1);
    turn--;
    if(side==0&&null_move_value>=beta)
        return null_move_value;
    if(side==1&&null_move_value<=alpha)
        return null_move_value;
    
    int bm =-1;
    int res=-1;
    short cutAll=0;

    initSelectionSort();
    for(int j=0; j<candidate_pointer[turn]; j++){
        int i = selectionSort();
        selectionSortArray[turn][i]=-10*agrobonus;

        if(DEBUG){
            std::cout<<"{"<<std::endl;
            std::cout<<"DEPTH "<<turn<<" MOVE: "<<j<<"/"<<candidate_pointer[turn]<<std::endl;
            std::cout<<candidateMoveStack[turn][i][0]<<" "<<candidateMoveStack[turn][i][1]<<" "<<candidateMoveStack[turn][i][2]<<" "<<candidateMoveStack[turn][i][3]<<std::endl;
            printState();
        }

        if(turn==start_turn && mdepth>=12){
            std::cout<<candidateMoveStack[turn][i][0]<<" "<<candidateMoveStack[turn][i][1]<<" "<<candidateMoveStack[turn][i][2]<<" "<<candidateMoveStack[turn][i][3]<<std::endl;
        }

        makeMove(candidateMoveStack[turn][i][0], candidateMoveStack[turn][i][1], candidateMoveStack[turn][i][2], candidateMoveStack[turn][i][3]);
        endOfTurnTriggers(side);    

        ++turn;
        try{
            if(side){
                res = evaluate(alpha, std::min(best, beta), mdepth-1-(j>2&&depth>2 ? 1 : 0), side^1);
                if(j>2&&depth>2&&res<best)
                    res = evaluate(alpha, std::min(best, beta), mdepth-1, side^1);
            }
            else{
                res = evaluate(std::max(alpha, best), beta, mdepth-1-(j>2&&depth>2 ? 1 : 0), side^1);
                if(j>2&&depth>2&&res>best)
                    res = evaluate(std::max(alpha, best), beta, mdepth-1, side^1);
            }
        }
        catch(...){
            --turn;
            unmakeMoves(turn);
            throw;
        }
        --turn;
        if(DEBUG){
            std::cout<<"}"<<std::endl;
            std::cout<<"UNMAKE"<<std::endl;
        }
        unmakeMoves(turn);
            
        if(DEBUG)
            printState();  
          
        /*if(static_evaluation(side)!=evall)
            throw std::logic_error("unmake failed");*/

        if(hash!=hashl){
            printState();
            throw std::logic_error("hash invalidated");
        }


        if(side==0){
            if(res>best)
                bm=i;
            best = std::max(best, res);
            if(best>=beta){
                cutAll=1;
                break;
            }
        }
        else{
            if(res<best)
                bm=i;
            best = std::min(best, res);
            if(best<=alpha){
                cutAll=-1;
                break;
            }
        }
    }

    if(turn==start_turn)
    {
        if(bm==-1)
        {
            move_chosen[0]=nodeMove[tail][0];
            move_chosen[1]=nodeMove[tail][1];
            move_chosen[2]=id_board[nodeMove[tail][2]][nodeMove[tail][3]];
            move_chosen[3]=nodeMove[tail][4];
        }
        else{
            move_chosen[0]=candidateMoveStack[turn][bm][0];
            move_chosen[1]=candidateMoveStack[turn][bm][1];
            move_chosen[2]=candidateMoveStack[turn][bm][2];
            move_chosen[3]=candidateMoveStack[turn][bm][3];
        }
    }

    zobrist[tail]=(hash^(side ? zblack : 0));
    nodeEval[tail]=best;
    if(bm!=-1){
        nodeMove[tail][0]=candidateMoveStack[turn][bm][0];
        nodeMove[tail][1]=candidateMoveStack[turn][bm][1];
        nodeMove[tail][2]=px[candidateMoveStack[turn][bm][2]];
        nodeMove[tail][3]=py[candidateMoveStack[turn][bm][2]];
        nodeMove[tail][4]=candidateMoveStack[turn][bm][3];
        nodeType[tail]=cutAll;
    }
    nodeDepth[tail]=mdepth;


    return best;
}

void makeChosenMove(){
    makeMove(move_chosen[0], move_chosen[1], move_chosen[2], move_chosen[3]);
}

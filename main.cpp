#include <stdio.h>
#include <string.h>
#include <bits/stdc++.h>
#include <random>

const bool DEBUG = false;

const int hash_table_size = 67108864; //2^26
const long long hash_mask = hash_table_size-1;
long long zblack;

long long zobrist[hash_table_size] = {-1};
int nodeEval[hash_table_size] = {}; 
short nodeMove[hash_table_size][5] = {}; 
short nodeType[hash_table_size] = {}; //0 = normal, 1 = lower bound, -1 = upperbound
short nodeDepth[hash_table_size] = {};


//(0, 0) is bottom left corner of board
int board[8][8] = {}; //included in hash
int id_board[8][8]; //not

long long zboard[8][8][1200] = {};

int lightning[8][8] = {}; //yes
int lightning_cnt=0;
int butterfly[8][8] = {}; //yes
int butterfly_cnt=0;
int meteor[8][8] = {}; //yes
int meteor_cnt=0;

long long zlightning[8][8] = {};
long long zmeteor[8][8] = {};

int doveList[2][20] = {};
int dovePnt[2]={};

int samuraiList[2][20] = {};
int samuraiPnt[2]={0,0};
int lustList[2][20] = {};
int lustPnt[2]={0,0};
int alchList[2][20]={};
int alchPnt[2]={};

int pieces[200] = {0}; //not
int death[200] = {0}; //not
long long status[200] = {0}; //yes
bool voided[200] = {}; //yes
bool nulled[200] = {}; //yes
bool transparent[200] = {}; //no

long long zvoided[200] = {};
long long znulled[200] = {};
long long zmorale[2][250] = {}; //starts at -50

int moved[200] = {0}; //no
int px[200] = {0}; //no
int py[200] = {0}; //no
int pmorale[200]={0}; //no
int pc_cnt = 0;
int m_depth = 1;
int turn = 0;

int king_id[2] = {-1, -1};

long long hash=0;

//first value is a spacer! 
bool freeze_immune[1050] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
bool petrify_immune[1050] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
bool poison_immune[1050] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
bool siren_immune[1050] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1};
bool displacement_immune[1050] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1};

//first value is a spacer! 
bool isMinion[1050] = {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool isChampion[1050] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0};
int pieceValue[1050] = {0,0,1,5,9,0,1,2,3,1,2,3,4,8,9,10,11,0,1,3,5,10,11,12,13,1,2,3,4,5,6,7,8,0,1,2,4,0,1,2,4,2,3,4,5,0,1,2,3,5,7,10,14,3,4,6,7,6,7,8,9,0,1,3,6,6,8,10,11,1,1,1,1,0,1,2,3,0,1,2,4,5,8,10,13,0,1,2,3,2,3,4,6,1,2,4,5,1,2,5,6,0,1,2,3,1,2,4,5,1,3,5,7,7,8,10,11,7,8,9,10,8,9,10,15,8,10,12,14,2,3,4,5,8,9,11,14,8,12,16,20,8,12,16,20,0,1,2,3,4,5,6,7,4,7,10,13,4,7,10,13,5,6,8,10,2,3,4,5,5,7,9,12,3,4,5,8,0,1,2,3,5,8,9,10,9,11,13,14,9,10,11,12,7,10,12,14,0,2,4,6,9,12,14,15,3,4,5,6,2,4,6,8,0,1,2,3,9,10,12,14,0,0,0,0,10,11,13,16,6,8,13,16,12,13,14,15,3,4,5,6,0,1,2,4,5,9,12,15,0,1,3,5,2,4,6,7,4,7,9,10,4,6,9,12,4,5,6,7,7,9,11,13,15,18,21,24,2,3,4,5,9,11,16,21,0,2,3,5,5,8,10,12,10,12,16,18,15,18,21,24,6,8,11,13,8,9,10,11,12,13,14,16,8,11,13,15,8,11,13,15,4,5,6,7,9,11,13,17,1,2,4,6,12,14,15,18,13,14,16,18,4,7,9,12,7,10,12,14,13,16,18,21,13,16,18,21,21,24,28,30,3,4,5,6,0,3,4,5,0,2,3,4,4,5,6,7,10,12,14,16,0,2,4,6,0,1,2,4,8,10,12,14,13,14,15,16,15,17,19,24,7,10,12,13,6,9,12,16,6,7,8,9,4,7,10,12,4,6,8,10,15,18,20,21,16,21,25,28,15,16,17,18,15,16,17,18,5,6,9,12,4,5,8,9,3,2,1,0,7,10,12,14,2,3,4,5,4,6,8,10,4,5,6,8,5,8,9,10,12,14,16,20,14,17,20,24,0,5,10,16,6,10,14,18,2,3,5,6,4,6,7,9,11,14,20,22,5,6,8,10,16,19,22,25,6,9,14,16,6,8,12,15,6,8,9,10,0,0,3,1,1};
int promotions[1050] = {0,0,0,0,0,448,448,448,448,248,248,248,248,0,0,0,0,968,968,968,968,0,0,0,0,0,0,0,0,0,0,0,0,240,240,240,240,232,232,232,232,0,0,0,0,432,432,432,432,8,8,8,8,8,8,8,8,8,8,8,8,576,576,576,576,0,0,0,0,-8,-8,-8,-8,-104,-104,-104,-104,0,0,0,0,0,0,0,0,-56,-56,-56,-56,6,4,2,0,0,0,0,0,176,176,176,176,48,48,48,48,16,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,336,336,336,336,0,0,0,0,0,0,0,0,0,0,0,0,504,504,504,504,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,272,272,272,272,0,0,0,0,0,0,0,0,0,0,0,0,200,200,200,200,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,112,112,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,248,248,248,248,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int depth = 0;
int morale[2] = {0};
int position_bonus[2] = {};

int piece_type[1050] = {100,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,2,2,2,2,3,3,3,3,2,2,2,2,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,3,3,3,3,1,1,1,1,3,3,3,3,1,1,1,1,3,3,3,3,2,2,2,2,2,2,2,2,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,5,6,7,8,1,1,1,1,1,1,1,1,3,3,3,3,5,5,6,7,3,3,3,3,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,3,3,3,3,1,1,1,1,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,5,6,6,7,2,2,2,2,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,7,8,8,3,3,3,3,1,1,1,1,2,2,2,2,1,1,1,1,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0};

int piece_square_tables[11][8][8] =
 {    
    {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    },  
    {
        {10,30,50,60,60,50,30,10},
        {20,50,100,120,120,100,50,20},
        {20,70,90,100,100,90,70,20},
        {20,40,80,100,100,80,40,20},
        {20,40,60,100,100,60,40,20},
        {10,20,40,50,50,40,20,10},
        {5,10,20,30,30,20,10,5},
        {0,5,10,20,20,10,5,0}
    },

    {
        {0,0,0,0,0,0,0,0},
        {150,150,150,150,150,150,150,150},
        {80,90,100,100,100,100,90,80},
        {40,60,70,80,80,70,60,40},
        {10,30,50,60,60,50,30,10},
        {-10,10,30,40,40,30,10,-10},
        {-30,-10,0,10,10,0,-10,-30},
        {-50,-30,-20,-10,-10,-20,-30,-50}
    },

    {
        {0,10,30,40,40,30,10,0},
        {10,20,40,50,50,40,20,10},
        {20,30,60,80,80,60,30,20},
        {30,40,80,100,100,80,40,30},
        {30,40,60,80,80,60,40,30},
        {20,30,50,60,60,50,30,20},
        {10,20,40,50,50,40,20,10},
        {0,10,20,40,40,20,10,0}
    },

    {
        {0,0,0,0,0,0,0,0},
        {-40,-40,-40,-40,-40,-40,-40,-40},
        {-100,-100,-100,-100,-100,-100,-100,-100},
        {-100,-100,-100,-100,-100,-100,-100,-100},
        {-80,-80,-80,-80,-80,-80,-80,-80},
        {-60,-60,-60,-60,-60,-60,-60,-60},
        {-20,-20,-20,-20,-20,-20,-20,-20},
        {-30,-30,-30,-30,-30,-30,-30,-30},
    },

    {
        {-100,-90,-70,-60,-60,-70,-90,-100},
        {-90,-80,-60,-50,-50,-60,-80,-90},
        {-80,-70,-40,-20,-20,-40,-70,-80},
        {-70,-60,-20,0,0,-20,-60,-70},
        {-70,-60,-40,-20,-20,-40,-60,-70},
        {-80,-70,-50,-40,-40,-50,-70,-80},
        {-90,-80,-60,-50,-50,-60,-80,-90},
        {-100,-90,-80,-60,-60,-80,-90,-100}
    },

    {
        {-200,-190,-170,-160,-160,-170,-190,-200},
        {-190,-180,-160,-150,-150,-160,-180,-190},
        {-180,-170,-140,-120,-120,-140,-170,-180},
        {-170,-160,-120,-100,-100,-120,-160,-170},
        {-170,-160,-140,-120,-120,-140,-160,-170},
        {-180,-170,-150,-140,-140,-150,-170,-180},
        {-190,-180,-160,-150,-150,-160,-180,-190},
        {-200,-190,-180,-160,-160,-180,-190,-200}
    },

    {
        {-300,-290,-270,-260,-260,-270,-290,-300},
        {-290,-280,-260,-250,-250,-260,-280,-290},
        {-280,-270,-240,-220,-220,-240,-270,-280},
        {-270,-260,-220,-200,-200,-220,-260,-270},
        {-270,-260,-240,-220,-220,-240,-260,-270},
        {-280,-270,-250,-240,-240,-250,-270,-280},
        {-290,-280,-260,-250,-250,-260,-280,-290},
        {-300,-290,-280,-260,-260,-280,-290,-300},
    },
        
    {
        {-400,-390,-370,-360,-360,-370,-390,-400},
        {-390,-380,-360,-350,-350,-360,-380,-390},
        {-380,-370,-340,-320,-320,-340,-370,-380},
        {-370,-360,-320,-300,-300,-320,-360,-370},
        {-370,-360,-340,-320,-320,-340,-360,-370},
        {-380,-370,-350,-340,-340,-350,-370,-380},
        {-390,-380,-360,-350,-350,-360,-380,-390},
        {-400,-390,-380,-360,-360,-380,-390,-400},
    },

    {
        {-500,-490,-470,-460,-460,-470,-490,-500},
        {-490,-480,-460,-450,-450,-460,-480,-490},
        {-480,-470,-440,-420,-420,-440,-470,-480},
        {-470,-460,-420,-400,-400,-420,-460,-470},
        {-470,-460,-440,-420,-420,-440,-460,-470},
        {-480,-470,-450,-440,-440,-450,-470,-480},
        {-490,-480,-460,-450,-450,-460,-480,-490},
        {-500,-490,-480,-460,-460,-480,-490,-500},
    },
    
    {
        {-600,-590,-570,-560,-560,-570,-590,-600},
        {-590,-580,-560,-550,-550,-560,-580,-590},
        {-580,-570,-540,-520,-520,-540,-570,-580},
        {-570,-560,-520,-500,-500,-520,-560,-570},
        {-570,-560,-540,-520,-520,-540,-560,-570},
        {-580,-570,-550,-540,-540,-550,-570,-580},
        {-590,-580,-560,-550,-550,-560,-580,-590},
        {-600,-590,-580,-560,-560,-580,-590,-600}
    }
};

long long zstatus_id[200] = {};
long long status_prime = 1000000007LL;
long long zstatus(int pc_id){
    if(DEBUG)
        std::cout<<"STATUS HASH: "<<((status[pc_id])%status_prime)*zstatus_id[pc_id]<<"Piece: "<<pc_id<<" STATUS VALUE: "<<status[pc_id]<<std::endl;
    return ((status[pc_id])%status_prime)*zstatus_id[pc_id];
}

const int MAX_DEPTH = 150;

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

    zblack=std::abs((long long)rnd());;
}

int candidateMoveStack[150][400][4] = {0};
int candidate_pointer[150] = {};

void addMove(int xx, int yy, int pc_id, int nMoveType)
{
    candidateMoveStack[turn][candidate_pointer[turn]][0] = xx;
    candidateMoveStack[turn][candidate_pointer[turn]][1] = yy;
    candidateMoveStack[turn][candidate_pointer[turn]][2] = pc_id;
    candidateMoveStack[turn][candidate_pointer[turn]++][3] = nMoveType;
}

bool isPoisoned(int pc_id)
{
    return (status[pc_id] & ((1LL << 7) - 1)) > 0;
}
bool isFrozen(int pc_id)
{
    return (status[pc_id] & ((1LL << 14) - (1LL << 7))) > 0;
}
bool isCompeled(int pc_id)
{
    return (status[pc_id] & ((1LL << 21) - (1LL << 14))) > 0;
}
bool isPetrified(int pc_id)
{
    return (status[pc_id] & ((1LL << 32) - (1LL << 21))) > 0;
}
bool isEnchanted(int pc_id)
{
    return (status[pc_id] & ((1LL << 38) - (1LL << 32))) > 0;
}

void addCandidateMove(int x, int y, int xx, int yy, int piece, int pc_id, int moveType)
{
    //std::cout<<"moveGen "<<xx<<" "<<yy<<" "<<piece<<" "<<moveType<<std::endl;
    //std::cout<<x<<" "<<y<<" "<<board[xx][yy]<<std::endl;

    piece=pieces[pc_id];

    //behemoth check
    if(isMinion[piece/2]&&board[xx][yy]>=562&&board[xx][yy]<570&&((piece^board[xx][yy])&1)){
        return;
    }

    //melee king kill check
    if(isEnchanted(pc_id)&&king_id[(piece&1)^1]==id_board[xx][yy])
        if(moveType == 3 || moveType == 8 || moveType == 11 || moveType==4 || moveType==19 || moveType == 13 || moveType == 14 || moveType == 42)
            return;

    switch (moveType)
    {
    case 1: // move
    case 9: // teleport
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        break;
    case 2:  // move from starting position
    case 44: // teleport from starting position
        if (!moved[pc_id] && board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        break;
    case 3:  // move or attack
    case 8:  // path (not used, path uses 32)
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        else if (((pieces[pc_id] ^ board[xx][yy]) & 1) && !isEnchanted(id_board[xx][yy])){
            addMove(xx, yy, pc_id, 32);
        }
        break;
    case 11: // unblockable move or attack
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        else if (((pieces[pc_id] ^ board[xx][yy]) & 1) && !isEnchanted(id_board[xx][yy])){
            addMove(xx, yy, pc_id, 2);
        }
        break;
    case 4:  // attack only
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) && !isEnchanted(id_board[xx][yy]))
            addMove(xx, yy, pc_id, 32);
        break;
    case 19: // unblockable attack only
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) && !isEnchanted(id_board[xx][yy]))
            addMove(xx, yy, pc_id, 2);
        break;
    case 5:  // ranged destroy
    case 46: // magic destroy
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) )
            addMove(xx, yy, pc_id, 14);
        break;
    case 6: // wind 
        if (board[xx][yy] != 0 && !displacement_immune[board[xx][yy]/2])
        {
            int dx = xx - x;
            int dy = yy - y;
            dx = dx < 0 ? -1 : dx > 0 ? 1
                                      : 0;
            dy = dy < 0 ? -1 : dy > 0 ? 1
                                      : 0;
            for(int i=1; i<=3; i++){
                if(xx+dx*i<0||xx+dx*i>7||yy+dy*i<0||yy+dy*i>7)
                    break;
                if(board[xx+dx*i][yy+dy*i]==0){
                    addMove(xx, yy, pc_id, 13);
                    break;
                }
                else{
                    if(board[xx+dx*i][yy+dy*i]&2024)
                        continue;
                    else
                        break;
                }
            }
        }
        break;
    case 7: // ranged petrify
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && (!petrify_immune[board[xx][yy]/2]))
            if (!(piece >= 346 && piece < 354) || !isPetrified(id_board[xx][yy]))
                addMove(xx, yy, pc_id, 6);
        break;
    case 10: // move or swap
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        else if (((pieces[pc_id] ^ board[xx][yy]) & 1) == 0 && !displacement_immune[board[xx][yy]/2])
            addMove(xx, yy, pc_id, 3);
        break;
    case 12: // tree
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 22);
        else if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1)
            addMove(xx, yy, pc_id, 15);
        break;
    case 13: // attack, move or swap
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        else if (((pieces[pc_id] ^ board[xx][yy]) & 1)){
            if(!isEnchanted(id_board[xx][yy]))
                addMove(xx, yy, pc_id, 2);
        }
        else
            addMove(xx, yy, pc_id, 3);
        break;
    case 14: // jump attack (not implemented)
        break;
    case 15: // freeze
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && (!freeze_immune[board[xx][yy]/2]))
            addMove(xx, yy, pc_id, 5);
        break;
    case 16: // poison 
        if(board[xx][yy]!=0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && (!poison_immune[board[xx][yy]/2]) && !isPoisoned(id_board[xx][yy]))
            addMove(xx, yy, pc_id, 7);
        break;
    case 17: // attack minion
        if (board[xx][yy] != 0 && isMinion[board[xx][yy]/2] && ((pieces[pc_id] ^ board[xx][yy]) & 1) && !isEnchanted(id_board[xx][yy]))
            addMove(xx, yy, pc_id, 2);
        break;
    case 18: // freeze push
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1)
            addMove(xx, yy, pc_id, 12);
        break;
    case 20: // necromagic
        if (board[xx][yy] != 0 && isMinion[board[xx][yy]/2] && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1)
            addMove(xx, yy, pc_id, 16);
        else if (board[xx][yy] != 0 && board[xx][yy] >= 178 && board[xx][yy] <= 185) //no ally check needed here because of the other if
            addMove(xx, yy, pc_id, 31);
    case 21: // nexus (not implemented)
        break;
    case 22: // portal (not implemented)
        break;
    case 23: // summon pillar or petrify
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 23);
        else if (((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && (!petrify_immune[board[xx][yy]/2]))
            addMove(xx, yy, pc_id, 6);
        break;
    case 24: // lightning
        if (lightning[xx][yy] == 0)
            addMove(xx, yy, pc_id, 18);
        break;
    case 25: // butterfly
        if (butterfly[xx][yy] == 0)
            addMove(xx, yy, pc_id, 20);
        break;
    case 26: // lifestone (not implemented)
        break;
    case 27: // null 
        if(board[xx][yy] != 0 &&  ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && !nulled[id_board[xx][yy]])
            addMove(xx, yy, pc_id, 9);
        break;
    case 28: // charm
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && isMinion[board[xx][yy]/2])
            addMove(xx, yy, pc_id, 29);
        break;
    case 29: // summoner (not implemented)
        break;
    case 30: // summon skeleton
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 21);
        break;
    case 31: // summon king (valk)
        if (board[xx][yy] == 0 && king_id[pieces[pc_id] & 1] >= 0 && death[king_id[pieces[pc_id] & 1]] == 0)
            addMove(xx, yy, king_id[pieces[pc_id] & 1], 1);
        break;
    case 32: // aquarius suicide (not implemented)
        break;
    case 33: // comet suicide (not implemented)
        break;
    case 34: // enchant
        if(board[xx][yy]!=0 && ((board[xx][yy]^pieces[pc_id]) & 1)==0)
            addMove(xx, yy, pc_id, 11);
        break;
    case 35: // transform into bat
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 25);
        break;
    case 36: // gravity
        addMove(xx, yy, pc_id, 27);
        break;
    case 37: // omniswap
        if (board[xx][yy] == 0)
            addMove(xx, yy, pc_id, 1);
        else if(!displacement_immune[board[xx][yy]/2])
            addMove(xx, yy, pc_id, 3);
        break;
    case 38: // NA
        break;
    case 39: // augmented teleport (not implemented)
        break;
    case 40: // siren (not implemented yet)
        if(!siren_immune[board[xx][yy]/2])
            addMove(xx, yy, pc_id, 10);
    case 41: // ghost
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1)
            addMove(xx, yy, pc_id, 17);
        break;
    case 42: // rush (implement displacement immune)
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1 && !displacement_immune[board[xx][yy]])
            addMove(xx, yy, pc_id, 30);
        break;
    case 43: // void (not implemented yet)
        break;
    case 45: // meteor
        if (meteor[xx][yy] == 0)
            addMove(xx, yy, pc_id, 19);
        break;
    case 47: // envy
        if (board[xx][yy] != 0 && ((pieces[pc_id] ^ board[xx][yy]) & 1) == 1)
            addMove(xx, yy, pc_id, 26);
    }
}

void rookBackward(int range, int moveType, int piece, int pc_id, int x, int y, int start);

void rookForward(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    if ((piece & 1) == 1)
    {
        rookBackward(range, moveType, piece - 1, pc_id, x, y, start);
        return;
    }
    //std::cout<<"rookForward "<<std::endl;

    for (int i = y + 1; i <= std::min(y + range, 7); i++)
    {
        if (i - y >= start)
            addCandidateMove(x, y, x, i, piece, pc_id, moveType);
        if (moveType < 9 && board[x][i] != 0)
        {
            if (!transparent[id_board[x][i]])
                break;
            else
                continue;
        }
    }
}

void rookBackward(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    if ((piece & 1) == 1)
    {
        rookForward(range, moveType, piece - 1, pc_id, x, y, start);
        return;
    }
    //std::cout<<"rookBackward "<<std::endl;
    for (int i = y - 1; i >= std::max(y - range, 0); i--)
    {
        if (y - i >= start)
            addCandidateMove(x, y, x, i, piece, pc_id, moveType);
        if (moveType < 9 && board[x][i] != 0)
        {
            if (!transparent[id_board[x][i]])
                break;
            else
                continue;
        }
    }
}

void rookHorizontal(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    //std::cout<<"rookHorizontal "<<std::endl;
    for (int i = x + 1; i <= std::min(x + range, 7); i++)
    {
        if (i - x >= start)
            addCandidateMove(x, y, i, y, piece, pc_id, moveType);

        if (moveType < 9 && board[i][y] != 0)
        {
            if (!transparent[id_board[i][y]])
                break;
            else
                continue;
        }
    }

    for (int i = x - 1; i >= std::max(x - range, 0); i--)
    {
        if (x - i >= start)
            addCandidateMove(x, y, i, y, piece, pc_id, moveType);
        if (moveType < 9 && board[i][y] != 0)
        {
            if (!transparent[id_board[i][y]])
                break;
            else
                continue;
        }
    }
}


void rookVertical(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    rookForward(range, moveType, piece, pc_id, x, y, start);
    rookBackward(range, moveType, piece, pc_id, x, y, start);
}

void rook(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    rookForward(range, moveType, piece, pc_id, x, y, start);
    rookHorizontal(range, moveType, piece, pc_id, x, y, start);
    rookBackward(range, moveType, piece, pc_id, x, y, start);
}



void bishopBackward(int range, int moveType, int piece, int pc_id, int x, int y, int start);

void bishopForward(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    if ((piece & 1) == 1)
    {
        bishopBackward(range, moveType, piece - 1, pc_id, x, y, start);
        return;
    }
    // ne
    for (int i = 1; i <= std::min(range, std::min(7 - x, 7 - y)); i++)
    {
        if (i >= start)
            addCandidateMove(x, y, x + i, y + i, piece, pc_id, moveType);
        if (moveType < 9 && board[x + i][y + i] != 0)
        {
            if (!transparent[id_board[x + i][y + i]])
                break;
            else
                continue;
        }
    }
    // nw
    for (int i = 1; i <= std::min(range, std::min(x, 7 - y)); i++)
    {
        if (i >= start)
            addCandidateMove(x, y, x - i, y + i, piece, pc_id, moveType);
        if (board[x - i][y + i] != 0)
        {
            if (moveType < 9 && !transparent[id_board[x - i][y + i]])
                break;
            else
                continue;
        }
    }
}

void bishopBackward(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    if ((piece & 1) == 1)
    {
        bishopForward(range, moveType, piece - 1, pc_id, x, y, start);
        return;
    }
    // se
    for (int i = 1; i <= std::min(range, std::min(7 - x, y)); i++)
    {
        if (i >= start)
            addCandidateMove(x, y, x + i, y - i, piece, pc_id, moveType);
        if (board[x + i][y - i] != 0)
        {
            if (!transparent[id_board[x + i][y - i]])
                break;
            else
                continue;
        }
    }

    // sw
    for (int i = 1; i <= std::min(range, std::min(x, y)); i++)
    {
        if (i >= start)
            addCandidateMove(x, y, x - i, y - i, piece, pc_id, moveType);
        if (moveType < 9 && board[x - i][y - i] != 0)
        {
            if (!transparent[id_board[x - i][y - i]])
                break;
            else
                continue;
        }
    }
}

void bishop(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    bishopForward(range, moveType, piece, pc_id, x, y, start);
    bishopBackward(range, moveType, piece, pc_id, x, y, start);
}

void militia(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    bishopForward(range, moveType, piece, pc_id, x, y, start);
    rookForward(range, moveType, piece, pc_id, x, y, start);
}

void queen(int range, int moveType, int piece, int pc_id, int x, int y, int start = 1)
{
    //std::cout<<"queen entered "<<std::endl;
    rook(range, moveType, piece, pc_id, x, y, start);
    bishop(range, moveType, piece, pc_id, x, y, start);
}

int knight_directions[8][2] = {{1, 2}, {-1, 2}, {-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {-1, -2}, {1, -2}};

void knight(int moveType, int piece, int pc_id, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
    }
}

void doubleKnight(int moveType, int piece, int pc_id, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        if (x + 2 * knight_directions[i][0] >= 0 && x + 2 * knight_directions[i][0] <= 7 && y + 2 * knight_directions[i][1] >= 0 && y + 2 * knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + 2 * knight_directions[i][0], y + 2 * knight_directions[i][1], piece, pc_id, moveType);
    }
}

void knightBackward(int moveType, int piece, int pc_id, int x, int y);

void knightForward(int moveType, int piece, int pc_id, int x, int y)
{
    if ((piece & 1) == 1)
    {
        knightBackward(moveType, piece - 1, pc_id, x, y);
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
    }
}

void knightBackward(int moveType, int piece, int pc_id, int x, int y)
{
    if ((piece & 1) == 1)
    {
        knightForward(moveType, piece - 1, pc_id, x, y);
        return;
    }
    for (int i = 4; i < 8; i++)
    {
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
    }
}

void knightOnlyBackward(int moveType, int piece, int pc_id, int x, int y);

void knightOnlyForward(int moveType, int piece, int pc_id, int x, int y){
    if ((piece & 1) == 1)
    {
        knightOnlyBackward(moveType, piece - 1, pc_id, x, y);
        return;
    }
    for (int i = 0; i < 2; i++)
    {
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
    }
}

void knightOnlyBackward(int moveType, int piece, int pc_id, int x, int y){
    if ((piece & 1) == 1)
    {
        knightOnlyForward(moveType, piece - 1, pc_id, x, y);
        return;
    }
    for (int i = 6; i < 8; i++)
    {
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
    }
}

void knightHorizontal(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 2; i < 6; i++)
        if (x + knight_directions[i][0] >= 0 && x + knight_directions[i][0] <= 7 && y + knight_directions[i][1] >= 0 && y + knight_directions[i][1] <= 7)
            addCandidateMove(x, y, x + knight_directions[i][0], y + knight_directions[i][1], piece, pc_id, moveType);
}

int valk_directions[8][2] = {{1, 3}, {-1, 3}, {1, -3}, {-1, -3}, {-3, -1}, {3, 1}, {3, -1}, {-3, 1}};
void valk(int moveType, int piece, int pc_id, int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        if (x + valk_directions[i][0] >= 0 && x + valk_directions[i][0] <= 7 && y + valk_directions[i][1] >= 0 && y + valk_directions[i][1] <= 7)
            addCandidateMove(x, y, x + valk_directions[i][0], y + valk_directions[i][1], piece, pc_id, moveType);
    }
}

void valkVertical(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 0; i < 4; i++)
    {
        if (x + valk_directions[i][0] >= 0 && x + valk_directions[i][0] <= 7 && y + valk_directions[i][1] >= 0 && y + valk_directions[i][1] <= 7)
            addCandidateMove(x, y, x + valk_directions[i][0], y + valk_directions[i][1], piece, pc_id, moveType);
    }
}

void valkHorizontal(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 4; i < 8; i++)
    {
        if (x + valk_directions[i][0] >= 0 && x + valk_directions[i][0] <= 7 && y + valk_directions[i][1] >= 0 && y + valk_directions[i][1] <= 7)
            addCandidateMove(x, y, x + valk_directions[i][0], y + valk_directions[i][1], piece, pc_id, moveType);
    }
}

int mancer_directions[8][2] = {{2, 3}, {-2, 3}, {3, 2}, {-3, 2}, {3, -2}, {-3, -2}, {2, -3}, {-2, -3}};
void mancer(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 0; i < 8; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void mancerOnlyBackward(int moveType, int piece, int pc_id, int x, int y);

void mancerOnlyForward(int moveType, int piece, int pc_id, int x, int y){
    if((piece&1)==1)
        return mancerOnlyBackward(moveType, piece-1, pc_id, x, y);
    for (int i = 0; i < 2; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void mancerOnlyBackward(int moveType, int piece, int pc_id, int x, int y){
    if((piece&1)==1)
        return mancerOnlyForward(moveType, piece-1, pc_id, x, y);
    for (int i = 6; i < 8; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void mancerBackward(int moveType, int piece, int pc_id, int x, int y);

void mancerForward(int moveType, int piece, int pc_id, int x, int y){
    if((piece&1)==1)
        return mancerBackward(moveType, piece-1, pc_id, x, y);
    for (int i = 0; i < 4; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void mancerBackward(int moveType, int piece, int pc_id, int x, int y){
    if((piece&1)==1)
        return mancerForward(moveType, piece-1, pc_id, x, y);
    for (int i = 4; i < 8; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void mancerHorizonal(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 2; i < 6; i++)
    {
        if (x + mancer_directions[i][0] >= 0 && x + mancer_directions[i][0] <= 7 && y + mancer_directions[i][1] >= 0 && y + mancer_directions[i][1] <= 7)
            addCandidateMove(x, y, x + mancer_directions[i][0], y + mancer_directions[i][1], piece, pc_id, moveType);
    }
}

void bomberBackward(int range, int moveType, int piece, int pc_id, int x, int y);
void bomberForward(int range, int moveType, int piece, int pc_id, int x, int y){
    if(piece&1)
        return bomberBackward(range, moveType, piece-1, pc_id, x, y);
    if(y!=0)
        return;
    for(int i=-range; i<=range; i++){
        if(i==0)
            continue;
        addCandidateMove(x, y, x+i, 7, piece, pc_id, moveType);
    }
}

void bomberBackward(int range, int moveType, int piece, int pc_id, int x, int y){
    if(piece&1)
        return bomberForward(range, moveType, piece-1, pc_id, x, y);
    if(y!=7)
        return;
    for(int i=-range; i<=range; i++){
        if(i==0)
            continue;
        addCandidateMove(x, y, x+i, 0, piece, pc_id, moveType);
    }
}

void fortress(int range, int piece, int pc_id, int x, int y){
    if(x+1<8&&y+1<8&&board[x+1][y+1]==0){
        addCandidateMove(x, y, x+1, y+1, piece, pc_id, 1);

        for(int xx=2; xx<=std::min(7-x, range); xx++){
            addCandidateMove(x,y,x+xx, y+1, piece, pc_id, 4);
            if(board[x+xx][y+1]==0 || transparent[id_board[x+xx][y+1]])
                continue;
            else
                break;
        }

        for(int yy=2; yy<=std::min(7-y, range); yy++){
            addCandidateMove(x,y,x+1, y+yy, piece, pc_id, 4);
            if(board[x+1][y+yy]==0 || transparent[id_board[x+1][y+yy]])
                continue;
            else
                break;
        }
    }

    if(x+1<8&&y-1>=0&&board[x+1][y-1]==0){
        addCandidateMove(x, y, x+1, y-1, piece, pc_id, 1);
        
        for(int xx=2; xx<=std::min(7-x, range); xx++){
            addCandidateMove(x,y,x+xx, y-1, piece, pc_id, 4);
            if(board[x+xx][y-1]==0 || transparent[id_board[x+xx][y-1]])
                continue;
            else
                break;
        }

        for(int yy=2; yy<=std::min(y, range); yy++){
            addCandidateMove(x,y,x+1, y-yy, piece, pc_id, 4);
            if(board[x+1][y-yy]==0 || transparent[id_board[x+1][y-yy]])
                continue;
            else
                break;
        }
    }

    if(x-1>=0&&y-1>=0&&board[x-1][y-1]==0){
        addCandidateMove(x, y, x-1, y-1, piece, pc_id, 1);
        
        for(int xx=2; xx<=std::min(x, range); xx++){
            addCandidateMove(x,y,x-xx, y-1, piece, pc_id, 4);
            if(board[x-xx][y-1]==0 || transparent[id_board[x-xx][y-1]])
                continue;
            else
                break;
        }

        for(int yy=2; yy<=std::min(y, range); yy++){
            addCandidateMove(x,y,x-1, y-yy, piece, pc_id, 4);
            if(board[x-1][y-yy]==0 || transparent[id_board[x-1][y-yy]])
                continue;
            else
                break;
        }
    }

    if(x-1>=0&&y+1<8&&board[x-1][y+1]==0){
        addCandidateMove(x, y, x-1, y+1, piece, pc_id, 1);

        for(int xx=2; xx<=std::min(x, range); xx++){
            addCandidateMove(x,y,x-xx, y+1, piece, pc_id, 4);
            if(board[x-xx][y+1]==0 || transparent[id_board[x-xx][y+1]])
                continue;
            else
                break;
        }

        for(int yy=2; yy<=std::min(7-y, range); yy++){
            addCandidateMove(x,y,x-1, y+yy, piece, pc_id, 4);
            if(board[x-1][y+yy]==0 || transparent[id_board[x-1][y+yy]])
                continue;
            else
                break;
        }
    }
}

void archbishop(int range, int piece, int pc_id, int x, int y){
    if(y+1<8 && board[x][y+1]==0){
        addCandidateMove(x, y, x, y+1, piece, pc_id, 1);
        for(int i=1; i<=std::min(range, std::min(7-(y+1), 7-x)); i++){
            addCandidateMove(x,y,x+i, y+1+i, piece, pc_id, 4);
            if(board[x+i][y+1+i]==0 || transparent[id_board[x+i][y+1+i]])
                continue;
            else
                break;
        }

        for(int i=1; i<=std::min(range, std::min(7-(y+1), x)); i++){
            addCandidateMove(x,y,x-i, y+1+i, piece, pc_id, 4);
            if(board[x-i][y+1+i]==0 || transparent[id_board[x-i][y+1+i]])
                continue;
            else
                break;
        }
    }

    if(y-1>=0 && board[x][y-1]==0){
        addCandidateMove(x, y, x, y-1, piece, pc_id, 1);
        for(int i=1; i<=std::min(range, std::min(y-1, 7-x)); i++){
            addCandidateMove(x, y, x+i, y-1-i, piece, pc_id, 4);
            if(board[x+i][y-1-i]==0 || transparent[id_board[x+i][y-1-i]])
                continue;
            else
                break;
        }

        for(int i=1; i<=std::min(range, std::min(y-1, x)); i++){
            addCandidateMove(x,y,x-i, y-1-i, piece, pc_id, 4);
            if(board[x-i][y-1-i]==0 || transparent[id_board[x-i][y-1-i]])
                continue;
            else
                break;
        }
    }

    if(x+1<8 && board[x+1][y]==0){
        addCandidateMove(x, y, x+1, y, piece, pc_id, 1);
        for(int i=1; i<=std::min(range, std::min(7-y, 7-(x+1))); i++){
            addCandidateMove(x, y, x+1+i, y+i, piece, pc_id, 4);
            if(board[x+1+i][y+i]==0 || transparent[id_board[x+1+i][y+i]])
                continue;
            else
                break;
        }

        for(int i=1; i<=std::min(range, std::min(y, 7-(x+1))); i++){
            addCandidateMove(x,y,x+1+i, y-i, piece, pc_id, 4);
            if(board[x+1+i][y-i]==0 || transparent[id_board[x+i+1][y-i]])
                continue;
            else
                break;
        }
    }

    if(x-1>=0 && board[x-1][y]==0){
        addCandidateMove(x, y, x-1, y, piece, pc_id, 1);
        for(int i=1; i<=std::min(range, std::min(7-y, x-1)); i++){
            addCandidateMove(x,y,x-1-i, y+i, piece, pc_id, 4);
            if(board[x-i-1][y+i]==0 || transparent[id_board[x-i-1][y+i]])
                continue;
            else
                break;
        }

        for(int i=1; i<=std::min(range, std::min(y, x-1)); i++){
            addCandidateMove(x,y,x-1-i, y-i, piece, pc_id, 4);
            if(board[x-i-1][y-i]==0 || transparent[id_board[x-i-1][y-i]])
                continue;
            else
                break;
        }
    }
}

int stonemage_directions[8][2] = {{1, 4}, {-1, 4}, {1, -4}, {-1, -4}, {-4, -1}, {4, 1}, {4, -1}, {-4, 1}};
void stonemage(int moveType, int piece, int pc_id, int x, int y){
    for (int i = 4; i < 8; i++)
    {
        if (x + stonemage_directions[i][0] >= 0 && x + stonemage_directions[i][0] <= 7 && y + stonemage_directions[i][1] >= 0 && y + stonemage_directions[i][1] <= 7)
            addCandidateMove(x, y, x + stonemage_directions[i][0], y + stonemage_directions[i][1], piece, pc_id, moveType);
    }
}


void generateMoves(int pc_id, int x, int y)
{
    if(isFrozen(pc_id)||isPetrified(pc_id))
        return;
    //std::cout<<pc_id<<" "<<x<<" "<<y<<std::endl;
    int piece = pieces[pc_id];
    switch ((piece ^ (piece & 1)))
    {
    case 2: // apprentice
        queen(1, 10, piece, pc_id, x, y);
        break;
    case 4:
        queen(2, 10, piece, pc_id, x, y);
        break;
    case 6:
        bishop(2, 10, piece, pc_id, x, y);
        rook(1, 10, piece, pc_id, x, y);
        rook(2, 13, piece, pc_id, x, y, 2);
        break;
    case 8:
        queen(1, 10, piece, pc_id, x, y);
        queen(2, 13, piece, pc_id, x, y, 2);
        break;
    case 10: // archer
        bishopForward(1, 1, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        rookForward(2, 5, piece, pc_id, x, y, 2);
        break;
    case 12:
        bishopForward(2, 1, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        rookForward(2, 5, piece, pc_id, x, y, 2);
        break;
    case 14:
        bishopForward(2, 1, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        rookForward(2, 5, piece, pc_id, x, y, 2);
        mancerOnlyForward(44, piece, pc_id, x, y);
        break;
    case 16:
        bishopForward(2, 1, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        rookForward(2, 5, piece, pc_id, x, y, 2);
        mancerForward(9, piece, pc_id, x, y);
        break;
    case 18: // axeman
        rookForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(1, 4, piece, pc_id, x, y);
        bishopForward(1, 4, piece, pc_id, x, y);
        break;
    case 20:
        rookForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(1, 4, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        break;
    case 22:
        rookForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(1, 4, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        militia(2, 2, piece, pc_id, x, y, 2);
        break;
    case 24:
        rookForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(2, 4, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        militia(2, 2, piece, pc_id, x, y, 2);
        break;
    case 26: // banshee
        queen(1, 13, piece, pc_id, x, y);
        rook(4, 9, piece, pc_id, x, y, 2);
        break;
    case 28:
        queen(1, 13, piece, pc_id, x, y);
        rook(5, 9, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 30:
        queen(1, 13, piece, pc_id, x, y);
        rook(6, 9, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 32:
        queen(1, 13, piece, pc_id, x, y);
        rook(6, 9, piece, pc_id, x, y, 3);
        rook(2, 10, piece, pc_id, x, y, 2);
        bishop(2, 10, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 34: // bat
        rookHorizontal(1, 1, piece, pc_id, x, y);
        knightHorizontal(9, piece, pc_id, x, y);
        rookBackward(1, 4, piece, pc_id, x, y);
        break;
    case 36:
        rookHorizontal(1, 1, piece, pc_id, x, y);
        knightHorizontal(9, piece, pc_id, x, y);
        mancerHorizonal(9, piece, pc_id, x, y);
        rookBackward(1, 4, piece, pc_id, x, y);
        break;
    case 38:
        rookHorizontal(1, 1, piece, pc_id, x, y);
        knightHorizontal(9, piece, pc_id, x, y);
        mancerHorizonal(9, piece, pc_id, x, y);
        rookBackward(1, 4, piece, pc_id, x, y);
        valkVertical(9, piece, pc_id, x, y);
        break;
    case 40:
        rookHorizontal(1, 1, piece, pc_id, x, y);
        knightHorizontal(9, piece, pc_id, x, y);
        mancerHorizonal(9, piece, pc_id, x, y);
        rookBackward(1, 4, piece, pc_id, x, y);
        valkVertical(9, piece, pc_id, x, y);
        rook(7, 1, piece, pc_id, x, y, 5);
        break;
    case 42: // bishop
        bishop(7, 3, piece, pc_id, x, y);
        break;
    case 44:
        bishop(7, 3, piece, pc_id, x, y);
        rook(1, 1, piece, pc_id, x, y);
        break;
    case 46:
        bishop(7, 3, piece, pc_id, x, y);
        rook(3, 1, piece, pc_id, x, y);
        break;
    case 48:
        bishop(7, 3, piece, pc_id, x, y);
        rook(1, 1, piece, pc_id, x, y);
        valk(9, piece, pc_id, x, y);
        break;
    case 50: // bomber
        rookForward(1, 3, piece, pc_id, x, y);
        bomberBackward(1, 9, piece, pc_id, x, y);
        break;
    case 52:
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        bomberBackward(2, 9, piece, pc_id, x, y);
        break;
    case 54:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(2, 1, piece, pc_id, x, y, 2);
        bishopForward(1, 1, piece, pc_id, x, y);
        bomberBackward(3, 9, piece, pc_id, x, y);
        break;
    case 56:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(3, 1, piece, pc_id, x, y, 2);
        bishopForward(2, 1, piece, pc_id, x, y);
        bomberBackward(4, 9, piece, pc_id, x, y);
        break;
    case 58: // dryad
        rook(1, 12, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        break;
    case 60:
        rook(1, 12, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 62:
        rook(1, 12, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        bishop(2, 1, piece, pc_id, x, y);
        break;
    case 64:
        rook(1, 12, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        bishop(2, 1, piece, pc_id, x, y);
        valk(9, piece, pc_id, x, y);
        break;
    case 66: // duelist
        bishop(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        break;
    case 68:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        break;
    case 70:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 1, piece, pc_id, x, y);
        break;
    case 72:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 3, piece, pc_id, x, y);
        break;
    case 74: // duelist minus armor
        bishop(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        break;
    case 76:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        break;
    case 78:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 1, piece, pc_id, x, y);
        break;
    case 80:
        bishopForward(2, 1, piece, pc_id, x, y);
        bishopBackward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 3, piece, pc_id, x, y);
        break;
    case 82: // fireball
        rookForward(3, 5, piece, pc_id, x, y);
        break;
    case 84:
        rookForward(3, 5, piece, pc_id, x, y);
        bishopForward(2, 5, piece, pc_id, x, y);
        break;
    case 86:
        rookForward(3, 5, piece, pc_id, x, y);
        bishopForward(2, 5, piece, pc_id, x, y);
        rookHorizontal(2, 5, piece, pc_id, x, y);
        break;
    case 88:
        rookForward(3, 5, piece, pc_id, x, y);
        bishopForward(2, 5, piece, pc_id, x, y);
        rookHorizontal(2, 5, piece, pc_id, x, y);
        knightForward(44, piece, pc_id, x, y);
        break;
    case 90: // frog
        bishopForward(1, 14, piece, pc_id, x, y);
        break;
    case 92:
        bishopForward(1, 14, piece, pc_id, x, y);
        rookForward(1, 1, piece, pc_id, x, y);
        break;
    case 94:
        militia(1, 14, piece, pc_id, x, y);
        break;
    case 96:
        militia(1, 14, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        break;
    case 98: // frostmage
        rook(2, 1, piece, pc_id, x, y);
        valk(15, piece, pc_id, x, y);
        rook(3, 15, piece, pc_id, x, y, 3);
        break;
    case 100:
        rook(2, 1, piece, pc_id, x, y);
        valk(15, piece, pc_id, x, y);
        rook(3, 15, piece, pc_id, x, y, 3);
        bishop(1, 15, piece, pc_id, x, y);
        break;
    case 102:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        valk(15, piece, pc_id, x, y);
        rook(3, 15, piece, pc_id, x, y, 3);
        bishop(1, 15, piece, pc_id, x, y);
        break;
    case 104:
        rook(2, 3, piece, pc_id, x, y);
        valk(15, piece, pc_id, x, y);
        rook(3, 15, piece, pc_id, x, y, 3);
        bishop(2, 15, piece, pc_id, x, y);
        break;
    case 106: // ghost
        rook(1, 13, piece, pc_id, x, y);
        break;
    case 108:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 110:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 112:
        queen(1, 13, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        break;
    case 114: // guardian
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        break;
    case 116:
        queen(1, 3, piece, pc_id, x, y);
        queen(3, 1, piece, pc_id, x, y, 2);
        break;
    case 118:
        queen(1, 3, piece, pc_id, x, y);
        queen(4, 1, piece, pc_id, x, y, 2);
        break;
    case 120:
        queen(1, 3, piece, pc_id, x, y);
        queen(5, 1, piece, pc_id, x, y, 2);
        break;
    case 122: // hoplite
        rookForward(1, 3, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 124:
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 126:
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 128:
        militia(1, 3, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 130: // knight
        knight(11, piece, pc_id, x, y);
        break;
    case 132:
        knight(11, piece, pc_id, x, y);
        rook(1, 1, piece, pc_id, x, y);
        break;
    case 134:
        knight(11, piece, pc_id, x, y);
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 136:
        knight(11, piece, pc_id, x, y);
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 138: // militia
        militia(1, 3, piece, pc_id, x, y);
        break;
    case 140:
        militia(1, 3, piece, pc_id, x, y);
        militia(1, 2, piece, pc_id, x, y, 2);
        break;
    case 142:
        militia(1, 3, piece, pc_id, x, y);
        militia(1, 1, piece, pc_id, x, y, 2);
        break;
    case 144:
        militia(1, 3, piece, pc_id, x, y);
        militia(1, 1, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 146: // pawn
        rookForward(1, 1, piece, pc_id, x, y);
        rookForward(2, 2, piece, pc_id, x, y, 2);
        bishopForward(1, 4, piece, pc_id, x, y);
        break;
    case 148:
        rookForward(2, 1, piece, pc_id, x, y);
        rookForward(3, 2, piece, pc_id, x, y, 3);
        bishopForward(1, 4, piece, pc_id, x, y);
        break;
    case 150:
        rookForward(3, 1, piece, pc_id, x, y);
        rookForward(4, 2, piece, pc_id, x, y, 4);
        bishopForward(1, 4, piece, pc_id, x, y);
        break;
    case 152:
        rookForward(4, 1, piece, pc_id, x, y);
        rookForward(5, 2, piece, pc_id, x, y, 6);
        bishopForward(1, 4, piece, pc_id, x, y);
        break;
    case 154: // penguin
        rookForward(1, 3, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        bishopForward(1, 15, piece, pc_id, x, y);
        break;
    case 156:
        rookVertical(1, 3, piece, pc_id, x, y);
        bishop(1, 15, piece, pc_id, x, y);
        break;
    case 158:
        rookVertical(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 1, piece, pc_id, x, y);
        bishop(1, 15, piece, pc_id, x, y);
        break;
    case 160:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 15, piece, pc_id, x, y);
        break;
    case 162: // poisonmage
        rook(1, 1, piece, pc_id, x, y);
        bishop(1, 16, piece, pc_id, x, y);
        rook(2, 16, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 164:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 16, piece, pc_id, x, y);
        rook(2, 16, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 166:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 16, piece, pc_id, x, y);
        rook(2, 16, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 168:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 16, piece, pc_id, x, y);
        rook(2, 16, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        valkHorizontal(16, piece, pc_id, x, y);
        break;
    case 170: // shieldsman
        bishopForward(1, 1, piece, pc_id, x, y);
        rookForward(1, 4, piece, pc_id, x, y);
        break;
    case 172:
        bishopForward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        break;
    case 174:
        bishopForward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(2, 1, piece, pc_id, x, y);
        break;
    case 176:
        bishopForward(1, 1, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        rookHorizontal(3, 1, piece, pc_id, x, y);
        knightForward(9, piece, pc_id, x, y);
        break;
    case 178: // skeleton
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 180:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 182:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        break;
    case 184:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 186: // slime
        rookVertical(1, 3, piece, pc_id, x, y);
        break;
    case 188:
        rookVertical(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 1, piece, pc_id, x, y);
        break;
    case 190:
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 192:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        break;
    case 194: // spearman
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(2, 4, piece, pc_id, x, y, 2);
        break;
    case 196:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(2, 4, piece, pc_id, x, y, 2);
        bishopForward(1, 1, piece, pc_id, x, y);
        break;
    case 198:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(3, 4, piece, pc_id, x, y, 2);
        bishopForward(1, 1, piece, pc_id, x, y);
        break;
    case 200:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(3, 4, piece, pc_id, x, y, 2);
        bishopForward(1, 1, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        break;
    case 202: // spider
        rook(1, 16, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y);
        break;
    case 204:
        rook(1, 16, piece, pc_id, x, y);
        bishop(4, 1, piece, pc_id, x, y);
        break;
    case 206:
        rook(1, 16, piece, pc_id, x, y);
        bishop(6, 1, piece, pc_id, x, y);
        break;
    case 208:
        rook(1, 16, piece, pc_id, x, y);
        bishop(6, 1, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 210: // swordsman
        rookHorizontal(1, 4, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        break;
    case 212:
        rookHorizontal(1, 3, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 214:
        rookHorizontal(1, 3, piece, pc_id, x, y);
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 4, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 216:
        militia(1, 3, piece, pc_id, x, y);
        rookHorizontal(1, 3, piece, pc_id, x, y);
        rookHorizontal(2, 3, piece, pc_id, x, y, 2);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 218: // tiger
        queen(1, 1, piece, pc_id, x, y);
        rook(2, 17, piece, pc_id, x, y, 2);
        break;
    case 220:
        queen(1, 1, piece, pc_id, x, y);
        queen(2, 17, piece, pc_id, x, y, 2);
        break;
    case 222:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        queen(2, 17, piece, pc_id, x, y, 2);
        break;
    case 224:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 17, piece, pc_id, x, y, 2);
        break;
    case 226: // warrior
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 228:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 230:
        queen(2, 3, piece, pc_id, x, y);
        break;
    case 232:
        queen(2, 3, piece, pc_id, x, y);
        queen(3, 1, piece, pc_id, x, y, 3);
        break;
    case 234: // windmage
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 6, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 236:
        knight(9, piece, pc_id, x, y);
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(3, 6, piece, pc_id, x, y, 2);
        bishop(2, 6, piece, pc_id, x, y, 2);
        break;
    case 238:
        queen(1, 13, piece, pc_id, x, y);
        queen(3, 6, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 240:
        queen(1, 13, piece, pc_id, x, y);
        knight(10, piece, pc_id, x, y);
        rook(4, 6, piece, pc_id, x, y, 2);
        bishop(3, 6, piece, pc_id, x, y, 2);
        break;
    case 242: // Alchemist
        queen(2, 16, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 244:
        queen(2, 16, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 246:
        queen(2, 16, piece, pc_id, x, y, 2);
        knight(10, piece, pc_id, x, y);
        break;
    case 248:
        queen(2, 16, piece, pc_id, x, y, 2);
        knight(13, piece, pc_id, x, y);
        break;
    case 250: // Arachnid
        bishop(3, 3, piece, pc_id, x, y);
        rook(1, 16, piece, pc_id, x, y);
        break;
    case 252:
        bishop(5, 3, piece, pc_id, x, y);
        rook(1, 16, piece, pc_id, x, y);
        break;
    case 254:
        bishop(7, 3, piece, pc_id, x, y);
        rook(1, 16, piece, pc_id, x, y);
        break;
    case 256:
        bishop(7, 3, piece, pc_id, x, y);
        rook(2, 16, piece, pc_id, x, y);
        break;
    case 258: // Basilisk
        bishopForward(1, 3, piece, pc_id, x, y);
        rookForward(1, 7, piece, pc_id, x, y);
        break;
    case 260:
        bishopForward(1, 3, piece, pc_id, x, y);
        rookForward(1, 7, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 262:
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        rookForward(1, 7, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 264:
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        rookForward(1, 7, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        knightOnlyForward(44, piece, pc_id, x, y);
        mancerOnlyForward(44, piece, pc_id, x, y);
        break;
    case 266: // Berserker
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 4, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 3);
        break;
    case 268:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 3);
        break;
    case 270:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(4, 4, piece, pc_id, x, y, 3);
        break;
    case 272:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(5, 4, piece, pc_id, x, y, 3);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 274: // Crusader
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 276:
        rook(4, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y);
        break;
    case 278:
        rook(4, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 280:
        rook(5, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 282: // Crusader w/o armor
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 284:
        rook(4, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y);
        break;
    case 286:
        rook(4, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 288:
        rook(5, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 290: // Dove
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 292:
        bishop(2, 1, piece, pc_id, x, y);
        break;
    case 294:
        bishop(2, 1, piece, pc_id, x, y);
        valk(9, piece, pc_id, x, y);
        break;
    case 296:
        bishop(2, 1, piece, pc_id, x, y);
        valk(10, piece, pc_id, x, y);
        break;
    case 298: // Drake
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 300:
        rook(2, 3, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 302:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 11, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 304:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 11, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        valk(9, piece, pc_id, x, y);
        break;
    case 306: // Fencer
        bishop(4, 1, piece, pc_id, x, y);
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 308:
        bishop(4, 1, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 310:
        bishop(5, 1, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 312:
        bishop(5, 9, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 314: // Fencer w/o armor
        bishop(4, 1, piece, pc_id, x, y);
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 316:
        bishop(4, 1, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 318:
        bishop(5, 1, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 320:
        bishop(5, 9, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 322: // Firemage
        knight(9, piece, pc_id, x, y);
        rook(1, 46, piece, pc_id, x, y);
        rook(3, 45, piece, pc_id, x, y, 3);
        break;
    case 324:
        knight(9, piece, pc_id, x, y);
        rook(1, 46, piece, pc_id, x, y);
        rook(3, 45, piece, pc_id, x, y, 3);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 326:
        knight(9, piece, pc_id, x, y);
        rook(1, 46, piece, pc_id, x, y);
        rook(3, 45, piece, pc_id, x, y, 3);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 328:
        knight(10, piece, pc_id, x, y);
        rook(1, 46, piece, pc_id, x, y);
        rook(3, 45, piece, pc_id, x, y, 3);
        rook(5, 45, piece, pc_id, x, y, 5);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 330: // Frostmephit
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 332:
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 334:
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        rook(1, 1, piece, pc_id, x, y);
        break;
    case 336:
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        rook(1, 10, piece, pc_id, x, y);
        break;
    case 338: // Giant Slime
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        break;
    case 340:
        rook(2, 3, piece, pc_id, x, y);
        break;
    case 342:
        rook(2, 3, piece, pc_id, x, y);
        rook(3, 1, piece, pc_id, x, y, 3);
        break;
    case 344:
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 346: // Gnome
        bishop(1, 1, piece, pc_id, x, y);
        rook(1, 7, piece, pc_id, x, y);
        break;
    case 348:
        bishop(2, 1, piece, pc_id, x, y);
        rook(1, 7, piece, pc_id, x, y);
        break;
    case 350:
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y);
        rook(1, 7, piece, pc_id, x, y);
        break;
    case 352:
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y);
        rook(1, 7, piece, pc_id, x, y);
        break;
    case 354: // Hostage
        queen(1, 1, piece, pc_id, x, y);
        break;
    case 356:
        bishop(2, 1, piece, pc_id, x, y);
        rook(1, 1, piece, pc_id, x, y);
        break;
    case 358:
        queen(2, 1, piece, pc_id, x, y);
        break;
    case 360:
        queen(1, 1, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 362: // Hydromancer
        mancer(9, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        queen(1, 18, piece, pc_id, x, y);
        break;
    case 364:
        mancer(9, piece, pc_id, x, y);
        queen(2, 4, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        queen(1, 18, piece, pc_id, x, y);
        break;
    case 366:
        mancer(9, piece, pc_id, x, y);
        queen(2, 4, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        valk(9, piece, pc_id, x, y);
        queen(1, 18, piece, pc_id, x, y);
        break;
    case 368:
        mancer(9, piece, pc_id, x, y);
        queen(2, 19, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        valk(9, piece, pc_id, x, y);
        queen(1, 18, piece, pc_id, x, y);
        break;
    case 370: // Legionary
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 4, piece, pc_id, x, y, 2);
        break;
    case 372:
        queen(1, 3, piece, pc_id, x, y);
        bishop(3, 4, piece, pc_id, x, y, 2);
        rook(2, 4, piece, pc_id, x, y, 2);
        break;
    case 374:
        queen(1, 3, piece, pc_id, x, y);
        queen(3, 4, piece, pc_id, x, y, 2);
        break;
    case 376:
        queen(2, 3, piece, pc_id, x, y);
        queen(3, 4, piece, pc_id, x, y, 3);
        break;
    case 378: // Mage Tower
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 3, piece, pc_id, x, y, 3);
        break;
    case 380:
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 3, piece, pc_id, x, y, 3);
        break;
    case 382:
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 3, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 384:
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 3, piece, pc_id, x, y, 3);
        bishop(2, 9, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 386: // Necromancer
        queen(1, 1, piece, pc_id, x, y);
        knight(20, piece, pc_id, x, y);
        break;
    case 388:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        knight(20, piece, pc_id, x, y);
        break;
    case 390:
        queen(1, 3, piece, pc_id, x, y);
        knight(20, piece, pc_id, x, y);
        break;
    case 392:
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 20, piece, pc_id, x, y, 2);
        knight(20, piece, pc_id, x, y);
        break;
    case 394: // Nexus
        break;
    case 396:
        break;
    case 398:
        break;
    case 400:
        break;
    case 402: // Paladin
        rookVertical(2, 3, piece, pc_id, x, y);
        rookHorizontal(7, 13, piece, pc_id, x, y);
        break;
    case 404:
        rookVertical(2, 3, piece, pc_id, x, y);
        rookHorizontal(7, 13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 406:
        rookVertical(2, 3, piece, pc_id, x, y);
        rookHorizontal(7, 13, piece, pc_id, x, y);
        rookVertical(3, 9, piece, pc_id, x, y, 3);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 408:
        rookVertical(2, 3, piece, pc_id, x, y);
        rookHorizontal(7, 13, piece, pc_id, x, y);
        rookVertical(3, 10, piece, pc_id, x, y, 3);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 10, piece, pc_id, x, y, 2);
        break;
    case 410: // Phantasm
        rook(1, 4, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 412:
        rook(1, 3, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 414:
        rook(1, 13, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 416:
        rook(1, 13, piece, pc_id, x, y);
        bishop(2, 10, piece, pc_id, x, y, 2);
        rook(3, 10, piece, pc_id, x, y, 3);
        break;
    case 418: // Pikeman
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 4, piece, pc_id, x, y);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 420:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(2, 4, piece, pc_id, x, y, 2);
        bishopForward(1, 4, piece, pc_id, x, y);
        rookBackward(2, 1, piece, pc_id, x, y);
        break;
    case 422:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(2, 4, piece, pc_id, x, y, 2);
        bishopForward(2, 4, piece, pc_id, x, y);
        rookBackward(3, 1, piece, pc_id, x, y);
        break;
    case 424:
        rookForward(1, 3, piece, pc_id, x, y);
        rookForward(3, 4, piece, pc_id, x, y, 2);
        bishopForward(2, 4, piece, pc_id, x, y);
        rookBackward(4, 1, piece, pc_id, x, y);
        break;
    case 426: // Portal
        break;
    case 428:
        break;
    case 430:
        break;
    case 432:
        break;
    case 434: // Prince
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 436:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        break;
    case 438:
        queen(2, 3, piece, pc_id, x, y);
        break;
    case 440:
        queen(1, 13, piece, pc_id, x, y);
        queen(2, 3, piece, pc_id, x, y, 2);
        break;
    case 442: // Princess
        queen(1, 1, piece, pc_id, x, y);
        break;
    case 444:
        queen(2, 1, piece, pc_id, x, y);
        break;
    case 446:
        queen(3, 1, piece, pc_id, x, y);
        break;
    case 448:
        queen(3, 1, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 450: // Pyromancer
        mancer(9, piece, pc_id, x, y);
        queen(1, 46, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 452:
        mancer(9, piece, pc_id, x, y);
        queen(1, 46, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 454:
        mancer(9, piece, pc_id, x, y);
        queen(1, 46, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y, 2);
        bishop(2, 1, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 456:
        mancer(9, piece, pc_id, x, y);
        queen(1, 46, piece, pc_id, x, y);
        queen(2, 3, piece, pc_id, x, y, 2);
        rook(3, 9, piece, pc_id, x, y, 3);
        break;
    case 458: // Ranger
        rook(1, 3, piece, pc_id, x, y);
        bishop(2, 5, piece, pc_id, x, y, 2);
        break;
    case 460:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 5, piece, pc_id, x, y, 2);
        break;
    case 462:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 5, piece, pc_id, x, y, 2);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 5, piece, pc_id, x, y, 2);
        break;
    case 464:
        rook(2, 5, piece, pc_id, x, y, 2);
        queen(1, 3, piece, pc_id, x, y);
        bishop(2, 5, piece, pc_id, x, y, 2);
        break;
    case 466: // Rook
        rook(7, 3, piece, pc_id, x, y);
        break;
    case 468:
        rook(7, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 470:
        rook(7, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 472:
        rook(7, 3, piece, pc_id, x, y);
        bishop(2, 10, piece, pc_id, x, y);
        break;
    case 474: // Samurai
        bishopForward(1, 3, piece, pc_id, x, y);
        break;
    case 476:
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        break;
    case 478:
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 480:
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        rookBackward(1, 1, piece, pc_id, x, y);
        break;
    case 482: // Snake
        bishop(1, 1, piece, pc_id, x, y);
        rook(1, 17, piece, pc_id, x, y);
        break;
    case 484:
        bishop(2, 1, piece, pc_id, x, y);
        rook(1, 17, piece, pc_id, x, y);
        break;
    case 486:
        bishop(3, 1, piece, pc_id, x, y);
        rook(1, 17, piece, pc_id, x, y);
        break;
    case 488:
        bishop(3, 1, piece, pc_id, x, y);
        rook(2, 17, piece, pc_id, x, y);
        break;
    case 490: // Stonemage
        rook(1, 13, piece, pc_id, x, y);
        rookHorizontal(2, 10, piece, pc_id, x, y, 2);
        bishop(1, 23, piece, pc_id, x, y);
        break;
    case 492:
        rook(1, 13, piece, pc_id, x, y);
        rookHorizontal(3, 10, piece, pc_id, x, y, 2);
        bishop(1, 23, piece, pc_id, x, y);
        knightHorizontal(23, piece, pc_id, x, y);
        break;
    case 494:
        rook(1, 13, piece, pc_id, x, y);
        rookHorizontal(4, 10, piece, pc_id, x, y, 2);
        bishop(1, 23, piece, pc_id, x, y);
        knightHorizontal(23, piece, pc_id, x, y);
        valkHorizontal(23, piece, pc_id, x, y);
        break;
    case 496:
        rook(1, 13, piece, pc_id, x, y);
        rookHorizontal(5, 10, piece, pc_id, x, y, 2);
        bishop(1, 23, piece, pc_id, x, y);
        knightHorizontal(23, piece, pc_id, x, y);
        valkHorizontal(23, piece, pc_id, x, y);
        stonemage(23, piece, pc_id, x, y);
        break;
    case 498: // Sylph
        rook(1, 1, piece, pc_id, x, y);
        bishop(1, 6, piece, pc_id, x, y);
        break;
    case 500:
        rook(2, 1, piece, pc_id, x, y);
        bishop(1, 6, piece, pc_id, x, y);
        break;
    case 502:
        rook(2, 1, piece, pc_id, x, y);
        bishop(2, 6, piece, pc_id, x, y);
        break;
    case 504:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        bishop(2, 6, piece, pc_id, x, y);
        break;
    case 506: // Templar
        rookVertical(1, 1, piece, pc_id, x, y);
        rookHorizontal(2, 13, piece, pc_id, x, y);
        break;
    case 508:
        rookVertical(1, 3, piece, pc_id, x, y);
        rookHorizontal(2, 13, piece, pc_id, x, y);
        break;
    case 510:
        rookVertical(1, 3, piece, pc_id, x, y);
        rookHorizontal(3, 13, piece, pc_id, x, y);
        break;
    case 512:
        rookVertical(1, 3, piece, pc_id, x, y);
        rookHorizontal(3, 13, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 514: // Thundermage
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(4, 24, piece, pc_id, x, y, 4);
        bishop(3, 24, piece, pc_id, x, y, 3);
        break;
    case 516:
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(4, 24, piece, pc_id, x, y, 4);
        bishop(3, 24, piece, pc_id, x, y, 3);
        break;
    case 518:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 24, piece, pc_id, x, y, 4);
        bishop(3, 24, piece, pc_id, x, y, 3);
        break;
    case 520:
        queen(1, 13, piece, pc_id, x, y);
        rook(2, 9, piece, pc_id, x, y, 2);
        rook(5, 24, piece, pc_id, x, y, 4);
        bishop(4, 24, piece, pc_id, x, y, 3);
        break;
    case 522: // Toad
        queen(1, 14, piece, pc_id, x, y);
        break;
    case 524:
        queen(1, 14, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 526:
        queen(1, 14, piece, pc_id, x, y);
        rook(2, 11, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 528:
        queen(1, 14, piece, pc_id, x, y);
        queen(2, 11, piece, pc_id, x, y, 2);
        break;
    case 530: // Wisp
        break;
    case 532:
        break;
    case 534:
        break;
    case 536:
        break;
    case 538: // Air Elemental
        rook(3, 3, piece, pc_id, x, y);
        bishop(2, 6, piece, pc_id, x, y);
        break;
    case 540:
        rook(4, 3, piece, pc_id, x, y);
        bishop(3, 6, piece, pc_id, x, y);
        break;
    case 542:
        rook(5, 3, piece, pc_id, x, y);
        bishop(4, 6, piece, pc_id, x, y);
        break;
    case 544:
        rook(6, 3, piece, pc_id, x, y);
        bishop(5, 6, piece, pc_id, x, y);
        break;
    case 546: // Archbishop
        archbishop(2, piece, pc_id, x, y);
        bishop(3,3, piece, pc_id, x, y);
        break;
    case 548:
        archbishop(3, piece, pc_id, x, y);
        bishop(4,3, piece, pc_id, x, y);
        break;
    case 550:
        archbishop(4, piece, pc_id, x, y);
        bishop(5,3, piece, pc_id, x, y);
        break;
    case 552:
        archbishop(5, piece, pc_id, x, y);
        bishop(6,3, piece, pc_id, x, y);
        break;
    case 554: // Beacon
        break;
    case 556:
        break;
    case 558:
        break;
    case 560:
        break;
    case 562: // Behemoth
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 564:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        break;
    case 566:
        queen(2, 3, piece, pc_id, x, y);
        break;
    case 568:
        queen(3, 3, piece, pc_id, x, y);
        break;
    case 570: // Butterfly
        rook(1, 1, piece, pc_id, x, y);
        bishop(3, 25, piece, pc_id, x, y, 3);
        valk(25, piece, pc_id, x, y);
        break;
    case 572:
        rook(1, 3, piece, pc_id, x, y);
        bishop(3, 25, piece, pc_id, x, y, 3);
        valk(25, piece, pc_id, x, y);
        break;
    case 574:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(3, 25, piece, pc_id, x, y, 3);
        valk(25, piece, pc_id, x, y);
        break;
    case 576:
        queen(1, 3, piece, pc_id, x, y);
        bishop(3, 25, piece, pc_id, x, y, 3);
        valk(25, piece, pc_id, x, y);
        break;
    case 578: // Chastity
        queen(1, 3, piece, pc_id, x, y);
        rookHorizontal(7, 4, piece, pc_id, x, y, 2);
        break;
    case 580:
        queen(1, 3, piece, pc_id, x, y);
        rookHorizontal(7, 19, piece, pc_id, x, y, 2);
        break;
    case 582:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 19, piece, pc_id, x, y, 2);
        rookHorizontal(7, 19, piece, pc_id, x, y, 3);
        break;
    case 584:
        queen(1, 13, piece, pc_id, x, y);
        queen(2, 19, piece, pc_id, x, y, 2);
        rookHorizontal(7, 19, piece, pc_id, x, y, 3);
        break;
    case 586: // Demon
        rook(4, 3, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 588:
        rook(4, 3, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        doubleKnight(9, piece, pc_id, x, y);
        break;
    case 590:
        rook(4, 3, piece, pc_id, x, y);
        knight(11, piece, pc_id, x, y);
        doubleKnight(9, piece, pc_id, x, y);
        break;
    case 592:
        rook(4, 3, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        doubleKnight(10, piece, pc_id, x, y);
        break;
    case 594: // Dragon
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        knight(11, piece, pc_id, x, y);
        break;
    case 596:
        queen(2, 3, piece, pc_id, x, y);
        knight(11, piece, pc_id, x, y);
        break;
    case 598:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 11, piece, pc_id, x, y, 2);
        knight(11, piece, pc_id, x, y);
        break;
    case 600:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 11, piece, pc_id, x, y, 2);
        knight(11, piece, pc_id, x, y);
        valk(9, piece, pc_id, x, y);
        break;
    case 602: // Earth Elemental
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 23, piece, pc_id, x, y, 3);
        break;
    case 604:
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 23, piece, pc_id, x, y, 3);
        bishop(2, 23, piece, pc_id, x, y, 2);
        break;
    case 606:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(3, 23, piece, pc_id, x, y, 3);
        bishop(2, 23, piece, pc_id, x, y, 2);
        break;
    case 608:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        rook(4, 23, piece, pc_id, x, y, 3);
        bishop(2, 23, piece, pc_id, x, y, 2);
        break;
    case 610: // Ghast
        break;
    case 612:
        break;
    case 614:
        break;
    case 616:
        break;
    case 618: // Harpy
        rook(1, 6, piece, pc_id, x, y);
        rook(4, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(3, 9, piece, pc_id, x, y, 2);
        mancer(9, piece, pc_id, x, y);
        break;
    case 620:
        rook(1, 6, piece, pc_id, x, y);
        rook(5, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(3, 9, piece, pc_id, x, y, 2);
        mancer(9, piece, pc_id, x, y);
        break;
    case 622:
        rook(1, 6, piece, pc_id, x, y);
        rook(6, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(3, 9, piece, pc_id, x, y, 2);
        mancer(9, piece, pc_id, x, y);
        break;
    case 624:
        rook(1, 6, piece, pc_id, x, y);
        rook(7, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(3, 10, piece, pc_id, x, y, 2);
        mancer(9, piece, pc_id, x, y);
        break;
    case 626: // Haunted Armor
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 628:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        break;
    case 630:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 632:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 10, piece, pc_id, x, y, 2);
        break;
    case 634: // Haunted Armor w/o armor
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 636:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        break;
    case 638:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 640:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 10, piece, pc_id, x, y, 2);
        break;
    case 642: // Life Stone
        rookForward(1, 26, piece, pc_id, x, y);
        break;
    case 644:
        militia(1, 26, piece, pc_id, x, y);
        break;
    case 646:
        militia(1, 26, piece, pc_id, x, y);
        rookHorizontal(1, 26, piece, pc_id, x, y);
        break;
    case 648:
        queen(1, 26, piece, pc_id, x, y);
        break;
    case 650: // Medusa
        bishop(3, 7, piece, pc_id, x, y);
        rook(2, 7, piece, pc_id, x, y);
        break;
    case 652:
        bishop(4, 7, piece, pc_id, x, y);
        rook(2, 7, piece, pc_id, x, y);
        break;
    case 654:
        bishop(4, 7, piece, pc_id, x, y);
        rook(3, 7, piece, pc_id, x, y);
        break;
    case 656:
        bishop(4, 7, piece, pc_id, x, y);
        rook(4, 7, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 658: // Mercenary
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 660:
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 662:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 664:
        rook(3, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 666: // Minotaur
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 668:
        rook(4, 3, piece, pc_id, x, y);
        break;
    case 670:
        rook(4, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 672:
        rook(4, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 1, piece, pc_id, x, y, 2);
        break;
    case 674: // Ninja
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 11, piece, pc_id, x, y, 3);
        bishop(2, 11, piece, pc_id, x, y, 2);
        break;
    case 676:
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 1, piece, pc_id, x, y, 2);
        rook(3, 11, piece, pc_id, x, y, 3);
        bishop(2, 11, piece, pc_id, x, y, 2);
        break;
    case 678:
        rook(2, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        rook(3, 11, piece, pc_id, x, y, 3);
        bishop(2, 11, piece, pc_id, x, y, 2);
        break;
    case 680:
        rook(2, 3, piece, pc_id, x, y, 2);
        bishop(1, 3, piece, pc_id, x, y);
        rook(3, 11, piece, pc_id, x, y, 3);
        bishop(2, 11, piece, pc_id, x, y, 2);
        valk(9, piece, pc_id, x, y);
        break;
    case 682: // Nullmage
        bishop(1, 27, piece, pc_id, x, y);
        knight(27, piece, pc_id, x, y);
        valk(27, piece, pc_id, x, y);
        rook(2, 10, piece, pc_id, x, y);
        break;
    case 684:
        bishop(1, 27, piece, pc_id, x, y);
        knight(27, piece, pc_id, x, y);
        valk(27, piece, pc_id, x, y);
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 10, piece, pc_id, x, y, 2);
        break;
    case 686:
        bishop(1, 27, piece, pc_id, x, y);
        knight(27, piece, pc_id, x, y);
        valk(27, piece, pc_id, x, y);
        rook(1, 13, piece, pc_id, x, y);
        rook(4, 10, piece, pc_id, x, y, 2);
        break;
    case 688:
        bishop(1, 27, piece, pc_id, x, y);
        knight(27, piece, pc_id, x, y);
        valk(27, piece, pc_id, x, y);
        rook(2, 13, piece, pc_id, x, y);
        rook(4, 10, piece, pc_id, x, y, 3);
        break;
    case 690: // Patience
        if(turn<50){
            bishop(3, 1, piece, pc_id, x, y);
            break;
        }
        bishop(3, 3, piece, pc_id, x, y);
        rook(1, 4, piece, pc_id, x, y);
        break;
    case 692:
        if(turn<60){
            bishop(4, 1, piece, pc_id, x, y);
            rook(1, 1, piece, pc_id, x, y);
            break;
        }
        bishop(4, 3, piece, pc_id, x, y);
        rook(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        break;
    case 694:
        if(turn<70){
            bishop(5, 1, piece, pc_id, x, y);
            rook(2, 1, piece, pc_id, x, y);
            break;
        }
        bishop(5, 3, piece, pc_id, x, y);
        rook(2, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 3);
        break;
    case 696:
        if(turn<80){
            bishop(5, 1, piece, pc_id, x, y);
            rook(2, 1, piece, pc_id, x, y);
            break;
        }
        bishop(7, 3, piece, pc_id, x, y);
        rook(3, 3, piece, pc_id, x, y);
        rook(4, 4, piece, pc_id, x, y, 4);
        break;
    case 698: // Phalanx
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        break;
    case 700:
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 702:
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 2);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 704:
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 2);
        bishop(3, 4, piece, pc_id, x, y, 2);
        break;
    case 706: // Phalanx w/o armor
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        break;
    case 708:
        queen(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y, 2);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 710:
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 2);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 712:
        queen(1, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 2);
        bishop(3, 4, piece, pc_id, x, y, 2);
        break;
    case 714: // Queen
        queen(7, 3, piece, pc_id, x, y);
        break;
    case 716:
        queen(7, 3, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 718:
        queen(7, 3, piece, pc_id, x, y);
        knight(11, piece, pc_id, x, y);
        break;
    case 720:
        queen(7, 3, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        break;
    case 722: // Reaver
        bishop(1, 3, piece, pc_id, x, y);
        rook(1, 4, piece, pc_id, x, y);
        break;
    case 724:
        bishop(1, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y);
        break;
    case 726:
        bishop(2, 3, piece, pc_id, x, y);
        rook(2, 4, piece, pc_id, x, y);
        break;
    case 728:
        bishop(2, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y);
        break;
    case 730: // Salamander
        rookForward(1, 46, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        break;
    case 732:
        rookForward(1, 46, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 2, piece, pc_id, x, y, 2);
        break;
    case 734:
        rookForward(1, 46, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        break;
    case 736:
        rookForward(1, 46, piece, pc_id, x, y);
        bishopForward(1, 3, piece, pc_id, x, y);
        bishopForward(2, 1, piece, pc_id, x, y, 2);
        bishopBackward(1, 1, piece, pc_id, x, y);
        break;
    case 738: // Soulflare
        rook(2, 28, piece, pc_id, x, y);
        break;
    case 740:
        rook(2, 28, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 742:
        rook(3, 28, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 744:
        rook(3, 28, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 746: // Summoner
        break;
    case 748:
        break;
    case 750:
        break;
    case 752:
        break;
    case 754: // Temperance
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        valk(19, piece, pc_id, x, y);
        rook(3, 19, piece, pc_id, x, y, 3);
        break;
    case 756:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        valk(19, piece, pc_id, x, y);
        rook(3, 19, piece, pc_id, x, y, 3);
        break;
    case 758:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        valk(19, piece, pc_id, x, y);
        rook(3, 19, piece, pc_id, x, y, 3);
        break;
    case 760:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 19, piece, pc_id, x, y, 2);
        valk(19, piece, pc_id, x, y);
        rook(3, 19, piece, pc_id, x, y, 3);
        break;
    case 762: // Tombstone
        break;
    case 764:
        break;
    case 766:
        break;
    case 768:
        break;
    case 770: // Undine
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(1, 18, piece, pc_id, x, y);
        break;
    case 772:
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        rookHorizontal(2, 18, piece, pc_id, x, y);
        break;
    case 774:
        rookForward(1, 3, piece, pc_id, x, y);
        bishopForward(1, 1, piece, pc_id, x, y);
        rookForward(2, 18, piece, pc_id, x, y, 2);
        rookHorizontal(2, 18, piece, pc_id, x, y);
        break;
    case 776:
        militia(1, 3, piece, pc_id, x, y);
        rookForward(2, 18, piece, pc_id, x, y, 2);
        rookHorizontal(2, 18, piece, pc_id, x, y);
        break;
    case 778: // Valkyrie
        valk(11, piece, pc_id, x, y);
        rook(1, 31, piece, pc_id, x, y);
        break;
    case 780:
        valk(11, piece, pc_id, x, y);
        rook(1, 31, piece, pc_id, x, y);
        rook(3, 31, piece, pc_id, x, y, 3);
        bishop(2, 31, piece, pc_id, x, y, 2);
        break;
    case 782:
        valk(11, piece, pc_id, x, y);
        rook(1, 31, piece, pc_id, x, y);
        rook(3, 31, piece, pc_id, x, y, 3);
        rook(2, 11, piece, pc_id, x, y, 2);
        bishop(2, 31, piece, pc_id, x, y, 2);
        break;
    case 784:
        valk(11, piece, pc_id, x, y);
        rook(1, 31, piece, pc_id, x, y);
        rook(3, 31, piece, pc_id, x, y, 3);
        rook(2, 11, piece, pc_id, x, y, 2);
        bishop(2, 31, piece, pc_id, x, y, 2);
        bishop(1, 11, piece, pc_id, x, y);
        break;
    case 786: // Wizard
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 13, piece, pc_id, x, y, 2);
        break;
    case 788:
        queen(2, 13, piece, pc_id, x, y);
        break;
    case 790:
        queen(2, 13, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 792:
        queen(2, 13, piece, pc_id, x, y);
        knight(10, piece, pc_id, x, y);
        break;
    case 794: // Angel
        rook(2, 3, piece, pc_id, x, y);
        valk(13, piece, pc_id, x, y);
        break;
    case 796:
        rook(2, 3, piece, pc_id, x, y);
        valk(13, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 798:
        rook(2, 3, piece, pc_id, x, y);
        valk(13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 800:
        rook(3, 3, piece, pc_id, x, y);
        valk(13, piece, pc_id, x, y);
        bishop(2, 3, piece, pc_id, x, y);
        break;
    case 802: // Antimage
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 4, piece, pc_id, x, y);
        break;
    case 804:
        rook(2, 3, piece, pc_id, x, y);
        bishop(2, 4, piece, pc_id, x, y);
        break;
    case 806:
        rook(2, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 3);
        bishop(2, 4, piece, pc_id, x, y);
        break;
    case 808:
        rook(2, 3, piece, pc_id, x, y);
        rook(3, 4, piece, pc_id, x, y, 3);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 4, piece, pc_id, x, y, 2);
        break;
    case 810: // Aquarius
        rook(2, 3, piece, pc_id, x, y);
        knight(32, piece, pc_id, x, y);
        break;
    case 812:
        rook(3, 3, piece, pc_id, x, y);
        knight(32, piece, pc_id, x, y);
        valk(32, piece, pc_id, x, y);
        break;
    case 814:
        rook(3, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        knight(32, piece, pc_id, x, y);
        valk(32, piece, pc_id, x, y);
        break;
    case 816:
        rook(4, 3, piece, pc_id, x, y);
        bishop(2, 3, piece, pc_id, x, y);
        knight(32, piece, pc_id, x, y);
        valk(32, piece, pc_id, x, y);
        break;
    case 818: // Comet
        break;
    case 820:
        break;
    case 822:
        break;
    case 824:
        break;
    case 826: // Enchantress
        queen(1, 1, piece, pc_id, x, y);
        queen(2, 34, piece, pc_id, x, y, 2);
        break;
    case 828:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        queen(2, 34, piece, pc_id, x, y, 2);
        break;
    case 830:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 34, piece, pc_id, x, y, 2);
        break;
    case 832:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 34, piece, pc_id, x, y, 2);
        knight(34, piece, pc_id, x, y);
        break;
    case 834: // Envy
        rook(1, 3, piece, pc_id, x, y);
        valk(47, piece, pc_id, x, y);
        break;
    case 836:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        valk(47, piece, pc_id, x, y);
        break;
    case 838:
        queen(1, 3, piece, pc_id, x, y);
        valk(47, piece, pc_id, x, y);
        break;
    case 840:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        rook(3, 47, piece, pc_id, x, y, 3);
        valk(47, piece, pc_id, x, y);
        break;
    case 842: // Fire Elemental
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        queen(2, 46, piece, pc_id, x, y, 2);
        break;
    case 844:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 46, piece, pc_id, x, y, 2);
        break;
    case 846:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 46, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 848:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 46, piece, pc_id, x, y, 2);
        knight(10, piece, pc_id, x, y);
        break;
    case 850: // Fortress
        fortress(3, piece, pc_id, x, y);
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 852:
        fortress(4, piece, pc_id, x, y);
        rook(4, 3, piece, pc_id, x, y);
        break;
    case 854:
        fortress(5, piece, pc_id, x, y);
        rook(5, 3, piece, pc_id, x, y);
        break;
    case 856:
        fortress(6, piece, pc_id, x, y);
        rook(6, 3, piece, pc_id, x, y);
        break;
    case 858: // Gemini
        rookHorizontal(3, -1, piece, pc_id, x, y, 3);
        knight(9, piece, pc_id, x, y);
        break;
    case 860:
        rookHorizontal(3, -1, piece, pc_id, x, y, 2);
        knight(11, piece, pc_id, x, y);
        break;
    case 862:
        rookHorizontal(3, -1, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        break;
    case 864:
        rook(3, -1, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        break;
    case 866: // Gemitwin
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 868:
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 870:
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 872:
        rook(3, 3, piece, pc_id, x, y);
        break;
    case 874: // Gluttony
        break;
    case 876:
        break;
    case 878:
        break;
    case 880:
        break;
    case 882: // Gravity Mage
        rook(3, 36, piece, pc_id, x, y, 3);
        bishop(2, 36, piece, pc_id, x, y, 2);
        rook(1, 10, piece, pc_id, x, y);
        break;
    case 884:
        rook(3, 36, piece, pc_id, x, y, 3);
        bishop(2, 36, piece, pc_id, x, y, 2);
        rook(2, 10, piece, pc_id, x, y);
        break;
    case 886:
        rook(3, 36, piece, pc_id, x, y, 3);
        bishop(2, 36, piece, pc_id, x, y, 2);
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 10, piece, pc_id, x, y, 2);
        break;
    case 888:
        rook(3, 36, piece, pc_id, x, y, 3);
        bishop(2, 36, piece, pc_id, x, y, 2);
        bishop(1, 10, piece, pc_id, x, y);
        rook(1, 13, piece, pc_id, x, y);
        rook(2, 10, piece, pc_id, x, y, 2);
        break;
    case 890: // Greed
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 892:
        rook(3, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 894:
        rook(4, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 896:
        rook(5, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 898: // Illusionist
        rook(2, 3, piece, pc_id, x, y);
        knight(36, piece, pc_id, x, y);
        break;
    case 900:
        rook(3, 3, piece, pc_id, x, y);
        knight(36, piece, pc_id, x, y);
        break;
    case 902:
        rook(4, 3, piece, pc_id, x, y);
        knight(36, piece, pc_id, x, y);
        break;
    case 904:
        rook(5, 3, piece, pc_id, x, y);
        knight(36, piece, pc_id, x, y);
        break;
    case 906: // Illusion
        knight(36, piece, pc_id, x, y);
        break;
    case 908:
        knight(36, piece, pc_id, x, y);
        break;
    case 910:
        knight(36, piece, pc_id, x, y);
        break;
    case 912:
        knight(36, piece, pc_id, x, y);
        break;
    case 914: // Lich
        queen(1, 1, piece, pc_id, x, y);
        queen(2, 30, piece, pc_id, x, y, 2);
        break;
    case 916:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        queen(2, 30, piece, pc_id, x, y, 2);
        break;
    case 918:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 30, piece, pc_id, x, y, 2);
        break;
    case 920:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 30, piece, pc_id, x, y, 2);
        knight(30, piece, pc_id, x, y);
        break;
    case 922: // Lilith
        queen(1, 28, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 924:
        queen(1, 28, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        bishop(2, 9, piece, pc_id, x, y, 2);
        break;
    case 926:
        queen(1, 28, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 928:
        queen(1, 28, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        rook(3, 9, piece, pc_id, x, y, 2);
        bishop(2, 9, piece, pc_id, x, y, 2);
        valk(9, piece, pc_id, x, y);
        break;
    case 930: // Lust
        bishop(1, 3, piece, pc_id, x, y);
        rook(1, 1, piece, pc_id, x, y);
        break;
    case 932:
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 934:
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 936:
        queen(1, 3, piece, pc_id, x, y);
        knight(9, piece, pc_id, x, y);
        break;
    case 938: // Moonfox
        bishop(3, 3, piece, pc_id, x, y);
        break;
    case 940:
        bishop(4, 3, piece, pc_id, x, y);
        break;
    case 942:
        bishop(5, 3, piece, pc_id, x, y);
        break;
    case 944:
        bishop(5, 3, piece, pc_id, x, y);
        rookForward(1, 4, piece, pc_id, x, y);
        break;
    case 946: // Phoenix
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 948:
        bishop(1, 3, piece, pc_id, x, y);
        rook(1, 13, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 950:
        queen(1, 13, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y, 2);
        break;
    case 952:
        queen(1, 13, piece, pc_id, x, y);
        queen(2, 10, piece, pc_id, x, y, 2);
        break;
    case 954: // Pheonix egg
        break;
    case 956:
        break;
    case 958:
        break;
    case 960:
        break;
    case 962: // Pride
        queen(1, 3, piece, pc_id, x, y);
        break;
    case 964:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 966:
        queen(2, 3, piece, pc_id, x, y);
        break;
    case 968:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 13, piece, pc_id, x, y);
        break;
    case 970: // Royal Guard
        rook(1, 3, piece, pc_id, x, y);
        break;
    case 972:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        break;
    case 974:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 976:
        rook(1, 13, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        break;
    case 978: // Siren
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        rook(6, 40, piece, pc_id, x, y, 4);
        break;
    case 980:
        queen(1, 3, piece, pc_id, x, y);
        rook(6, 40, piece, pc_id, x, y, 4);
        break;
    case 982:
        queen(1, 3, piece, pc_id, x, y);
        rook(6, 40, piece, pc_id, x, y, 4);
        knight(9, piece, pc_id, x, y);
        break;
    case 984:
        queen(1, 3, piece, pc_id, x, y);
        rook(6, 40, piece, pc_id, x, y, 4);
        rook(2, 4, piece, pc_id, x, y, 2);
        knight(9, piece, pc_id, x, y);
        break;
    case 986: // Soulkeeper
        rook(1, 41, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        break;
    case 988:
        rook(1, 41, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y);
        break;
    case 990:
        queen(1, 41, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y);
        break;
    case 992:
        queen(1, 41, piece, pc_id, x, y);
        knight(13, piece, pc_id, x, y);
        queen(2, 9, piece, pc_id, x, y);
        valk(9, piece, pc_id, x, y);
        break;
    case 994: // Taurus
        rook(1, 3, piece, pc_id, x, y);
        bishop(2, 42, piece, pc_id, x, y, 2);
        rook(3, 42, piece, pc_id, x, y, 3);
        break;
    case 996:
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 42, piece, pc_id, x, y, 2);
        rook(3, 42, piece, pc_id, x, y, 3);
        break;
    case 998:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        bishop(2, 42, piece, pc_id, x, y, 2);
        rook(3, 42, piece, pc_id, x, y, 3);
        break;
    case 1000:
        rook(2, 3, piece, pc_id, x, y);
        bishop(1, 3, piece, pc_id, x, y);
        bishop(2, 42, piece, pc_id, x, y, 2);
        rook(3, 42, piece, pc_id, x, y, 3);
        break;
    case 1002: // Vampire
        queen(1, 3, piece, pc_id, x, y);
        queen(3, 4, piece, pc_id, x, y, 2);
        knight(35, piece, pc_id, x, y);
        break;
    case 1004:
        queen(1, 3, piece, pc_id, x, y);
        queen(4, 4, piece, pc_id, x, y, 2);
        knight(35, piece, pc_id, x, y);
        break;
    case 1006:
        queen(1, 3, piece, pc_id, x, y);
        queen(5, 4, piece, pc_id, x, y, 2);
        knight(35, piece, pc_id, x, y);
        break;
    case 1008:
        queen(1, 3, piece, pc_id, x, y);
        queen(6, 4, piece, pc_id, x, y, 2);
        knight(35, piece, pc_id, x, y);
        doubleKnight(35, piece, pc_id, x, y);
        break;
    case 1010: // Voidmage
        rook(2, 43, piece, pc_id, x, y);
        bishop(1, 10, piece, pc_id, x, y);
        break;
    case 1012:
        rook(2, 43, piece, pc_id, x, y);
        bishop(1, 13, piece, pc_id, x, y);
        break;
    case 1014:
        rook(3, 43, piece, pc_id, x, y);
        bishop(1, 13, piece, pc_id, x, y);
        break;
    case 1016:
        rook(3, 43, piece, pc_id, x, y);
        bishop(1, 13, piece, pc_id, x, y);
        bishop(2, 10, piece, pc_id, x, y, 2);
        break;
    case 1018: // Waterelmental
        queen(1, 3, piece, pc_id, x, y);
        knight(18, piece, pc_id, x, y);
        break;
    case 1020:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 1, piece, pc_id, x, y, 2);
        knight(18, piece, pc_id, x, y);
        break;
    case 1022:
        queen(2, 3, piece, pc_id, x, y);
        knight(18, piece, pc_id, x, y);
        break;
    case 1024:
        queen(1, 3, piece, pc_id, x, y);
        queen(2, 13, piece, pc_id, x, y, 2);
        knight(18, piece, pc_id, x, y);
        break;
    case 1026: // Wrath
        rook(1, 3, piece, pc_id, x, y);
        bishop(1, 1, piece, pc_id, x, y);
        valk(24, piece, pc_id, x, y);
        break;
    case 1028:
        queen(1, 1, piece, pc_id, x, y);
        valk(24, piece, pc_id, x, y);
        break;
    case 1030:
        queen(1, 1, piece, pc_id, x, y);
        bishop(2, 24, piece, pc_id, x, y, 2);
        valk(24, piece, pc_id, x, y);
        break;
    case 1032:
        queen(1, 1, piece, pc_id, x, y);
        bishop(2, 24, piece, pc_id, x, y, 2);
        rook(2, 1, piece, pc_id, x, y, 2);
        valk(24, piece, pc_id, x, y);
        break;
    case 1034: //king 
        queen(1, 3, piece, pc_id, x, y);
        //castling to be added later
        break;
    case 1036: //sapling
        break;
    case 1038: //tree
        break;
    case 1040: //bonepile
        break;
    case 1042: //stonepillar
        break;
    }
    
}

void addPVT(int pc, int x, int y){
    position_bonus[pc&1]+=piece_square_tables[piece_type[pc/2]][pc&1 ? y : 7-y][x];
}

void removePVT(int pc, int x, int y){
    position_bonus[pc&1]-=piece_square_tables[piece_type[pc/2]][pc&1 ? y : 7-y][x];
}

/*
56,Gustavus_Adolph#4253,Guhbuh#8296,3000,4500,v56_replay,7,6,4,1,1,1,0,152,46,36,31,31,5,0,White,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,Lust4,Spider,,,,,Drake,Gemini3,FireMage,Samurai,,,,,Drake,Alchemist3,Phalanx4,Samurai,,,,,Pikeman3,Fencer2,EarthElemental4,Samurai,,,,,Mercenary,NullMage3,EarthElemental4,Samurai,,,,,Mercenary,Ranger4,King,Samurai,,,,,Pikeman3,Fencer2,ThunderMage4,Samurai,,,,,Bomber,Lich4,Lust4,Spider,,,,,Bomber,King,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,52,45,1,18,48,34,8,26,34,33,11,20,60,52,7,22,52,44,26,17,49,35,18,42,35,34,18,8,45,38,22,21,34,52,9,16,51,43,15,29,30,38,29,36,33,41,36,44,57,51,2,11,44,36,20,29,36,44,29,38,54,46,0,9,56,57,13,22,53,45,11,18,51,34,18,19,34,16,9,18,34,40,18,9,52,34,14,23,50,42,4,13,40,50,13,37,43,36,8,18,26,40,18,42,34,35,6,5,42,34,5,45,36,37,21,11,28,12,11,12,20,12,3,35,45,53,18,26,44,35,19,20,35,42,9,19,53,45,17,35,58,51,18,28,50,44,28,34,42,43,23,30,59,43,30,37,45,37,19,28,44,61,28,36,36,28,20,28,62,45,5,21,51,44,28,27,62,54,35,43,61,51,35,49,43,44,27,20,43,36,34,28,36,28,21,28,57,49,28,35,44,46,35,28,40,26,28,19,26,10,3,10,59,52,20,28


*/
void moveToSquare(int x, int y, int xx, int yy, int pc_id);
void killPiece(int xx, int yy, int pc_id, int killType);
void swap(int x, int y, int xx, int yy, int pc_id);
void inflictStatus(int nstatus, int xx, int yy, int pc_id);
void pushPiece(int x, int y, int xx, int yy, int pc_id, int dis);
void summonPiece(int xx, int yy, int pieceType);
void markSquare(int mark, int xx, int yy);
void payMorale(int side, int qnt);
void replacePiece(int xx, int yy, int pc_id, int nPieceType);
void increaseValue(int pc_id, int amount);

int undostack[200][200][6] = {};
int undo_pnt[200] = {};

void moveToSquare(int x, int y, int xx, int yy, int pc_id){
    //std::cout<<"move "<<x<<" "<<y<<" "<<xx<<" "<<yy<<std::endl;
    if(yy==7&&(pieces[pc_id]&1)==0&&promotions[board[x][y]/2]!=0)
        replacePiece(x, y, pc_id, board[x][y]+promotions[board[x][y]/2]);
    if(yy==0&&((pieces[pc_id]&1)==1)&&promotions[board[x][y]/2]!=0){
        replacePiece(x, y, pc_id, board[x][y]+promotions[board[x][y]/2]);
        //std::cout<<"PROMOTE"<<std::endl;
    }
    //std::cout<<"move "<<x<<" "<<y<<" "<<xx<<" "<<yy<<" "<<promotions[board[x][y]/2]<<" "<<((pieces[pc_id]&1)==1)<<" "<<(yy==0&&(pieces[pc_id]&1)==1&&promotions[board[x][y]/2]!=0)<<std::endl;
    
    if(!death[pc_id]){
        removePVT(pieces[pc_id], x, y);
        addPVT(pieces[pc_id], xx, yy);
    }
    
    board[xx][yy]=board[x][y];
    id_board[xx][yy]=pc_id;
    board[x][y]=0;
    id_board[x][y]=-1;
    moved[pc_id]++;
    px[pc_id]=xx;
    py[pc_id]=yy;

    hash^=zboard[xx][yy][board[xx][yy]];
    hash^=zboard[x][y][board[xx][yy]];

    undostack[turn][undo_pnt[turn]][0]=0;
    undostack[turn][undo_pnt[turn]][1]=x;
    undostack[turn][undo_pnt[turn]][2]=y;
    undostack[turn][undo_pnt[turn]][3]=xx;
    undostack[turn][undo_pnt[turn]][4]=yy;
    undostack[turn][undo_pnt[turn]][5]=pc_id;
    undo_pnt[turn]++;
}


void killPiece(int xx, int yy, int pc_id, int killType=0){
    int takingPiece= pc_id==-1 ? 0 : pieces[pc_id];
    int capturedPiece=board[xx][yy];
    switch(takingPiece&(2048-2)){
        case 34: //bat
        case 36:
        case 38:
        case 40:
            increaseValue(king_id[takingPiece&1], 1);
            payMorale((takingPiece&1)^1, 1);
            break;
        case 50: //bomber
        case 52:
        case 54:
        case 56:
            killPiece(px[pc_id], py[pc_id], -1, -1);
            break;
        case 82: //fireball //incorrect behavior supposed to "vanish" 
        case 84:
        case 86:
        case 88:
            killPiece(px[pc_id], py[pc_id], -1);
            break;
        case 186: //slime
        case 188:
        case 190:
        case 192:
            replacePiece(px[pc_id], py[pc_id], pc_id, takingPiece+152);
            break;
        case 434: //prince
        case 436:
        case 438:
        case 440:
            if(king_id[takingPiece&1]==-1||death[king_id[takingPiece&1]]){
                replacePiece(px[pc_id], py[pc_id], pc_id, 1034+(takingPiece&1));
                payMorale(takingPiece&1, -20);
                int prevKing_id=king_id[takingPiece&1];
                king_id[takingPiece&1]=pc_id;
                undostack[turn][undo_pnt[turn]][0]=10;
                undostack[turn][undo_pnt[turn]][1]=takingPiece&1;
                undostack[turn][undo_pnt[turn]][2]=prevKing_id;
                undo_pnt[turn]++;
            }
            break;
        case 538: //airelemental
        case 540:
        case 542:
        case 544:
            increaseValue(pc_id, -6);
            if(pmorale[pc_id]<=0)
                replacePiece(px[pc_id], py[pc_id], pc_id, takingPiece-40);
            break;
        case 658: //mercenary
        case 660:
        case 662:
        case 664:
            if(!isMinion[capturedPiece/2]){
                replacePiece(px[pc_id], py[pc_id], pc_id, takingPiece^1);
                payMorale((takingPiece^1)&1, 1);
            }
            break;
        case 666: //minotaur
        case 668:
        case 670:
        case 672:
            if(isChampion[capturedPiece/2]&&king_id[(takingPiece^1)&1]!=-1&&death[king_id[(takingPiece^1)&1]]==0)
                swap(px[pc_id], py[pc_id], px[king_id[(takingPiece^1)&1]], py[king_id[(takingPiece^1)&1]], pc_id);
            break;
        case 722: //reaver
            payMorale((takingPiece&1)^1, 3);
            break;
        case 724:
            payMorale((takingPiece&1)^1, 4);
            break;
        case 726: 
            payMorale((takingPiece&1)^1, 7);
            break;
        case 728:
            payMorale((takingPiece&1)^1, 9);
            break;
        case 842: //fireelemental
            increaseValue(pc_id, 3);
            break;
        case 844:
            increaseValue(pc_id, 4);
            break;
        case 846:
            increaseValue(pc_id, 5);
            break;
        case 848:
            increaseValue(pc_id, 6);
            break;
        case 962: //pride
        case 964:
        case 966:
        case 968:
            increaseValue(pc_id, pmorale[id_board[xx][yy]]/2);
            break;
        case 1002: //vampire
            if(king_id[takingPiece&1]!=-1&&death[king_id[takingPiece&1]]==0)
                increaseValue(king_id[takingPiece&1], 2);
            else
                increaseValue(pc_id, 2);
            break;
        case 1004:
            if(king_id[takingPiece&1]!=-1&&death[king_id[takingPiece&1]]==0)
                increaseValue(king_id[takingPiece&1], 3);
            else
                increaseValue(pc_id, 3);
            break;
        case 1006:
            if(king_id[takingPiece&1]!=-1&&death[king_id[takingPiece&1]]==0)
                increaseValue(king_id[takingPiece&1], 4);
            else
                increaseValue(pc_id, 4);
            break;
        case 1008:
            if(king_id[takingPiece&1]!=-1&&death[king_id[takingPiece&1]]==0)
                increaseValue(king_id[takingPiece&1], 5);
            else
                increaseValue(pc_id, 5);
            break;
    }

    switch(capturedPiece&(2048-2)){
        case 50: //bomber
        case 52:
        case 54:
        case 56:
            //std::cout<<"captured bomber"<<std::endl;
            if(killType==0)
                killPiece(px[pc_id], py[pc_id], -1, -1);
            break;
        case 138: //militia
            payMorale(capturedPiece&1, 4);
            break;
        case 140:
            payMorale(capturedPiece&1, 5);
            break;
        case 142:
            payMorale(capturedPiece&1, 6);
            break;
        case 144:
            payMorale(capturedPiece&1, 7);
            break;
        case 290: //dove
        case 292:
        case 294:
        case 296:{
            for(int dx=std::max(-1, -1*xx); dx<=std::min(1, 7-xx); dx++)
                for(int dy=std::max(-1, -1*yy); dx<=std::min(1, 7-yy); dy++)
                    if((board[xx+dx][yy+dy]^(capturedPiece&1))==0 && isChampion[board[xx+dx][yy+dy]/2])
                        inflictStatus(32, xx+dx, yy+dy, id_board[xx+dx][yy+dy]);
        }
        case 330: //frostmphit
        case 332:
        case 334:
        case 336:
            if(killType==0)
                inflictStatus(11, px[pc_id], py[pc_id], pc_id);
            break;
        case 338: //giantslime
        case 340:
        case 342:
        case 344:
        {
            if(killType!=0)
                break;
            int ydir = -1+2*(capturedPiece&1);
            if(xx>=1&&yy+ydir>=0&&yy+ydir<=7&&board[xx-1][yy+ydir]==0)
                summonPiece(xx-1, yy+ydir, 186+(takingPiece&1));
            if(xx<=6&&yy+ydir>=0&&yy+ydir<=7&&board[xx+1][yy+ydir]==0)
                summonPiece(xx+1, yy+ydir, 186+(takingPiece&1));
            break;
        }
        case 354: //hostage
            payMorale(1-(capturedPiece&1), 8);
            break;
        case 356:
            payMorale(1-(capturedPiece&1), 10);
            break;
        case 358:
            payMorale(1-(capturedPiece&1), 12);
            break;
        case 360:
            payMorale(1-(capturedPiece&1), 14);
            break;
        case 418: //pikeman
        case 420:
        case 422:
        case 424:
        {
            int dx = xx-px[pc_id];
            if(dx==0)
                break;
            int dy = yy-py[pc_id];
            if(std::abs(dx)<=2&&std::abs(dy)+std::abs(dx)==3&& dy * (-1+(2*capturedPiece&1))>0)
                if(killType==0)
                    killPiece(px[pc_id], py[pc_id], -1, -1);
            break;
        }
        case 482: //snake
        case 484:
        case 486:
        case 488:
            if(killType==0&&pmorale[pc_id]<=10&&!poison_immune[pieces[pc_id]])
                inflictStatus(0, px[pc_id], py[pc_id], pc_id);
            break;
        case 5: //wisp NO
            break;
        case 642: //lifestone
            payMorale(capturedPiece&1, 5);
            break;
        case 644:
            payMorale(capturedPiece&1, 6);
            break;
        case 646:
            payMorale(capturedPiece&1, 7);
            break;
        case 648:
            payMorale(capturedPiece&1, 8);
            break;
        case 698: //phalanx
        case 700:
        case 702:
        case 704:
        {
            int dx = xx-px[pc_id];
            if(dx==0)
                break;
            int dy = yy-py[pc_id];
            if(std::abs(dx)<=2&&std::abs(dy)+std::abs(dx)==3)
                if(killType==0)
                    killPiece(px[pc_id], py[pc_id], -1, -1);
            break;
        }    
        case 794: //angel
        case 796:
        case 798:
        case 800:
        {
            for(int i=0; i<pc_cnt; i++){
                if(isMinion[pieces[i]/2]&&((pieces[i]^capturedPiece)&1)==0)
                    inflictStatus(34, xx, yy, -1); //enchant 3
            }
        }
        break;
        case 890: //greed
            payMorale(capturedPiece&1, 4);
            break;
        case 892:
            payMorale(capturedPiece&1, 12);
            break;
        case 894:
            payMorale(capturedPiece&1, 19);
            break;
        case 896:
            payMorale(capturedPiece&1, 25);
            break;
        case 938: //moonfox
        case 940:
        case 942:
        case 944:
            payMorale(capturedPiece&1, 2);
            if(killType==0){
                int ym=1-2*(capturedPiece&1);
                if(board[xx][yy-ym]==0){
                    summonPiece(xx, yy-ym, capturedPiece);
                    increaseValue(id_board[xx][yy-ym], -12);
                }
            }
            break;
        case 946: //phoenix
        case 948:
        case 950:
        case 952:
            if(killType==0)
                killPiece(px[pc_id], py[pc_id], -1);
            break;
        case 954: //phoenixegg
            payMorale(capturedPiece&1, 8);
            break;
        case 956:
            payMorale(capturedPiece&1, 7);
            break;
        case 958:
            payMorale(capturedPiece&1, 6);
            break;
        case 960:
            payMorale(capturedPiece&1, 4);
            break;
        case 962: //pride
        case 964:
        case 966:
        case 968:
            if(king_id[capturedPiece&1]!=-1&&death[king_id[capturedPiece&1]]==0)
                increaseValue(king_id[capturedPiece&1], pmorale[id_board[xx][yy]]/2);
            break;
        case 1034: //king
            payMorale(capturedPiece&1, 22);
            break;
    }

    
    undostack[turn][undo_pnt[turn]][0]=1;
    undostack[turn][undo_pnt[turn]][1]=xx;
    undostack[turn][undo_pnt[turn]][2]=yy;
    undostack[turn][undo_pnt[turn]][3]=id_board[xx][yy];
    undo_pnt[turn]++;

    removePVT(capturedPiece, xx, yy);
    hash^=zmorale[capturedPiece&1][morale[capturedPiece&1]+50];
    hash^=zboard[xx][yy][capturedPiece];
    hash^=zstatus(id_board[xx][yy]);

    morale[capturedPiece&1]-=pmorale[id_board[xx][yy]];
    board[xx][yy]=0;
    death[id_board[xx][yy]]=1;
    id_board[xx][yy]=-1;

    hash^=zmorale[capturedPiece&1][morale[capturedPiece&1]+50];

    switch(capturedPiece&(2048-2)){
        case 946: //phoenix
        case 948:
        case 950:
        case 952:
            if(killType==0)
                summonPiece(xx, yy, capturedPiece+8);
            break;
    }
    
    //dove
    for(int i=0; i<dovePnt[capturedPiece&1]; i++){
        int dove = doveList[capturedPiece&1][i];
        if(death[dove] || pieces[dove]<290 || pieces[dove]>=298)
            if(board[px[dove]][py[dove]+1-2*(capturedPiece&1)]==0)
                moveToSquare(px[dove], py[dove], px[dove], py[dove]+1-2*(capturedPiece&1), dove);
    }

    return;
}

void swap(int x, int y, int xx, int yy, int pc_id){
    undostack[turn][undo_pnt[turn]][0]=2;
    undostack[turn][undo_pnt[turn]][1]=x;
    undostack[turn][undo_pnt[turn]][2]=y;
    undostack[turn][undo_pnt[turn]][3]=xx;
    undostack[turn][undo_pnt[turn]][4]=yy;
    undo_pnt[turn]++;

    removePVT(board[x][y], x, y);
    removePVT(board[xx][yy], xx, yy);

    hash^=zboard[x][y][board[x][y]];
    hash^=zboard[xx][yy][board[xx][yy]];

    std::swap(board[xx][yy], board[x][y]);
    std::swap(id_board[xx][yy], id_board[x][y]);

    std::swap(px[id_board[x][y]], px[id_board[xx][yy]]);
    std::swap(py[id_board[x][y]], py[id_board[xx][yy]]);

    addPVT(board[x][y], x, y);
    addPVT(board[xx][yy], xx, yy);

    hash^=zboard[x][y][board[x][y]];
    hash^=zboard[xx][yy][board[xx][yy]];

    moved[id_board[xx][yy]]++;
    moved[id_board[x][y]]++;
}

void inflictStatus(int nstatus, int xx, int yy, int pc_id){
    if(board[xx][yy]>=82 && board[xx][yy] <= 90 && nstatus>=7 && nstatus<=13)
        return killPiece(xx, yy, pc_id, 1);
    if((status[id_board[xx][yy]]&(1LL<<nstatus))>0LL)
        return;
    hash^=zstatus(id_board[xx][yy]);
    status[id_board[xx][yy]]|=(1LL<<nstatus);
    hash^=zstatus(id_board[xx][yy]);

    undostack[turn][undo_pnt[turn]][0]=3;
    undostack[turn][undo_pnt[turn]][1]=id_board[xx][yy];
    undostack[turn][undo_pnt[turn]][2]=nstatus;
    undo_pnt[turn]++;
}

void pushPiece(int x, int y, int xx, int yy, int pc_id, int dis=3){
    int xdif = xx-x;
    int ydif = yy-y;
    if(abs(xdif)!=abs(ydif)){
        if(abs(xdif)>abs(ydif))
            ydif=0;
        else
            xdif=0;
    }
    int lvx=xx;
    int lvy=yy;
    for(int i=1; i<=dis; i++){
        if(0<=xx+xdif*i&&xx+xdif*i<=7&&yy+ydif*i>=0&&yy+ydif*i<=7)
            break;
        else if(board[xx+xdif*i][yy+ydif*i]==0){
            lvx=xx+xdif*i;
            lvy=yy+ydif*i;
        }
        else if(board[xx+xdif*i][yy+ydif*i]&&transparent[id_board[xx][yy]])
            continue;
        break;
    }
    if(lvx!=xx||lvy!=yy)
        moveToSquare(xx, yy, lvx, lvy, id_board[xx][yy]);
    
    //sylph promote
    if(pieces[pc_id]>=498 && pieces[pc_id]<504 && ((board[lvx][lvy]^pieces[pc_id])&1)){
        replacePiece(px[pc_id], py[pc_id], pc_id, pieces[pc_id]+2);
    }
    else if(pieces[pc_id]>=504 && pieces[pc_id]<506 && ((board[lvx][lvy]^pieces[pc_id])&1)){
        replacePiece(px[pc_id], py[pc_id], pc_id, pieces[pc_id]+34);
    }
    //air elemental promote/summon sylph
    else if(pieces[pc_id]>=538&&pieces[pc_id]<544 && ((board[lvx][lvy]^pieces[pc_id])&1)){
        replacePiece(px[pc_id], py[pc_id], pc_id, pieces[pc_id]+2);
    }
    else if(pieces[pc_id]>=544&&pieces[pc_id]<546 && ((board[lvx][lvy]^pieces[pc_id])&1)){
        summonPiece(xx, yy, 498 + (pieces[pc_id]&1));
    }
}

void summonPiece(int xx, int yy, int pieceType){
    pieces[pc_cnt]=pieceType;
    id_board[xx][yy]=pc_cnt;
    board[xx][yy]=pieceType;
    
    hash^=zboard[xx][yy][pieceType];

    pmorale[pc_cnt]=pieceValue[pieceType/2];

    hash^=zmorale[pieceType&1][morale[pieceType&1]+50];
    morale[pieceType&1]+=pmorale[pc_cnt];
    hash^=zmorale[pieceType&1][morale[pieceType&1]+50];

    px[pc_cnt]=xx;
    py[pc_cnt]=yy;

    transparent[pc_cnt]=0;

    pc_cnt++;

    addPVT(pieceType, xx, yy);

    if(pieceType>=242&&pieceType<250){ //alch
        alchList[pieceType&1][alchPnt[pieceType&1]++]=pc_cnt-1;
    }

    if(pieceType>=290 && pieceType<298){
        doveList[pieceType&1][dovePnt[pieceType&1]++]=pc_cnt-1;
    }

    if(pieceType>=474&&pieceType<482){ //samurai
        samuraiList[pieceType&1][samuraiPnt[pieceType&1]++]=pc_cnt-1;
    }

    if(pieceType>=930&&pieceType<938){ //lust
        lustList[pieceType&1][lustPnt[pieceType&1]++]=pc_cnt-1;
    }

    //transparency
    if(pieceType >= 26 && pieceType<34 || pieceType>=106&&pieceType<114 || pieceType>=410 && pieceType<418 || pieceType>=610 && pieceType<618)
        transparent[pc_cnt-1]=1;

    undostack[turn][undo_pnt[turn]][0]=4;
    undostack[turn][undo_pnt[turn]][1]=pc_cnt-1;
    undo_pnt[turn]++;
}

void markSquare(int mark, int xx, int yy){
    if(mark==0){
        if(lightning[xx][yy]>=turn)
            return;
        lightning[xx][yy]=turn+8;
        lightning_cnt++;
        hash^=zlightning[xx][yy];
    }
    else if(mark==1){
        if(meteor[xx][yy]>=turn)
            return;
        meteor[xx][yy]=turn+12;
        meteor_cnt++;
        hash^=zmeteor[xx][yy];
    }
    else if(mark==2){
        if(butterfly[xx][yy]>=turn)
            return;
        butterfly[xx][yy]=turn+40;
        butterfly_cnt++;
    }
    undostack[turn][undo_pnt[turn]][0]=5;
    undostack[turn][undo_pnt[turn]][1]=mark;
    undostack[turn][undo_pnt[turn]][2]=xx;
    undostack[turn][undo_pnt[turn]][3]=yy;
    undo_pnt[turn]++;
}

void payMorale(int side, int qnt){
    hash^=zmorale[side][morale[side]+50];
    morale[side]-=qnt;
    hash^=zmorale[side][morale[side]+50];

    undostack[turn][undo_pnt[turn]][0]=6;
    undostack[turn][undo_pnt[turn]][1]=side;
    undostack[turn][undo_pnt[turn]][2]=qnt;
    undo_pnt[turn]++;
}

//the pc_id HERE is the piece being replaced
void replacePiece(int xx, int yy, int pc_id, int nPieceType){
    undostack[turn][undo_pnt[turn]][0]=7;
    undostack[turn][undo_pnt[turn]][1]=pc_id;
    undostack[turn][undo_pnt[turn]][2]=board[xx][yy]+2048*transparent[pc_id];
    undostack[turn][undo_pnt[turn]][3]=pmorale[pc_id];
    undo_pnt[turn]++;

    removePVT(board[xx][yy], xx, yy);
    addPVT(nPieceType, xx, yy);

    hash^=zmorale[0][morale[0]+50];
    hash^=zmorale[1][morale[1]+50];

    morale[pieces[pc_id]&1]-=pmorale[pc_id];
    morale[nPieceType&1]+=pieceValue[nPieceType/2];

    hash^=zmorale[0][morale[0]+50];
    hash^=zmorale[1][morale[1]+50];
    hash^=zboard[xx][yy][board[xx][yy]];

    board[xx][yy]=nPieceType;
    pmorale[pc_id]=pieceValue[nPieceType/2];
    pieces[pc_id]=nPieceType;

    hash^=zboard[xx][yy][board[xx][yy]];

    if(nPieceType>=106 && nPieceType<114)
        transparent[pc_id]=1;
}

void increaseValue(int pc_id, int amount){
    undostack[turn][undo_pnt[turn]][0]=8;
    undostack[turn][undo_pnt[turn]][1]=pc_id;
    undostack[turn][undo_pnt[turn]][2]=pmorale[pc_id];
    undo_pnt[turn]++;

    hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];

    pmorale[pc_id]+=amount;
    morale[pieces[pc_id]&1]+=amount;

    hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];
}

void cureUnit(int xx, int yy, int pc_id){
    undostack[turn][undo_pnt[turn]][0]=9;
    undostack[turn][undo_pnt[turn]][1]=pc_id;
    undostack[turn][undo_pnt[turn]][2]=(int)(status[pc_id]>>30);
    undostack[turn][undo_pnt[turn]][3]=(int)(status[pc_id]&((1<<30)-1));
    undo_pnt[turn]++;

    hash^=zstatus(pc_id);

    status[pc_id]>>=32;
    status[pc_id]<<=32; 

    hash^=zstatus(pc_id);
}

void nullPiece(int xx, int yy, int pc_id){
    undostack[turn][undo_pnt[turn]][0]=12;
    undostack[turn][undo_pnt[turn]][1]=id_board[xx][yy];
    undostack[turn][undo_pnt[turn]][2]=board[xx][yy]+2048*transparent[id_board[xx][yy]];
    undostack[turn][undo_pnt[turn]][3]=pmorale[id_board[xx][yy]];
    undo_pnt[turn]++;

    nulled[id_board[xx][yy]]=1;
    transparent[id_board[xx][yy]]=1;

    hash^=znulled[id_board[xx][yy]];
    hash^=zmorale[board[xx][yy]&1][morale[board[xx][yy]&1]+50];

    morale[board[xx][yy]&1]-=pmorale[id_board[xx][yy]];
    pmorale[id_board[xx][yy]]=0;

    hash^=zmorale[board[xx][yy]&1][morale[board[xx][yy]&1]+50];
}

void unenchant(int xx, int yy, int pc_id){
    undostack[turn][undo_pnt[turn]][0]=13;
    undostack[turn][undo_pnt[turn]][1]=pc_id;
    undostack[turn][undo_pnt[turn]][2]=(int)(status[pc_id]>>30);
    undostack[turn][undo_pnt[turn]][3]=(int)(status[pc_id]&((1<<30)-1));
    undo_pnt[turn]++;

    hash^=zstatus(pc_id);

    status[pc_id]&=(1L<<32)-1;

    hash^=zstatus(pc_id);
}

//uses the refined list of 31 moves -> 10 or so operations
void makeMove(int xx, int yy, int pc_id, int moveType){
    //std::cout<<xx<<" "<<yy<<" "<<pc_id<<" "<<moveType<<std::endl;
    switch(moveType){
        case 1: //move
            moveToSquare(px[pc_id], py[pc_id], xx, yy, pc_id);
            break;
        case 2: //take
            killPiece(xx, yy, pc_id);
            moveToSquare(px[pc_id], py[pc_id], xx, yy, pc_id);
            if(isEnchanted(pc_id))
                unenchant(xx, yy, pc_id);
            break;
        case 3: //swap
        case 4: //omniswap
            swap(px[pc_id], py[pc_id], xx, yy, pc_id);
            break;
        case 5: //freeze 6
            inflictStatus(7, xx, yy, pc_id);
            break;
        case 6: //petrify 10
            inflictStatus(21, xx, yy, pc_id);
            break;
        case 7: //poison 6
            inflictStatus(0, xx, yy, pc_id);
            break;
        case 8: //void
            break;
        case 9: //null
            nullPiece(xx, yy, pc_id);
            break;
        case 10: //compel 6
            inflictStatus(14, xx, yy, pc_id);
            break;
        case 11: //enchant 5
            inflictStatus(32, xx, yy, pc_id);
            break;
        case 12: //freeze 4 push
            inflictStatus(9, xx, yy, pc_id);
            pushPiece(px[pc_id], py[pc_id], xx, yy, pc_id, 1);
            break;
        case 13: //push
            pushPiece(px[pc_id], py[pc_id], xx, yy, pc_id);
            break;
        case 14: //destroy target
            killPiece(xx, yy, pc_id, 1);
            break;
        case 15: //transfrom into sapling
            killPiece(xx, yy, pc_id, 2);
            summonPiece(xx, yy, 1036+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 2);
            break;
        case 16: //transform into bonepile
            killPiece(xx, yy, pc_id, 2);
            summonPiece(xx, yy, 1040+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 2);
            break;
        case 17: //transform into ghost
            killPiece(xx, yy, pc_id, 2);
            summonPiece(xx, yy, 106+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 1);
            break;
        case 18: //lightning
            markSquare(0, xx, yy);
            payMorale(pieces[pc_id]&1, 1);
            break;
        case 19: //meteor
            markSquare(1, xx, yy);
            payMorale(pieces[pc_id]&1, 2);
            break;
        case 20: //butterfly
            moveToSquare(px[pc_id], py[pc_id], xx, yy, pc_id);
            killPiece(xx, yy, pc_id, 1);
            markSquare(2, xx, yy);
            break;
        case 21: //summon skeleton
            summonPiece(xx, yy, 178+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 4);
            break;
        case 22: //summon sapling
            summonPiece(xx, yy, 1036+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 2);
            break;
        case 23: //summon stone pillar
            summonPiece(xx, yy, 1042+(pieces[pc_id]&1));
            payMorale(pieces[pc_id]&1, 1);
            break;
        case 24: //summon geminitwin
            summonPiece(xx, yy, pieces[pc_id]+8);
            payMorale(pieces[pc_id]&1, 4);
            replacePiece(xx, yy, pc_id, pieces[pc_id]+8);
            break;
        case 25: //transform to bat
            moveToSquare(px[pc_id], py[pc_id], xx, yy, pc_id);
            replacePiece(xx, yy, pc_id, pieces[pc_id]-968);
            break;
        case 26: //envy
            replacePiece(px[pc_id], py[pc_id], pc_id, board[px[pc_id]][py[pc_id]]^1);
            break;
        case 27: //gravity
            break;
        case 28: //jump attacker
            break;
        case 29: //charm
            if(pieces[pc_id]>=738&&pieces[pc_id]<746)
                killPiece(px[pc_id], py[pc_id], pc_id);
            replacePiece(xx, yy, id_board[xx][yy], board[xx][yy]^1);
            break;
        case 30: //rush
            break;
        case 31: //upgrade skeleton
            replacePiece(xx, yy, id_board[xx][yy], board[xx][yy]+2);
            break;
        case 32: //check armor
        {
            int tar = board[xx][yy];
            int dx = xx - px[pc_id];
            int dy = yy - py[pc_id];
            if(tar>=66&&tar<74){ //duelist
                if(dx==0&&dy*(1-2*(tar&1))){
                    replacePiece(xx, yy, id_board[xx][yy], tar+8);
                    break;
                }
            }
            else if(tar>=274&&tar<282) //crusader
            {
                if(tar<280){
                    if(std::abs(dx)+std::abs(dy)==1){
                        replacePiece(xx, yy, id_board[xx][yy], tar+8);
                        break;
                    }
                }
                else if(std::abs(dx)+std::abs(dy)<=2&&std::abs(dx)!=std::abs(dy)){
                    replacePiece(xx, yy, id_board[xx][yy], tar+8);
                    break;
                }
            }
            else if(tar>=306&&tar<314){ //fencer
                if(tar<310){
                    if(std::abs(dx)+std::abs(dy)==1){
                        replacePiece(xx, yy, id_board[xx][yy], tar+8);
                        break;
                    }
                }
                else if(std::abs(dx)+std::abs(dy)<=2&&std::abs(dx)!=std::abs(dy)){
                    replacePiece(xx, yy, id_board[xx][yy], tar+8);
                    break;
                }
            }
            else if(tar>=626&&tar<634){ //haunted armor
                if(std::abs(dx)<=1&&std::abs(dy)<=1){
                    replacePiece(xx, yy, id_board[xx][yy], tar+8);
                    break;
                }
            }
            else if(tar>=698&&tar<706){ //phalanx
                if(std::abs(dx)<=1&&std::abs(dy)<=1){
                    replacePiece(xx, yy, id_board[xx][yy], tar+8);
                    break;
                }
            }
            killPiece(xx, yy, pc_id);
            moveToSquare(px[pc_id], py[pc_id], xx, yy, pc_id);
            if(isEnchanted(pc_id))
                unenchant(xx, yy, pc_id);
            break;
        }
    }
}


void unmakeMoves(int tur){
    for(; undo_pnt[tur]>0; undo_pnt[tur]--){
        int i = undo_pnt[tur]-1;
        int mT= undostack[tur][i][0];
        switch(mT){
            case 0: 
            {
                int x=undostack[tur][i][1];
                int y=undostack[tur][i][2];
                int xx = undostack[tur][i][3];
                int yy = undostack[tur][i][4];
                int pc_id = undostack[tur][i][5];

                if(!death[pc_id]){
                    addPVT(pieces[pc_id], x, y);
                    removePVT(pieces[pc_id], xx, yy);
                }


                hash^=zboard[xx][yy][board[xx][yy]];
                hash^=zboard[x][y][board[xx][yy]];            

                board[x][y]=board[xx][yy];
                id_board[x][y]=pc_id;

                board[xx][yy]=0;
                id_board[xx][yy]=-1;

                moved[pc_id]--;
                px[pc_id]=x;
                py[pc_id]=y;
                break;
            }

            case 1:
            {
                int xx = undostack[tur][i][1];
                int yy = undostack[tur][i][2];
                int pc_id = undostack[tur][i][3];

                addPVT(pieces[pc_id], xx, yy);

                hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];
                hash^=zboard[xx][yy][pieces[pc_id]];

                morale[pieces[pc_id]&1]+=pmorale[pc_id];
                board[xx][yy]=pieces[pc_id];

                hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];
                hash^=zstatus(pc_id);

                death[pc_id]=0;
                id_board[xx][yy]=pc_id;
                break;
            }

            case 2:
            {
                int x = undostack[tur][i][1];
                int y = undostack[tur][i][2];
                int xx = undostack[tur][i][3];
                int yy = undostack[tur][i][4];

                removePVT(board[x][y], x, y);
                removePVT(board[xx][yy], xx, yy);

                hash^=zboard[x][y][board[x][y]];
                hash^=zboard[xx][yy][board[xx][yy]];            

                std::swap(board[xx][yy], board[x][y]);
                std::swap(id_board[xx][yy], id_board[x][y]);

                addPVT(board[x][y], x, y);
                addPVT(board[xx][yy], xx, yy);

                hash^=zboard[x][y][board[x][y]];
                hash^=zboard[xx][yy][board[xx][yy]];
            
                std::swap(px[id_board[x][y]], px[id_board[xx][yy]]);
                std::swap(py[id_board[x][y]], py[id_board[xx][yy]]);

                moved[id_board[xx][yy]]--;
                moved[id_board[x][y]]--;
                break;
            }

            case 3:
            {
                int pc_id = undostack[tur][i][1];
                hash^=zstatus(pc_id);
                status[pc_id]-= (1L<<undostack[tur][i][2]);
                hash^=zstatus(pc_id);
                break;
            }

            case 4:
            {
                pc_cnt--;
                removePVT(pieces[pc_cnt], px[pc_cnt], py[pc_cnt]);
                
                hash^=zboard[px[pc_cnt]][py[pc_cnt]][pieces[pc_cnt]];

                id_board[px[pc_cnt]][py[pc_cnt]]=-1;
                board[px[pc_cnt]][py[pc_cnt]]=0;

                hash^=zmorale[pieces[pc_cnt]&1][morale[pieces[pc_cnt]&1]+50];
                morale[pieces[pc_cnt]&1]-=pmorale[pc_cnt];
                hash^=zmorale[pieces[pc_cnt]&1][morale[pieces[pc_cnt]&1]+50];

                /*pmorale[pc_cnt]=0;
                px[pc_cnt]=-1;
                py[pc_cnt]=-1;
                pieces[pc_cnt]=-1;*/

                break;
            }
            
            case 5:
            {
                int mark = undostack[tur][i][1];
                int xx = undostack[tur][i][2];
                int yy = undostack[tur][i][3];    
                if(mark==0){
                    lightning_cnt--;
                    lightning[xx][yy]=0;
                    hash^=zlightning[xx][yy];
                }
                else if(mark==1){
                    meteor_cnt--;
                    meteor[xx][yy]=0;
                    hash^=zmeteor[xx][yy];
                }
                else if(mark==2){
                    butterfly_cnt--;
                    butterfly[xx][yy]=0;
                }
                break;
            }

            case 6:
            {
                int side= undostack[tur][i][1];

                hash^=zmorale[side][morale[side]+50];
                morale[side]+=undostack[tur][i][2];
                hash^=zmorale[side][morale[side]+50];
                break;
            }

            case 7:
            {
                int pc_id = undostack[tur][i][1];
                int pc = undostack[tur][i][2];
                int oval= undostack[tur][i][3];

                transparent[pc_id]=(pc&2048);
                pc&=2047;

                removePVT(pieces[pc_id], px[pc_id], py[pc_id]);

                hash^=zmorale[0][morale[0]+50];
                hash^=zmorale[1][morale[1]+50];            

                morale[pieces[pc_id]&1]-=pmorale[pc_id];
                morale[pc&1]+=oval;

                hash^=zmorale[0][morale[0]+50];
                hash^=zmorale[1][morale[1]+50];            
                hash^=zboard[px[pc_id]][py[pc_id]][board[px[pc_id]][py[pc_id]]];

                board[px[pc_id]][py[pc_id]]=pc;
                pmorale[pc_id]=oval;
                pieces[pc_id]=pc;

                hash^=zboard[px[pc_id]][py[pc_id]][board[px[pc_id]][py[pc_id]]];

                addPVT(pc, px[pc_id], py[pc_id]);

                break;            
            }

            case 8:
            {
                int pc_id = undostack[tur][i][1];
                int oval = undostack[tur][i][2];

                hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];

                morale[pieces[pc_id]&1]-=pmorale[pc_id];
                pmorale[pc_id]=oval;
                morale[pieces[pc_id]&1]+=pmorale[pc_id];

                hash^=zmorale[pieces[pc_id]&1][morale[pieces[pc_id]&1]+50];

                break;
            }

            case 9:
            {
                int pc_id = undostack[tur][i][1];

                hash^=zstatus(pc_id);

                long long ostatus= ((undostack[tur][i][2]+0LL)<<30)+undostack[tur][i][3];
                status[pc_id]=ostatus;

                hash^=zstatus(pc_id);

                break;
            }

            case 10:
            {
                int side = undostack[tur][i][1];
                int oID = undostack[tur][i][2];
                king_id[side]=oID;
                break;
            }

            case 11:
            {
                int x = undostack[tur][i][1]=x;
                int y = undostack[tur][i][2]=y;
                switch(undostack[tur][i][3]){
                    case 1:
                        meteor_cnt++;
                        meteor[x][y]=tur;
                        hash^=zmeteor[x][y];
                        break;
                    case 2:
                        butterfly_cnt++;
                        butterfly[x][y]=tur;
                        break;
                    case 3:
                        lightning_cnt++;
                        lightning[x][y]=tur;
                        hash^=zlightning[x][y];
                        break;
                }
                break;
            }

            case 12:
            {
                int pc_id = undostack[tur][i][1];
                bool trans = (undostack[tur][i][2]&2048);
                int side= (undostack[tur][i][2]&1);
                int omorale = undostack[tur][i][3];

                hash^=znulled[pc_id];

                nulled[pc_id]=0;
                transparent[pc_id]=trans;

                hash^=zmorale[side][morale[side]+50];

                pmorale[pc_id]=omorale;
                morale[side]+=omorale;

                hash^=zmorale[side][morale[side]+50];

                break;
            }

            case 13:
            {
                int pc_id = undostack[tur][i][1];

                hash^=zstatus(pc_id);
                long long ostatus= ((undostack[tur][i][2]+0LL)<<30)+undostack[tur][i][3];
                status[pc_id]=ostatus;
                hash^=zstatus(pc_id);

                break;
            }
        }
    }
}

void processStatus(int pc_id, int side){
    if(status[pc_id]==0||death[pc_id])
        return;
    
    undostack[turn][undo_pnt[turn]][0]=9;
    undostack[turn][undo_pnt[turn]][1]=pc_id;
    undostack[turn][undo_pnt[turn]][2]=(int)(status[pc_id]>>30);
    undostack[turn][undo_pnt[turn]][3]=(int)(status[pc_id]&((1<<30)-1));
    undo_pnt[turn]++;

    hash^=zstatus(pc_id);

    status[pc_id]<<=1;

    //poison
    if(status[pc_id]&(1<<6)){
        hash^=zstatus(pc_id);
        killPiece(px[pc_id], py[pc_id], -1, 1);
        return;
    }

    status[pc_id]&=(1LL<<37) - 1 - (1LL<<31) - (1LL<<20) - (1LL<<13) - (1LL<<6);
    hash^=zstatus(pc_id);


    //compel
    if((pieces[pc_id]&1)!=side&&isCompeled(pc_id)&&!isFrozen(pc_id)&&!isPetrified(pc_id))
        if(board[px[pc_id]][py[pc_id]+1-2*side]==0)
            moveToSquare(px[pc_id],py[pc_id], px[pc_id], py[pc_id]+1-2*side, pc_id);
}

//order: poison, freeze, petrify, compel, meteor, butterfly, lightning, lust, alch, samurai
void endOfTurnTriggers(int side){
    for(int i=0; i<pc_cnt; i++)
        if(status[i]!=0)
            processStatus(i, side);
    
    if(meteor_cnt!=0){
        for(int x=0; x<8; x++)
            for(int y=0; y<8; y++)
                if(meteor[x][y]==turn){
                    if(board[x][y]!=0)
                        killPiece(x, y, -1, 1);
                    for(int xx=-1; xx<2; xx++)
                        for(int yy=-1; yy<2; yy++){
                            if(board[x+xx][y+yy]==0)
                                continue;
                            pushPiece(x, y, x+xx, y+yy, -1, 1);
                        }
                    meteor_cnt--;
                    meteor[x][y]=0;
                    hash^=zmeteor[x][y];

                    undostack[turn][undo_pnt[turn]][0]=11;
                    undostack[turn][undo_pnt[turn]][1]=x;
                    undostack[turn][undo_pnt[turn]][2]=y;
                    undostack[turn][undo_pnt[turn]][3]=1;
                    undo_pnt[turn]++;
                }
    }

    /*if(butterfly_cnt!=0){
        for(int x=0; x<8; x++)
            for(int y=0; y<8; y++)
                if(meteor[x][y]==turn){
                    if(board[x][y]==0)
                        killPiece(x, y, -1, 1);
                    for(int xx=-1; xx<2; xx++)
                        for(int yy=-1; yy<2; yy++){
                            if(board[x+xx][y+yy]==0)
                                continue;
                            pushPiece(x, y, x+xx, y+yy, -1, 1);
                        }
                    meteor_cnt--;
                    meteor[x][y]=0;
                    undostack[turn][undo_pnt[turn]][0]=11;
                    undostack[turn][undo_pnt[turn]][1]=x;
                    undostack[turn][undo_pnt[turn]][2]=y;
                    undostack[turn][undo_pnt[turn]][3]=1;
                    undo_pnt[turn]++;
                }
    }*/

    if(lightning_cnt!=0){
        for(int x=0; x<8; x++)
            for(int y=0; y<8; y++)
                if(lightning[x][y]==turn){
                    if(board[x][y]!=0)
                        killPiece(x, y, -1, 1);
                    lightning_cnt--;
                    lightning[x][y]=0;
                    hash^=zlightning[x][y];
                    
                    undostack[turn][undo_pnt[turn]][0]=11;
                    undostack[turn][undo_pnt[turn]][1]=x;
                    undostack[turn][undo_pnt[turn]][2]=y;
                    undostack[turn][undo_pnt[turn]][3]=3;
                    undo_pnt[turn]++;
                }
    }

    for(int i=0; i<lustPnt[side]; i++)
    {
        if(death[lustList[side][i]]||pieces[i]<930||pieces[i]>=938)
            continue;
        int x = px[lustList[side][i]];
        int y = py[lustList[side][i]];
        for(int xx=-1; xx<=1; xx++){
            if(2*xx+x>=8)
                break;
            if(2*xx+x<0)
                continue;
            for(int yy=-1; yy<=1; yy++)
                if(y+2*yy>=0&&y+2*yy<8&&board[x+2*xx][y+2*yy]!=0&&(board[x+2*xx][y+2*yy]&1)!=side&&board[x+xx][y+yy]==0){
                    moveToSquare(2*xx+x, 2*yy+y, xx+x, yy+y, id_board[2*xx+x][2*yy+y]);
                }
                    
        }
    }



    //alchemist
    for(int i=0; i<alchPnt[side]; i++)
    {
        if(death[alchList[side][i]])
            continue;
        int x = px[alchList[side][i]];
        int y = py[alchList[side][i]];
        for(int xx=-1; xx<=1; xx++){
            if(xx+x>=8)
                break;
            if(xx+x<0)
                continue;
            for(int yy=-1; yy<=1; yy++)
                if(y+yy>=0&&y+yy<8&&board[x+xx][y+yy]!=0&&(board[x+xx][y+yy]&1)==side){
                    if(isPoisoned(id_board[x+xx][y+yy])||isPoisoned(id_board[x+xx][y+yy])||isCompeled(id_board[x+xx][y+yy])||isFrozen(id_board[x+xx][y+yy])){
                        cureUnit(x+xx, y+yy, id_board[x+xx][y+yy]);
                        payMorale(side, 1);
                    }
                }
                    
        }
    }

    //samurai
    side^=1;
    for(int i=0; i<samuraiPnt[side]; i++){
        int id=samuraiList[side][i];
        if(death[id]||pieces[id]<474||pieces[id]>481)
            continue;
        if(board[px[id]][py[id]+1-2*side]!=0&&(board[px[id]][py[id]+1-2*side]&1)!=side)
            makeMove(px[id], py[id]+1-2*side, id, 2);
    }
    side^=1;

    if(king_id[side^1]==-1||death[king_id[side^1]]){
        payMorale(side^1, 3);
    }
    if(turn>=99){
        payMorale(side^1, 1);
    }
}



int getEncoding(std::string name)
{
    //std::cout<<name<<std::endl;
    int res = -1;
    int ls = name.back()-'0';
    int mod =0;
    /*if(name.back()=='-'){
        mod=1;
        name=name.substr(0, name.size()-1);
    }*/
    if(ls>=2&&ls<=4){
        mod+=2*(ls-1);
        name=name.substr(0, name.size()-1);
    }
    if (name == "Apprentice")
        res = 2;
    else if (name == "Archer")
        res = 10;
    else if (name == "Axeman")
        res = 18;
    else if (name == "Banshee")
        res = 26;
    else if (name == "Bat")
        res = 34;
    else if (name == "Bishop")
        res = 42;
    else if (name == "Bomber")
        res = 50;
    else if (name == "Dryad")
        res = 58;
    else if (name == "Duelist")
        res = 66;
    else if (name == "**Duelist")
        res = 74;
    else if (name == "Fireball")
        res = 82;
    else if (name == "Frog")
        res = 90;
    else if (name == "FrostMage")
        res = 98;
    else if (name == "Ghost")
        res = 106;
    else if (name == "Guardian")
        res = 114;
    else if (name == "Hoplite")
        res = 122;
    else if (name == "Knight")
        res = 130;
    else if (name == "Militia")
        res = 138;
    else if (name == "Pawn")
        res = 146;
    else if (name == "Penguin")
        res = 154;
    else if (name == "PoisonMage")
        res = 162;
    else if (name == "Shieldsman")
        res = 170;
    else if (name == "Skeleton")
        res = 178;
    else if (name == "Slime")
        res = 186;
    else if (name == "Spearman")
        res = 194;
    else if (name == "Spider")
        res = 202;
    else if (name == "Swordsman")
        res = 210;
    else if (name == "Tiger")
        res = 218;
    else if (name == "Warrior")
        res = 226;
    else if (name == "WindMage")
        res = 234;
    else if (name == "Alchemist")
        res = 242;
    else if (name == "Arachnid")
        res = 250;
    else if (name == "Basilisk")
        res = 258;
    else if (name == "Berserker")
        res = 266;
    else if (name == "Crusader")
        res = 274;
    else if (name == "**Crusader")
        res = 282;
    else if (name == "Dove")
        res = 290;
    else if (name == "Drake")
        res = 298;
    else if (name == "Fencer")
        res = 306;
    else if (name == "**Fencer")
        res = 314;
    else if (name == "FireMage")
        res = 322;
    else if (name == "FrostMephit")
        res = 330;
    else if (name == "GiantSlime")
        res = 338;
    else if (name == "Gnome")
        res = 346;
    else if (name == "Hostage")
        res = 354;
    else if (name == "Hydromancer")
        res = 362;
    else if (name == "Legionary")
        res = 370;
    else if (name == "MageTower")
        res = 378;
    else if (name == "Necromancer")
        res = 386;
    else if (name == "Nexus")
        res = 394;
    else if (name == "Paladin")
        res = 402;
    else if (name == "Phantasm")
        res = 410;
    else if (name == "Pikeman")
        res = 418;
    else if (name == "Portal")
        res = 426;
    else if (name == "Prince")
        res = 434;
    else if (name == "Princess")
        res = 442;
    else if (name == "Pyromancer")
        res = 450;
    else if (name == "Ranger")
        res = 458;
    else if (name == "Rook")
        res = 466;
    else if (name == "Samurai")
        res = 474;
    else if (name == "Snake")
        res = 482;
    else if (name == "StoneMage")
        res = 490;
    else if (name == "Sylph")
        res = 498;
    else if (name == "Templar")
        res = 506;
    else if (name == "ThunderMage")
        res = 514;
    else if (name == "Toad")
        res = 522;
    else if (name == "Wisp")
        res = 530;
    else if (name == "AirElemental")
        res = 538;
    else if (name == "ArchBishop")
        res = 546;
    else if (name == "Beacon")
        res = 554;
    else if (name == "Behemoth")
        res = 562;
    else if (name == "Butterfly")
        res = 570;
    else if (name == "Chastity")
        res = 578;
    else if (name == "Demon")
        res = 586;
    else if (name == "Dragon")
        res = 594;
    else if (name == "EarthElemental")
        res = 602;
    else if (name == "Ghast")
        res = 610;
    else if (name == "Harpy")
        res = 618;
    else if (name == "HauntedArmor")
        res = 626;
    else if (name == "**HauntedArmor")
        res = 634;
    else if (name == "LifeStone")
        res = 642;
    else if (name == "Medusa")
        res = 650;
    else if (name == "Mercenary")
        res = 658;
    else if (name == "Minotaur")
        res = 666;
    else if (name == "Ninja")
        res = 674;
    else if (name == "NullMage")
        res = 682;
    else if (name == "Patience")
        res = 690;
    else if (name == "Phalanx")
        res = 698;
    else if (name == "**Phalanx")
        res = 706;
    else if (name == "Queen")
        res = 714;
    else if (name == "Reaver")
        res = 722;
    else if (name == "Salamander")
        res = 730;
    else if (name == "SoulFlare")
        res = 738;
    else if (name == "Summoner")
        res = 746;
    else if (name == "Temperance")
        res = 754;
    else if (name == "Tombstone")
        res = 762;
    else if (name == "Undine")
        res = 770;
    else if (name == "Valkyrie")
        res = 778;
    else if (name == "Wizard")
        res = 786;
    else if (name == "Angel")
        res = 794;
    else if (name == "Antimage")
        res = 802;
    else if (name == "Aquarius")
        res = 810;
    else if (name == "Comet")
        res = 818;
    else if (name == "Enchantress")
        res = 826;
    else if (name == "Envy")
        res = 834;
    else if (name == "FireElemental")
        res = 842;
    else if (name == "Fortress")
        res = 850;
    else if (name == "Gemini")
        res = 858;
    else if (name == "GeminiTwin")
        res = 866;
    else if (name == "Gluttony")
        res = 874;
    else if (name == "GravityMage")
        res = 882;
    else if (name == "Greed")
        res = 890;
    else if (name == "Illusionist")
        res = 898;
    else if (name == "Illusion")
        res = 906;
    else if (name == "Lich")
        res = 914;
    else if (name == "Lilith")
        res = 922;
    else if (name == "Lust")
        res = 930;
    else if (name == "MoonFox")
        res = 938;
    else if (name == "Phoenix")
        res = 946;
    else if (name == "PheonixEgg")
        res = 954;
    else if (name == "Pride")
        res = 962;
    else if (name == "RoyalGuard")
        res = 970;
    else if (name == "Siren")
        res = 978;
    else if (name == "SoulKeeper")
        res = 986;
    else if (name == "Taurus")
        res = 994;
    else if (name == "Vampire")
        res = 1002;
    else if (name == "VoidMage")
        res = 1010;
    else if (name == "WaterElemental")
        res = 1018;
    else if (name == "Wrath")
        res = 1026;
    else if (name == "King")
        res = 1034;
    else if (name == "Sapling")
        res = 1036;
    else if(name == "Tree")
        res = 1038;
    else if(name == "BonePile")
        res = 1040;
    else if(name == "StonePillar")
        res = 1042;
    else
        throw std::invalid_argument( name+" not recognized");

    return res+mod;
}

void loadPosition(std::string in)
{
    std::vector<std::string> res;
    int pos = 0;
    while (true)
    {
        pos = in.find(",");
        if(pos==std::string::npos)
            break;
        res.push_back(in.substr(0, pos));
        in.erase(0, pos + 1);
    }
    res.push_back(in);
    int version = res[0]=="56" ? 56 : res[0]=="57" ? 57 : 54;
    // start at the 38th slice
    int begin = version==56 ? 37 : 37;
    for(int i=37; i<37+64; i++){
        if(res[i]=="")
            continue;
        int x = (i-37)/8;
        int y = 7-(i-37)%8;
        //std::cout<<res[i]<<std::endl;
        int type = getEncoding(res[i])+(res[i+64][0]-'0');
        summonPiece(x, y, type);

        if(pieces[pc_cnt-1]==1034){
            king_id[0]=pc_cnt-1;
        }
        if(pieces[pc_cnt-1]==1035){
            king_id[1]=pc_cnt-1;
        }
    }
    undo_pnt[turn]=0;
}


std::string names[134] = {"Apprentice","Archer","Axeman","Banshee","Bat","Bishop","Bomber","Dryad","Duelist","NA","FireBall","Frog","FrostMage","Ghost","Guardian","Hoplite","Knight","Militia","Pawn","Penguin","PoisonMage","Shieldsman","Skeleton","Slime","Spearman","Spider","Swordsman","Tiger","Warrior","WindMage","Alchemist","Arachnid","Basilisk","Berserker","Crusader","NA2","Dove","Drake","Fencer","NA3","FireMage","FrostMephit","GiantSlime","Gnome","Hostage","Hydromancer","Legionary","MageTower","Necromancer","Nexus","Paladin","Phantasm","Pikeman","Portal","Prince","Princess","Pyromancer","Ranger","Rook","Samurai","Snake","StoneMage","Sylph","Templar","ThunderMage","Toad","Wisp","AirElemental","ArchBishop","Beacon","Behemoth","Butterfly","Chastity","Demon","Dragon","EarthElemental","Ghast","Harpy","HauntedArmor","NA4","LifeStone","Medusa","Mercenary","Minotaur","Ninja","NullMage","Patience","Phalanx","NA5","Queen","Reaver","Salamander","SoulFlare","Summoner","Temperance","Tombstone","Undine","Valkyrie","Wizard","Angel","Antimage","Aquarius","Comet","Enchantress","Envy","FireElemental","Fortress","Gemini","GeminiTwin","Gluttony","GravityMage","Greed","Illusionist","Illusion","Lich","Lilith","Lust","MoonFox","Phoenix","PheonixEgg","Pride","RoyalGuard","Siren","SoulKeeper","Taurus","Vampire","VoidMage","WaterElemental","Wrath","King", "Sapling", "Tree", "BonePile", "StonePillar"};
std::string getName(int piece){
    bool black = (piece&1)==1;
    if((piece&1)==1)
        piece--;
    piece-=2;
    std::string name = names[piece/8];
    int mod =(piece%8)/2;
    std::string end = (black) ? "-" : "";
    if(mod==0){
        return name+end;
    }
    return name + std::to_string(mod+1) + end;
}

void recalculate_evaluation(){
    int white_position = 0;
    int black_position = 0;
    for(int x=0; x<8; x++)
        for(int y=0; y<8; y++){
            if(board[x][y]==0)
                continue;
            if((board[x][y]&1)==0)
                white_position+=piece_square_tables[piece_type[board[x][y]/2]][x][y];
            else
                black_position+=piece_square_tables[piece_type[board[x][y]/2]][x][y];
        }
    if(position_bonus[0]!=white_position||position_bonus[1]!=black_position){
        std::cout<<position_bonus[0]<<" "<<white_position<<" "<<position_bonus[1]<<" "<<black_position<<std::endl;
        throw std::logic_error("evaluation does not match");
    }
}
long long nodes=0;

void check_state(){
    for(int i=0; i<pc_cnt; i++){
        if(death[i])
            continue;
        if(board[px[i]][py[i]]!=pieces[i]||id_board[px[i]][py[i]]!=i)
            std::cout<<"FAIL "<<i<<" "<<px[i]<<" "<<py[i]<<" "<<pieces[i]<<" "<<death[i]<<" "<<nodes<<std::endl;
    }
}

void regenerate_hash(){
    
}

int static_evaluation(int side);
void printState(){

    std::cout<<"White morale: "<<morale[0]<<" "<<"Black morale: "<<morale[1]<<std::endl;
    std::cout<<static_evaluation(turn&1)<<std::endl;
    std::cout<<"Hash: "<<hash<<std::endl;
    for(int y=7; y>=0; y--){
        for(int x=0; x<=7; x++){
            if(board[x][y]==0)
                std::cout<<"-------("<<x<<","<<y<<")------- ";
            else{
                std::cout<<getName(board[x][y])<<"("<<id_board[x][y]<<") ";//["<<pmorale[id_board[x][y]]<<"]{"<<piece_type[board[x][y]/2]<<": "<<piece_square_tables[piece_type[board[x][y]/2]][board[x][y]&1 ? y : 7-y][x]<<"} ";
                if(getName(board[x][y]).size()<16)
                    std::cout<<std::string(16-getName(board[x][y]).size()-(id_board[x][y]>=10 ? 1 : 0), ' ');
            }
        }
        std::cout<<std::endl;
    }

    for(int i=0; i<pc_cnt; i++)
        std::cout<<"("<<i<<": "<<pmorale[i]<<", "<<status[i]<<", "<<death[i]<<", "<<nulled[i]<<") ";
    std::cout<<std::endl;
    check_state();
    std::cout<<std::endl;

}

void printCandidateMoves(){
    for(int i=0; i<candidate_pointer[turn]; i++)
        std::cout<<candidateMoveStack[turn][i][0]<<" "<<candidateMoveStack[turn][i][1]<<" "<<candidateMoveStack[turn][i][2]<<" "<<candidateMoveStack[turn][i][3]<<std::endl;
    std::cout << std::endl;
}

const int agrobonus=10000;
int moveScore(int mnum){
    int xx = candidateMoveStack[turn][mnum][0];
    int yy = candidateMoveStack[turn][mnum][1];
    int pc_id = candidateMoveStack[turn][mnum][2];
    switch(candidateMoveStack[turn][mnum][3]){
        case 1: //move
            return 0;
        case 2: //attack
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100;
        case 3:
            return -1;
        case 4:
            return -1;
        case 5:
            return 2;
        case 6:
            return 3;
        case 7:
            return agrobonus + pmorale[id_board[xx][yy]]*100-300;
        case 8:
            return agrobonus + pmorale[id_board[xx][yy]]*100-300;
        case 9:
            return agrobonus + pmorale[id_board[xx][yy]]*100-100;
        case 10:
            return 1;
        case 11:
            return 4;
        case 12:
            return 1;
        case 13:
            return 2;
        case 14:
            return agrobonus + pmorale[id_board[xx][yy]]*100+100;
        case 15:
            return agrobonus + pmorale[id_board[xx][yy]]*100+150;
        case 16:
            return agrobonus + pmorale[id_board[xx][yy]]*100+200;
        case 17:
            return agrobonus + pmorale[id_board[xx][yy]]*100+300;
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
            return agrobonus + (pmorale[id_board[xx][yy]] - pmorale[pc_id])*100;
        case 27:
            return -10;
        case 28: 
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100;
        case 29:
            return agrobonus +  200*pmorale[id_board[xx][yy]];
        case 30:
            return agrobonus + (pmorale[id_board[xx][yy]] - pmorale[pc_id])*100;
        case 31:
            return -100;
        case 32:
            return agrobonus + (pmorale[id_board[xx][yy]]-pmorale[pc_id])*100;
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

long long quiesent_nodes=0;
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

int start_turn=0;
int move_chosen[4]={};
const int null_move_reduction = 2;

int evaluate(int alpha, int beta, int mdepth, int side)
{
    ++nodes;
    long long hashl=hash;
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
        makeMove(nodeMove[tail][0], nodeMove[tail][1], id_board[nodeMove[tail][2]][nodeMove[tail][3]], nodeMove[tail][4]);
        endOfTurnTriggers(side);    
        ++turn;
        if(side)
            eval = evaluate(alpha, beta, mdepth-1, side^1);
        else
            eval = evaluate(alpha, beta, mdepth-1, side^1);
        
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

        makeMove(candidateMoveStack[turn][i][0], candidateMoveStack[turn][i][1], candidateMoveStack[turn][i][2], candidateMoveStack[turn][i][3]);
        endOfTurnTriggers(side);    

        ++turn;
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
        
        --turn;
        if(DEBUG){
            std::cout<<"}"<<std::endl;
            std::cout<<"UNMAKE"<<std::endl;
        }
        unmakeMoves(turn);

        if(DEBUG)
            printState();    

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

void printChosenMove(){
    std::cout<<"Best move: "<<move_chosen[0]<<" "<<move_chosen[1]<<" "<<move_chosen[2]<<" "<<move_chosen[3]<<std::endl;
    
}

void makeChosenMove(){
    //turn++;
    makeMove(move_chosen[0], move_chosen[1], move_chosen[2], move_chosen[3]);
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
    /*if(true){
        printState();
        int xx, yy, id, mt;
        std::cin>>xx>>yy>>id>>mt;
        makeMove(xx, yy, id, mt);
        endOfTurnTriggers(side);

        std::cout<<"OPERATIONS: "<<std::endl;
        for(int i=0; i<undo_pnt[turn]; i++)
            std::cout<<undostack[turn][i][0]<<" "<<undostack[turn][i][1]<<" "<<undostack[turn][i][2]<<" "<<undostack[turn][i][3]<<" "<<undostack[turn][i][4]<<" "<<undostack[turn][i][5]<<std::endl;
        
        printState();
        unmakeMoves(turn);
        printState();
        return 1;
    }*/
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

        if(true){
            std::cout<<"OPERATIONS: "<<std::endl;
            for(int i=0; i<undo_pnt[turn]; i++)
                std::cout<<undostack[turn][i][0]<<" "<<undostack[turn][i][1]<<" "<<undostack[turn][i][2]<<" "<<undostack[turn][i][3]<<" "<<undostack[turn][i][4]<<" "<<undostack[turn][i][5]<<std::endl;
        }
        
        start_turn++;
        turn++;
        printState();
        printChosenMove();
        int xx, yy, id, mt;
        std::cin>>xx>>yy>>id>>mt;
        makeMove(xx, yy, id, mt);
        endOfTurnTriggers(1-side);
        turn++;
        start_turn++;
    }
    return 1;
}


#ifndef CONST_GLOBAL_H
#define CONST_GLOBAL_H

extern const bool DEBUG;

extern const int hash_table_size;
extern const long long hash_mask;
extern long long zblack;

extern long long zobrist[];
extern int nodeEval[]; 
extern short nodeMove[][5]; 
extern short nodeType[]; 
extern short nodeDepth[];


//(0, 0) is bottom left corner of board
extern int board[8][8]; //included in hash
extern int id_board[8][8]; //not

extern long long zboard[8][8][1200];

extern int lightning[8][8]; //yes
extern int lightning_cnt;
extern int butterfly[8][8]; //yes
extern int butterfly_cnt;
extern int meteor[8][8]; //yes
extern int meteor_cnt;

extern long long zlightning[8][8];
extern long long zmeteor[8][8];

extern int doveList[2][20];
extern int dovePnt[2];

extern int samuraiList[2][20];
extern int samuraiPnt[2];
extern int lustList[2][20];
extern int lustPnt[2];
extern int alchList[2][20];
extern int alchPnt[2];

extern int pieces[200]; //not
extern int death[200]; //not
extern long long status[200]; //yes
extern bool voided[200]; //yes
extern bool nulled[200]; //yes
extern bool transparent[200]; //no

extern long long zstatus_id[200];
extern const long long status_prime;

extern long long zvoided[200];
extern long long znulled[200];
extern long long zmorale[2][250]; //starts at -50

extern int moved[200]; //no
extern int px[200]; //no
extern int py[200]; //no
extern int pmorale[200]; //no
extern int pc_cnt;
extern int m_depth;
extern int turn;

extern int king_id[2];

extern long long hash;

//indexed by turn used -> always set +1
extern int last_moved[200];

//first value is a spacer! 
extern const bool freeze_immune[1050];
extern const bool petrify_immune[1050];
extern const bool poison_immune[1050];
extern const bool siren_immune[1050];
extern const bool displacement_immune[1050];

extern const bool isMinion[1050];
extern const bool isChampion[1050];
extern const int pieceValue[1050];
extern const int promotions[1050];


extern int depth;
extern int morale[2];
extern int position_bonus[2];

extern const int piece_type[1050];
extern const int piece_square_tables[11][8][8];

extern int candidateMoveStack[150][400][4];
extern int candidate_pointer[150];

extern int undostack[200][200][6];
extern int undo_pnt[200];

extern int start_turn;
extern int move_chosen[4];

extern long long nodes;
extern long long quiesent_nodes;
#endif

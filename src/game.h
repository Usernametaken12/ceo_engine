#ifndef GAME_H
#define GAME_H
void generateMoves(int pc_id, int x, int y);
void summonPiece(int xx, int yy, int pieceType);
void makeMove(int xx, int yy, int pc_id, int moveType);
void unmakeMoves(int tur);
void endOfTurnTriggers(int side);
#endif
#ifndef ENGINE_H
#define ENGINE_H

extern long double time_limit;

int predictPVT(int pc, int x, int y, int xx, int yy);

int static_evaluation(int side);
int evaluate(int alpha, int beta, int mdepth, int side);

void makeChosenMove();
#endif
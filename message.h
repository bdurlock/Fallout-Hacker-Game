/*
 *  message.h
 */

#include <string>

typedef struct UDP_Control_MSG_Type
{
  int port;
  
}UDP_Cntr_MSG;

typedef struct Game_MSG_Type
{
  char Display[1000];
  int AttemptsLeft;
  bool IsGameWon;
  int Likeness;
  char Entry[10];
}Game_MSG;


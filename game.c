#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize(char board[], int x, int y){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(j=0; j<x; j++){
      board[(j+x*i)]='-';//(j+4*i)+'0';
    }
  }
}

void print_board_old(char board[], int x, int y){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(j=0; j<x; j++){
      printf("%c",board[j+x*i]);
    }
    printf("\n");
  }

  printf("------------------------------\n");
}

void print_board(char board[], int x, int y){
  int i;
  int j;
  for(i=y; i>=0; i--){
    for(j=0; j<x; j++){
      printf("%c ",board[j+x*i]);
    }
    printf("\n");
  }

  printf("------------------------------\n");
}

//target is where you want to place the chip
int placer_old(char board[], int x, int y, int target){//, int player){
  int i;
  for(i=0; i<y; i++){
    if(board[target+(x*i)] == '-'){

      //printf("succ %d %d\n", i, target);


      board[target+(4*i)] = 'o';
      /*
      if(player == 1){
	board[target+(4*i)] = 'o';
      }
      
      else{//if its two
	board[target+(4*1)] = 'x';
      }
      */
      return 1;//success
    }
  }
  return -1;//only if row full
}

int placer(char board[], int x, int y, int target, int player){
  if(target >= x){
    return -1;
  }
  int i;
  for(i=0; i<y; i++){
    if(board[target+(x*i)] == '-'){

      printf("succ %d %d\n", i, target);


      //board[target+(4*i)] = player+'0';
      if(player == 1){
	board[target+(x*i)] = 'O';
      }
      if(player == 2){
	board[target+(x*i)] = 'X';
      }
      /*
      if(player == 1){
	board[target+(4*i)] = 'o';
      }
      
      else{//if its two
	board[target+(4*1)] = 'x';
      }
      */
      return 1;//success
    }
  }
  return -1;//only if row full
}

int runner(int x, int y){
  char board [(x*y)];
  
  initialize(board, x, y);
  print_board(board, x, y);


  //Testing
  /*
  //player1=o
  //player2=x
  placer(board, x, y, 2, 1);//as of now target starts at 0
  placer(board, x, y, 2, 1);//as of now target starts at 0
  placer(board, x, y, 2, 1);//as of now target starts at 0
  placer(board, x, y, 1, 2);//as of now target starts at 0
  placer(board, x, y, 1, 2);//as of now target starts at 0
  placer(board, x, y, 1, 1);//as of now target starts at 0
  placer(board, x, y, 3, 2);//as of now target starts at 0
  
  //placer(board, x, y, 5, 1);//as of now target starts at 0
  //placer(board, x, y, 1, 2);//as of now target starts at 0
  print_board(board, x, y);
  */

  /*
  char str[100];
  
  printf( "Enter a value :");
  scanf("%s", str);
  printf("%s\n", str);

  int i;
  printf( "Enter a value :");
  scanf("%d", &i);
  printf("%d\n", (i+1));
  */
  
  printf("You are Playing\n");
  while(1){
    print_board(board, x, y);
    int input;
    
    printf("Player 1 move:\n");
    printf( "Enter your move (0 for leftmost row) :");
    scanf("%d", &input);
    placer(board, x, y, input, 1);
    print_board(board, x, y);

    printf("Player 2 move:\n");
    printf( "Enter your move (0 for leftmost row) :");
    scanf("%d", &input);
    placer(board, x, y, input, 2);
    print_board(board, x, y);
  }
  
}

int main(){
  int x = 7;
  int y = 7;
  runner(x,y);
  return 0;
}

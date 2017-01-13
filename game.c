#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize(char board[], int x, int y){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(j=0; j<x; j++){
      board[(j+x*i)]='-';
    }
  }
}

/*
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
*/

void print_board(char board[], int x, int y){
  int i;
  int j;
  for(i=(y-1); i>=0; i--){
    for(j=0; j<x; j++){
      printf("%c ",board[j+x*i]);
    }
    printf("\n");
  }

  printf("------------------------------\n");
}
/*
//target is where you want to place the chip
int placer_old(char board[], int x, int y, int target){//, int player){
  int i;
  for(i=0; i<y; i++){
    if(board[target+(x*i)] == '-'){

      //printf("succ %d %d\n", i, target);


      board[target+(4*i)] = 'o';
*/
      /*
      if(player == 1){
	board[target+(4*i)] = 'o';
      }
      
      else{//if its two
	board[target+(4*1)] = 'x';
      }
      */
/*
      return 1;//success
    }
  }
  return -1;//only if row full
}
*/

int placer(char board[], int x, int y, int target, char player_icon){
  if(target >= x){
    return -1;
  }
  int i;
  for(i=0; i<y; i++){
    if(board[target+(x*i)] == '-'){

      board[target+(x*i)] = player_icon;      
      
      /*
      //board[target+(4*i)] = player+'0';
      if(player == 1){
	board[target+(x*i)] = 'O';
      }
      if(player == 2){
	board[target+(x*i)] = 'X';
      }
      */
      return 1;//success
    }
  }
  return -1;//only if row full
}


int vertical_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+(inc*y)] != symbol){
      return -1;
    }
  }
  return 1;
}


int horizontal_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc] != symbol){
      return -1;
    }
  }
  return 1;
}

//right and up
int up_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc*(y+1)] != symbol){
      return -1;
    }
  }
  return 1;
}

//right and down
int down_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol){
  int inc;
  int pos = (y*ypos) + xpos;

  int connect_number=4;//hence called connect four
  
  for(inc=0; inc<connect_number; inc++){
    if(board[pos+inc*(y-1)] != symbol){
      return -1;
    }
  }
  return 1;
}

int checker(char board[], int x, int y, char symbol){
  int i;
  int j;
  for(i=0; i<y; i++){
    for(j=0; j<x; j++){
      if(vertical_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(horizontal_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(up_slant_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
      if(down_slant_check(board, x, y, j, i, symbol) == 1){
	return 1;
      }
    }
  }
  return -1;//if true
}

int runner(int x, int y, int players){
  char board [(x*y)];

  char icons[10] = {'O','X','@','*','$','M','W'};//seven player suppoerted currentlly
  
  initialize(board, x, y);
  //print_board(board, x, y);


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


  printf("The Game Has Begun\n");
  print_board(board, x, y);
  while(1){
    //print_board(board, x, y);

    int input;
    int a;
    
    for(a=0; a<players; a++){
      printf("Player %d move:\n", a+1);
      printf("Player %d's icon is: %c\n", a+1, icons[a]);
      printf( "Enter your move (0 for leftmost row) :");
      scanf("%d", &input);
      placer(board, x, y, input, icons[a]);
      print_board(board, x, y);
      
      //CHECK IF PLAYER a+1 is WINNER
      if(checker(board, x, y, icons[a]) == 1){
	printf("PLAYER %d has won\n", a+1);
	return 1;//aborts the game
      }
    }
    
    /*
    printf("Player 1 move:\n");
    printf( "Enter your move (0 for leftmost row) :");
    scanf("%d", &input);
    placer(board, x, y, input, 'O');
    print_board(board, x, y);
    //CHECK IF PLAYER 1 is WINNER
    if(checker(board, x, y, 'O') == 1){
      printf("PLAYER 1 has won\n");
      return 1;//aborts the game
    }

    
    printf("Player 2 move:\n");
    printf( "Enter your move (0 for leftmost row) :");
    scanf("%d", &input);
    placer(board, x, y, input, 'X');
    print_board(board, x, y);

    //CHECK IF PLAYER 2 is WINNER
    if(checker(board, x, y, 'X') == 1){
      printf("PLAYER 2 has won\n");
      return 1;//aborts the game
    }
    */
  }
}

int main(){
  int x = 7;
  int y = 7;

  int num_p;
  
  printf("Enter the number of players:\n");
  scanf("%d", &num_p);
  
  
  runner(x,y, num_p);
  return 0;
}

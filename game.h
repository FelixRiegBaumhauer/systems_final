//game.c header file

void initialize(char board[], int x, int y);
void print_board(char board[], int x, int y);
int placer(char board[], int x, int y, int target, char player_icon);
int vertical_check(char board[], int x, int y, int xpos, int ypos, char symbol);
int horizontal_check(char board[], int x, int y, int xpos, int ypos, char symbol);
int down_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol);
int up_slant_check(char board[], int x, int y, int xpos, int ypos, char symbol);
int checker(char board[], int x, int y, char symbol);
int runner(int x, int y, int players);

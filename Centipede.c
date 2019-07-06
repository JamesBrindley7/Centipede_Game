#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

struct player {
	int row,col;
	char display;
	int alive;
};
struct centipede {
	int row,col;
	int prow,pcol; //p = previous
	int alive; 
	int direction; //0 is left and 1 is right
	char display;
	int head;
	int body;
	int hit;
};
struct bullets {
	int row,col;
	int active; //0 is not active and 1 is active
	char display;
};
struct mush {
	int row,col;
	int active;
	char display;
};
struct centimush {
	int row,col;
	int active;
	char display;
	int first;
};
//Structs for the different objects in the game. Decleared here so they are all grouped together and can be initislised in a loop (Display = character to be displayed)
void menu();
void restart();
void nextlevel();
int speed = 50000;

int main(){
	struct player tank;
	struct centipede centip[20];
	struct bullets shot[5];
	struct mush mushroom[50];
	struct centimush cmush[20];
	
    int i=0, j=0, k=0, l=0;
	unsigned int input, loops=0, currentshots=0, centipcount=0; 
	unsigned int win=0, score=0;
	int whole=1, headdes=0;
	char score2[20];
	int hitmush = 0;
	int finished = 0;
	
	initscr();
	clear();
	noecho();
	cbreak();
	nodelay(stdscr,1);
	
	tank.row = LINES - 1;
	tank.col = COLS / 2;
	tank.display = '^';
	tank.alive = 1;
	
	
   for (i=0; i<20; ++i) {
      centip[i].row = 1;
      centip[i].col = i*2;
      centip[i].prow = 0;
      centip[i].pcol = 0;
      centip[i].display = '#';
      centip[i].alive = 1;
      centip[i].direction = 1;
      centip[i].head = 0;
      centip[i].body = 1;
      centip[i].hit = 0;
   }
   centipcount = 20;
   centip[19].display = '@';
   centip[19].head = 1;
   centip[19].body = 0;
   
   for (i=0; i<20; ++i) {
	cmush[i].row = 0;
	cmush[i].col = 0;
	cmush[i].active = 0;
	cmush[i].display = '%';
	cmush[i].first = 1;
   }
   
   
   	for (i=0; i<5; ++i) {
      shot[i].active = 0;
      shot[i].display = '*';
	}
	//The structs are initilised and given data values using loops
		for (i=0; i<50; ++i){
		mushroom[i].display = '%';
		mushroom[i].active = 1;
		mushroom[i].col = rand() % COLS + 2;
		mushroom[i].row = rand() % (LINES-4) + 2;
	}
	
	move(0,(COLS/2)-4);
	addstr("Score: ");
	
	move(0,COLS-20);
	addstr("R = Rules  Q = Quit");
	
	move(0,1);
	addstr("P = Restart");
	
	//Adding the score at the top opf the screen which will update
	//when the user kills a piece of the centipede
	
			
	for (i=0; i<50; ++i){
		move(mushroom[i].row,mushroom[i].col);
		addstr("%");
	}
	//Displays all the mushrooms
   
	while(1) {
		
		sprintf(score2, "%d", score);
		move(0,(COLS/2)+3);
		addstr(score2);
		
		//Prints the score and updates it every loop
		
		move(tank.row,tank.col);
		addch(tank.display);
		//adds the tank character which is '^'
		
		for (i=0; i<5; ++i) {
			if (shot[i].active == 1) {
				if (shot[i].row > 0) {
					if (shot[i].row < LINES - 2) {
						move(shot[i].row + 1,shot[i].col);
						addch(' ');
					}
					for (j=0; j<20; ++j) {
						for (l=0; l<50; ++l) {
							if (mushroom[l].active == 1 && mushroom[l].row == shot[i].row && mushroom[l].col == shot[i].col){
								mushroom[l].active = 0;
								shot[i].active = 0;
								score = score + 20;
								move(mushroom[l].row,mushroom[l].col);
								addch(' ');
								break;
							}
						}
						for (l=0; l<20; ++l) {
							if (cmush[l].active == 1 && cmush[l].row == shot[i].row && cmush[l].col == shot[i].col){
								cmush[l].active = 0;
								shot[i].active = 0;
								score = score + 10;
								move(cmush[l].row,cmush[l].col);
								addch(' ');
								break;
							}
						}
						//Allows the collision between both types of mushrooms and the shots
						if ((centip[j].alive == 1 && centip[j].row == shot[i].row && centip[j].col == shot[i].col)||(centip[j].alive == 1 && centip[j].row == shot[i].row && centip[j].pcol == shot[i].col)) {
							if (j != 19 && j !=0){
								whole = 0;
								headdes = 0;
							}
							if (j == 19 && whole == 1) {
								win = 1;
								headdes = 1;	
								score = score + 5000;
								for (k=0; k<20; ++k) {
									centip[k].alive = 0;
									move(centip[k].prow,centip[k].pcol);
									addch('~');		
								}
							//Detects whether the centipede is still whole and the head has been hit
							//Since the head is destoryed the centipede dies and stops while the chara turns into ~
							}
							if (centip[j].head == 1){
								for (k=j; k>-1; --k) {
									if (centip[k-1].body == 1 && centip[k-1].alive == 1){
										centip[k-1].hit = 1;
										score = score + 300;
										centipcount = centipcount - 1;
									}
									else{
										break;
								}
							}
							}
							//Detects if the player has hit the head of one of the centipedes
							cmush[j].active = 1;
							cmush[j].row = centip[j].row;
							cmush[j].col = centip[j].col;
							move(cmush[j].row,cmush[j].col);
							addch(cmush[j].display);	
							centip[j].head = 0;
							centip[j].display = ' ';
							centip[j].alive = 0;
							shot[i].active = 0;
							score = score + 100;
							centipcount = centipcount - 1;
							move(centip[j].prow,centip[j].pcol);
							addch(' ');
							break;
						}
					}
					if (shot[i].active == 1) {
						move(shot[i].row,shot[i].col);
						addch(shot[i].display);
						--shot[i].row;
					}
				}
				else {
				shot[i].active = 0;
				move(shot[i].row + 1,shot[i].col);
				addch(' ');
				}
			}
		}    
		//The above code is for the bullets that the user shoots. This contains two loops, one for 
		//the number of bullets and the other for the number of aliens to see if there was a collision 
		
		for (i=0; i<20; ++i) {
			if (centip[i].alive == 1) {
				for (j=0; j<50; ++j) {
					if (mushroom[j].active == 1 && centip[i].row == mushroom[j].row && centip[i].col == mushroom[j].col){
						if (centip[i].direction == 0){
							centip[i].direction = 1;
							centip[i].row = centip[i].row + 1;
						}
						else if (centip[i].direction == 1){
							centip[i].direction = 0;
							centip[i].row = centip[i].row + 1;
						}
					}
				}
				for (j=0; j<20; ++j) {
					if (cmush[j].active == 1 && centip[i].prow == cmush[j].row && centip[i].pcol == cmush[j].col){
						if (cmush[j].first == 1){
							centip[i].display = '@';
							centip[i].head = 1;
							centip[i].body = 0;
							cmush[j].first = 0;
						}
						//if it hits the new mushroom first (aka the split in the centipede) then it turns that character into a head character
						if (centip[i].direction == 0){
							centip[i].direction = 1;
							centip[i].row = centip[i].row + 1;
						}
						else if (centip[i].direction == 1){
							centip[i].direction = 0;
							centip[i].row = centip[i].row + 1;
						}
					}
				}
				//Tests if the centipede hit a mushroom, either that was generated at the start or if it hits one that was 
				//created by the player destorying part of the centipede
				if (centip[i].hit == 1){
					centip[i].display = ' ';
					centip[i].alive = 0;
					centip[i].hit == 0;					
				}
				//if the centipede is hit in the head all of the other peices that are part of the same centipede die aswell but only 1 mushroom is created
				move(centip[i].prow,centip[i].pcol);
				addch(' ');
				
				move(centip[i].row,centip[i].col);
				addch(centip[i].display);
				
				centip[i].prow = centip[i].row;
				centip[i].pcol = centip[i].col;
            
				if (centip[i].direction == 0){
					--centip[i].col;
				}
				else if (centip[i].direction == 1) {
					++centip[i].col;
				}
				if (centip[i].col == COLS-1) {
					++centip[i].row;
					centip[i].direction = 0;
				}
				else if (centip[i].col == 0) {
					++centip[i].row;
					centip[i].direction = 1;
				}
				if (centip[i].row == tank.row && centip[i].col == tank.col){
					for (k=0; k<20; ++k) {
						centip[k].alive = 0;
					}
					move((LINES/2),(COLS/2)-6);
					addstr("YOU LOSE!!!");
					move(((LINES/2)+1),(COLS/2)-19);
					addstr("Press Q to quit or P to restart!");
					win = 2;
					finished = 1;
					move(tank.row,tank.col);
					addch('>');
					tank.display = '>';
					tank.alive = 0;
				}
			}
		} 
		if (centipcount == 0 || win == 1){
			if (headdes == 1) {
				move((LINES/2),(COLS/2)-19);
				addstr("You destroyed the head, YOU WIN!!!!!");
				move(((LINES/2)+1),(COLS/2)-30);
				addstr("Press Q to quit or P to restart the level or Z to progress to the next level!");
				finished = 1;
			}
			else {
				move((LINES/2),(COLS/2)-6);
				addstr("YOU WIN!!!!");
				win = 1;
				move(((LINES/2)+1),(COLS/2)-28);
				addstr("Press Q to quit or P to restart or Z to progress to the next level!");
				finished = 1;
			}
		}
		//This loop is the loop that moves the entipedes around. It does this by replacing the existing 
		//character with a space and then placing the display character which is the '#' infront of it
		
      	move(0,COLS-1); //Puts the curser up at the top right so it isnt at the front of the centipede while moving
		refresh();
		usleep(speed);
		//This is the delay so that the centipede moves at a speed at which the player can hit it otherwise
		//all this code would execute in a matter of seconds and the game would instantly be over
      
		input = getch();
		
		move(tank.row,tank.col);
		addch(' ');
      
		if (input == 'a' && tank.alive == 1){
			--tank.col;
		}
		else if (input == 'd' && tank.alive == 1){
			++tank.col;
		}
		if (input == 'q'){
			clear();
			refresh();
			exit(0);
		}
		if (input == 'z'){
			nextlevel();
		}
		if (input == 'p'){
			restart();
		}
		if (tank.col > COLS){
			tank.col = COLS;
		}
		else if (tank.col < 0){
			tank.col = 0;    
		}
		else if (input == ' ' && tank.alive == 1) {
			for (i=0; i<5; ++i) {
				if (shot[i].active == 0) {
				shot[i].active = 1;
				shot[i].row = LINES - 2;
				shot[i].col = tank.col;
				break;
				}
			}
		}
		if (input == 'r'){
			menu();
		}
		//The above code is for the key presses, I used getch to detect inputs on the keyboard
		//If the user presss a then it deducts 1 from the tank column meaning it moves one column to the left
		//This is the same for the d key but in oppoiste.
		//The space bar is used to shoot a shot, inside this it has a if statement to limit the amount of shots
		//a user can fire to only 5
   }	
}
void menu(){
	
	char input2;
	
	clear();
	echo();
	nodelay(stdscr,0);
	
	move(0,COLS/2);
	addstr("Rules");
	move(0,COLS-10);
	addstr("R = Return");
	move(1,0);
	addstr("Destroying a mushroom left by a centipede death = 10 points");
	move(2,0);
	addstr("Destroying a mushroom = 20 points");
	move(3,0);
	addstr("Destroying a centipede = 100 points");
	move(4,0);
	addstr("Destroying a head centipede = 300 points for each body");
	move(5,0);
	addstr("Destroying the head centipede one the first shot = 5000 points");
	
	input2 = getch();
	
	if (input2 == 'r'){
			clear();
			echo();
			nodelay(stdscr,1);
			main();
		}
}
void restart(){
	speed = 50000;
	clear();
	echo();
	nodelay(stdscr,1);
	main();
}
void nextlevel () {
	speed =  speed - 10000;
	clear();
	echo();
	nodelay(stdscr,1);
	main();
}


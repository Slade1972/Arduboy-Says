/*
 Name:		ArduboySays.ino
 Created:	2/1/2017 2:00:34 PM
 Author:	Slade
 
 Arduboy screen size = 128 * 64

*/

#include <Arduboy.h>

const int gameLen = 20;	// 20 rounds of the game.
int currRound = 1;		// the current round being played - two movements to begin.
int totalMoves = 2;		// total moves per round.
int currSeq[22];		// array to hold the 22 (2 for start + 20) movements.
int lives = 3;			// total lives for the player
int score = 0;			// Game score. 1 Point per correct guess.
int upBuff = 0;			// button bounce for up, down, left, right buttons.
int downBuff = 0;
int leftBuff = 0;
int rightBuff = 0;
bool roundLost;			// set a flag to determine if the round is lost due to a wrong move.


Arduboy ab;

PROGMEM const unsigned char up_Arrow [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff,
	0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

PROGMEM const unsigned char down_Arrow [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff,
	0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

PROGMEM const unsigned char left_Arrow []{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

PROGMEM const unsigned char right_Arrow []{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

PROGMEM const unsigned char sad_Face[]{
	0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x04, 0x06, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x03, 0x02, 0x02, 0x02, 0x06, 0x04, 0x0c, 0x38, 0x70, 0xc0, 0x80, 0x00, 0x00,
	0xf0, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0xf8,
	0xf8, 0x00, 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1e, 0xf0,
	0x0f, 0x78, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x83,
	0x83, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x78, 0x0f,
	0x00, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x20, 0x66, 0x45, 0x44, 0xc4, 0x84, 0x84, 0x84,
	0x84, 0x84, 0x84, 0xc4, 0x44, 0x45, 0x66, 0x20, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 0x00, 0x00,
};

PROGMEM const unsigned char happy_Face[]{
	0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x04, 0x06, 0x02, 0x02, 0x03, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x03, 0x02, 0x02, 0x02, 0x06, 0x04, 0x0c, 0x38, 0x70, 0xc0, 0x80, 0x00, 0x00,
	0xf0, 0x1e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0xf8,
	0xf8, 0x00, 0x00, 0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1e, 0xf0,
	0x0f, 0x78, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x83,
	0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x78, 0x0f,
	0x00, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x20, 0x60, 0x41, 0x42, 0xc4, 0x84, 0x84, 0x84,
	0x84, 0x84, 0x84, 0xc4, 0x42, 0x41, 0x60, 0x20, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 0x00, 0x00,
};


// the setup function runs once when you press reset or power the board
void setup() {
	ab.begin();				//initialise the system
	txtinit();				//load txt / start screen.
	randomSeed(7/8);		//random seed...?
}

void txtinit() {
	ab.clear();
	ab.setTextSize(2);
	ab.setCursor(20,0);
	ab.println("Arduboy");
	ab.setCursor(37, 15);
	ab.println("Says");
	ab.setTextSize(1);
	ab.print("\n");
	ab.println("Shall We Play A Game");
	ab.println("Press A & B to Begin");
	ab.print("(c) Rodney Norton");
	ab.display();
}


// the loop function runs over and over again until power down or reset
void loop() {
	//if (!(ab.nextFrame())) return;	//if not the next frame, skip everything

	if (ab.pressed(A_BUTTON + B_BUTTON)) {

		//set & reset variables
		currRound = 1;
		totalMoves = 2;
		score = 0;
		lives = 3;
		currSeq[22];


		ab.clear();				// clear display
		ab.setCursor(30, 25);
		ab.print("Round: ");
		ab.print(currRound);
		ab.display();			// show display
		delay(1000);
		ab.clear();
		ab.display();
		playGame();				// run game routine
	}
}


void playGame() {

	while (currRound <= gameLen)
	{
		// generate 'random' sequence for current round
		// the random number represents the U/D/L/R sequence & image.
		for (int i = 0; i <= currRound; i++) {
			currSeq[i] = random(4) % 100;

			if (currSeq[i] == 0) {
				ab.drawBitmap(15, 0, up_Arrow, 32, 32, WHITE);	// display the image
				dispLives();
				delay(1000);
				ab.drawBitmap(15, 0, up_Arrow, 32, 32, BLACK);	// turn it off
				dispLives();
				delay(500);
			}
			else if (currSeq[i] == 1)
			{
				ab.drawBitmap(39, 15, right_Arrow, 32, 32, WHITE);
				dispLives();
				delay(1000);
				ab.drawBitmap(39, 15, right_Arrow, 32, 32, BLACK);
				dispLives();
				delay(500);
			}
			else if (currSeq[i] == 2)
			{
				ab.drawBitmap(15, 32, down_Arrow, 32, 32, WHITE);
				dispLives();
				delay(1000);
				ab.drawBitmap(15, 32, down_Arrow, 32, 32, BLACK);
				dispLives();
				delay(500);
			}
			else if (currSeq[i] == 3)
			{
				ab.drawBitmap(0, 15, left_Arrow, 32, 32, WHITE);
				dispLives();
				delay(1000);
				ab.drawBitmap(0, 15, left_Arrow, 32, 32, BLACK);
				dispLives();
				delay(500);
			}
		}

		// display get ready
		for (int j = 3; j > 0; j--) {
			ab.clear();
			ab.setCursor(25, 25);
			ab.print("Get Ready!");
			ab.setCursor(50, 35);
			ab.print(j);
			dispLives();
			delay(500);
		}

		ab.clear();
		ab.setTextSize(2);
		ab.setCursor(40, 25);
		ab.print("GO !");
		ab.display();
		delay(500);
		ab.clear();
		ab.setTextSize(1);
		dispLives();

		// check to see if the button presses match the previous sequence.
		btnInput();
		currRound++;
		totalMoves++;

		if (lives <= 0) {
			gameOver();
		}
	}

	gameOver();
}

void gameOver() {
	if (lives <= 0) {
		ab.clear();
		ab.setCursor(0, 0);
		ab.println("Game Over");
		ab.println("You Lose");
		ab.print("Press A + B to Begin");
		ab.display();
		delay(3000);
	}
	else
	{
		ab.clear();
		ab.setCursor(0, 0);
		ab.println("Well Done");
		ab.println("You beat the game");
		ab.println("Press A & B to begin");
		ab.display();
	}

	while (!(ab.pressed(A_BUTTON + B_BUTTON))) {
		//do nothing until A + B are pressed
		delay(100);
	}
	loop();
}

void dispLives() {
	// display lives
	ab.setCursor(75, 0);
	ab.print("Round: ");
	ab.print(currRound);
	ab.setCursor(75, 9);
	ab.print("Lives: ");
	ab.print(lives);
	ab.setCursor(75, 55);
	ab.print("Score: ");
	ab.print(score);
	ab.display();
}

void btnInput() {
	// allow loop through 
	int i = 0;
	roundLost = false;	// ensure the round lost flag is false at each initial run through.

	// currRound + 1 for each input (round 1 has 2 inputs, round 2 has 3 inputs, etc).
	do {
		ab.clear();


		if (ab.pressed(UP_BUTTON) == true and upBuff == 0) {
			upBuff = 1;
			ab.drawBitmap(15, 0, up_Arrow, 32, 32, WHITE);
			dispLives();
			delay(500);
			ab.drawBitmap(15, 0, up_Arrow, 32, 32, BLACK);
			if (currSeq[i] == 0) {
				//correct guess continue...
				i++;
				score++;
			}
			else
			{
				// lose a life.... Need to break out of here and back to the round.
				lives--;
				roundLost = true;
				break;
			}
			dispLives();
		}

		if (ab.pressed(DOWN_BUTTON) == true and downBuff == 0) {
			downBuff = 1;
			ab.drawBitmap(15, 32, down_Arrow, 32, 32, WHITE);
			dispLives();
			delay(500);
			ab.drawBitmap(15, 32, down_Arrow, 32, 32, BLACK);
			if (currSeq[i] == 2) {
				//correct guess continue...
				i++;
				score++;
			}
			else
			{
				// lose a life.... Need to break out of here and back to the round.
				lives--;
				roundLost = true;
				break;
			}
			dispLives();
		}

		if (ab.pressed(LEFT_BUTTON) == true and leftBuff == 0) {
			leftBuff = 1;
			ab.drawBitmap(0, 15, left_Arrow, 32, 32, WHITE);
			dispLives();
			delay(500);
			ab.drawBitmap(0, 15, left_Arrow, 32, 32, BLACK);
			if (currSeq[i] == 3) {
				//correct guess continue...
				i++;
				score++;
			}
			else
			{
				// lose a life.... Need to break out of here and back to the round.
				lives--;
				roundLost = true;
				break;
			}
			dispLives();
		}

		if (ab.pressed(RIGHT_BUTTON) == true and rightBuff == 0) {
			rightBuff = 1;
			ab.drawBitmap(39, 15, right_Arrow, 32, 32, WHITE);
			dispLives();
			delay(500);
			ab.drawBitmap(39, 15, right_Arrow, 32, 32, BLACK);
			if (currSeq[i] == 1) {
				//correct guess continue...
				i++;
				score++;
			}
			else
			{
				// lose a life.... Need to break out of here and back to the round.
				lives--;
				roundLost = true;
				break;
			}
			dispLives();
		}


		// debounce buttons
		if (ab.notPressed(UP_BUTTON)) {
			upBuff = 0;
		}
		if (ab.notPressed(DOWN_BUTTON)) {
			downBuff = 0;
		}
		if (ab.notPressed(LEFT_BUTTON)) {
			leftBuff = 0;
		}
		if (ab.notPressed(RIGHT_BUTTON)) {
			rightBuff = 0;
		}

	} while (i < totalMoves);


	if (roundLost) {		// display the sad face, exit the sub, deduct one from the round & total moves, they're going to be added again shortly.
							// this ensures a repeated round at this level.
		ab.drawBitmap(15, 16, sad_Face, 32, 32, WHITE);
		dispLives();
		delay(1000);
		ab.drawBitmap(15, 16, sad_Face, 32, 32, BLACK);
		dispLives();
		delay(500);
		currRound--;
		totalMoves--;
	}
	else
	{
		ab.drawBitmap(15, 16, happy_Face, 32, 32, WHITE);
		dispLives();
		delay(1000);
		ab.drawBitmap(15, 16, happy_Face, 32, 32, BLACK);
		dispLives();
		delay(500);

	}
}
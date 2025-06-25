
#include <Arduboy2.h>
#include "images.h"

Arduboy2 arduboy;
char strBuf[120];

enum GameState {Splash, Title, BuildUp, Smash, Hold, Award, Victory} gameState = GameState::Splash;

int buildUpFrames = 120;
int holdFrames = 60 * 3;
int responceFrames = 0;
int frameCounter = 0;

struct GameMatch {
	uint8_t* enemyImage;
	int buildUpMin;
	int buildUpMax;
	int responceTime;
};
const int TOTAL_MATCHES = 5;
const GameMatch gameMatch[TOTAL_MATCHES] {
	{pumpkinSamurai, 60, 600, 64},
	{broccoliSamurai, 60, 600, 32},
	{bellpepperSamurai, 60, 800, 24},
	{cornSamurai, 45, 1000, 16},
	{carrotSamurai, 30, 1200, 12}
};
int gameMatchIndex = 0;


void setup() {
	arduboy.begin();
	arduboy.clear();
  
	arduboy.setFrameRate(60);
}

void loop() {
	if (!arduboy.nextFrame()) return;

	arduboy.clear();
	arduboy.pollButtons();

	// sprintf(strBuf, "%d, %d, %d", gameMatchIndex, (int)gameState, frameCounter);
	// Serial.println(strBuf);

	switch (gameState) {
		case GameState::Splash:
			SplashScreen();
			break;
		case GameState::Title:
			TitleScreen();
			break;
		case GameState::BuildUp:
			BuildUpScreen();
			break;
		case GameState::Smash:
			SmashScreen();
			break;
		case GameState::Hold:
			HoldScreen();
			break;
		case GameState::Award:
			AwardScreen();
			break;
		case GameState::Victory:
			VictoryScreen();
			break;
	}

	arduboy.display();
}

void SplashScreen() {
	Sprites::drawOverwrite(0, 0, splashnana, 0);
	Sprites::drawOverwrite(74, 31, splashKanji, 0);
	Sprites::drawOverwrite(51, 3, title, 0);

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {

		gameState = GameState::Title;
	}
}

void TitleScreen() {
	Sprites::drawOverwrite(0, 0, startnana, 0);
	Sprites::drawOverwrite(51, 3, title, 0);
	arduboy.setCursor(71, 39);
	arduboy.print("Play");

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
		gameMatchIndex = 0;
		buildUpFrames = random(gameMatch[gameMatchIndex].buildUpMin, gameMatch[gameMatchIndex].buildUpMax);
		frameCounter = 0;

		gameState = GameState::BuildUp;
	}
}

void BuildUpScreen() {
	Sprites::drawSelfMasked(78, 6, gameMatch[gameMatchIndex].enemyImage, 0);
	Sprites::drawSelfMasked(0, 6, bananaSamurai, 0);

	frameCounter++;

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
		responceFrames = 6000;
		frameCounter = 0;

		gameState = GameState::Hold;
	}

	if (frameCounter >= buildUpFrames) {
		frameCounter = 0;

		gameState = GameState::Smash;
	}
}

void SmashScreen() {
	Sprites::drawSelfMasked(32, 0, go, 0);

	frameCounter++;

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
		responceFrames = frameCounter;
		frameCounter = 0;

		gameState = GameState::Hold;
	}
}

void HoldScreen() {
	float delta = 1.0f - (float)frameCounter/(float)holdFrames;
	int xoff = delta*delta * 16.0f;
	int yoff = delta*delta * 8.0f;
	Sprites::drawSelfMasked(8 + xoff, yoff, bananaSamurai, 0);
	Sprites::drawSelfMasked(72 - xoff, yoff, gameMatch[gameMatchIndex].enemyImage, 0);

	frameCounter++;

	if (frameCounter >= holdFrames) {
		gameState = GameState::Award;
	}
}

void AwardScreen() {
	bool win = responceFrames <= gameMatch[gameMatchIndex].responceTime;

	if (win) {
		arduboy.setCursor(10, 5);
		arduboy.print("Win");

		Sprites::drawSelfMasked(32, 0, bananaSamurai, 0);
	} else {
		arduboy.setCursor(10, 5);
		arduboy.print("Loose");

		Sprites::drawSelfMasked(32, 0, gameMatch[gameMatchIndex].enemyImage, 0);
	}

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
		frameCounter = 0;
		gameMatchIndex++;

		if (gameMatchIndex < TOTAL_MATCHES) {
			if (win) {
				gameState = GameState::BuildUp;
			} else {
				gameState = GameState::Title;
			}
		} else {
			if (win) {
				gameState = GameState::Victory;
			} else {
				gameState = GameState::Title;
			}
		}
	}
}

void VictoryScreen() {
	Sprites::drawOverwrite(74, 0, victorynana, 0);
	Sprites::drawOverwrite(3, 11, victoryTitle, 0);
	Sprites::drawOverwrite(31, 027, victoryKanji 	, 0);

	if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
		gameState = GameState::Title;
	}
}





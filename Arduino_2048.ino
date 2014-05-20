/*
  Ardunio_2048.ino - Game 2048 for Arduino Uno/Nano
  Copyright (c) 2014-2016 Daniel Lorenz <info@tinkering-arduino.de>

  This programm is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This programm is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <TFT.h>
#include <SPI.h>

#include "Game_2048.h" 

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

#define BUTTON_START 7
#define BUTTON_U 6
#define BUTTON_D 5
#define BUTTON_L 3
#define BUTTON_R 2

TFT TFTscreen = TFT(cs, dc, rst);

String tileValueString;
char tileValueCharArray[5];

Game_2048 game_2048;

void setup() {
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_U, INPUT);
  pinMode(BUTTON_D, INPUT);
  pinMode(BUTTON_L, INPUT);
  pinMode(BUTTON_R, INPUT);
  
  randomSeed(analogRead(0));

  TFTscreen.begin();
  
  TFTscreen.setRotation(2);

  TFTscreen.background(255, 255, 255);
  
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.fill(0, 0, 0);
  
  drawPoints();
  drawMatrix();
 
  //getAvailableCells(); 
}

void loop() {
  
  if ( digitalRead(BUTTON_START) == HIGH ) {
    pressedButtonStart();
  } else if ( digitalRead(BUTTON_U) == HIGH && game_2048.isGameStarted() ) {
    pressedButtonU();
  } else if ( digitalRead(BUTTON_D) == HIGH && game_2048.isGameStarted() ) {
    pressedButtonD();
  } else if ( digitalRead(BUTTON_L) == HIGH && game_2048.isGameStarted() ) {
    pressedButtonL();
  } else if ( digitalRead(BUTTON_R) == HIGH && game_2048.isGameStarted() ) {
    pressedButtonR();
  }
  
  if ( game_2048.needsOutputRefresh() ) {
    drawMatrix();
  }
}

void pressedButtonL() {
  delay(10);

  if (digitalRead(BUTTON_L) == HIGH) {
    
    erasePoints();
    game_2048.moveCellsL();
    drawPoints();
    
    while (digitalRead(BUTTON_L) == HIGH) {
      delay(10);
    }
    
    delay(50);
  }
}

void pressedButtonR() {
  delay(10);

  if (digitalRead(BUTTON_R) == HIGH) {
    
    erasePoints();
    game_2048.moveCellsR();
    drawPoints();
    
    while (digitalRead(BUTTON_R) == HIGH) {
      delay(10);
    }
    
    delay(50);
  }
}

void pressedButtonU() {
  delay(10);

  if (digitalRead(BUTTON_U) == HIGH) {
    
    erasePoints();
    game_2048.moveCellsU();
    drawPoints();
    
    while (digitalRead(BUTTON_U) == HIGH) {
      delay(10);
    }
    
    delay(50);
  }
}

void pressedButtonD() {
  delay(10);

  if (digitalRead(BUTTON_D) == HIGH) {
    
    erasePoints();
    game_2048.moveCellsD();
    drawPoints();
    
    while (digitalRead(BUTTON_D) == HIGH) {
      delay(10);
    }
    
    delay(50);
  }
}

void pressedButtonStart() {
  delay(10);

  if (digitalRead(BUTTON_START) == HIGH) {
    game_2048.startGame();
    
    while (digitalRead(BUTTON_START) == HIGH) {
      delay(10);
    }
  }
}

void drawMatrix() {
  int xOffset;
  int posX;
  int posY;
  unsigned int actCell = 0;
  
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      posX = col * 31 + 2;
      posY = row * 31 + 30;
      actCell = game_2048.getCell( row, col);
      switch ( actCell ) {
        case 2:
          xOffset = 12;
          TFTscreen.fill(0xEE, 0xE4, 0xDA);
          break;
        case 4:
          xOffset = 12;
          TFTscreen.fill(0xED, 0xE0, 0xc8);
          break;
        case 8:
          xOffset = 12;
          TFTscreen.fill(0xF9, 0xF6, 0xF2);
          break;
        case 16:
          xOffset = 8;
          TFTscreen.fill(0xF5, 0x95, 0x63);
          break;
        case 32:
          xOffset = 9;
          TFTscreen.fill(0xF6, 0x7C, 0x5F);
          break;
        case 64:
          xOffset = 9;
          TFTscreen.fill(0xF6, 0x5E, 0x3B);
          break;
        case 128:
          xOffset = 6;
          TFTscreen.fill(0xED, 0xCF, 0x72);
          break;
        case 256:
          xOffset = 6;
          TFTscreen.fill(0xED, 0xCC, 0x61);
          break;
        case 512:
          xOffset = 6;
          TFTscreen.fill(0xED, 0xC8, 0x50);
          break;
        case 1024:
          xOffset = 3;
          TFTscreen.fill(0xED, 0xC5, 0x3F);
          break;
        case 2048:
          xOffset = 3;
          TFTscreen.fill(0xED, 0xC2, 0x2E);
          break;
        default:
          TFTscreen.fill(0xBB, 0xAD, 0xA0);
      }
      TFTscreen.rect( posX, posY, 29, 29);
      if ( !game_2048.isGameLost() && !game_2048.isGameWon() && ( actCell > 0 ) ) {
        tileValueString = String( actCell );
        tileValueString.toCharArray(tileValueCharArray, 5);
        TFTscreen.text( tileValueCharArray , posX + xOffset, posY + 10 ); 
      }
    }
  }
  
  if ( game_2048.isGameLost() ) {
    drawGameOver();
  }
  
  if ( game_2048.isGameWon() ) {
    drawGameWon();
  }
  
  game_2048.outputRefreshed();
}

void drawPoints() {
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Points: ", 2, 2);
  
  tileValueString = String( game_2048.getPoints() );
  tileValueString.toCharArray(tileValueCharArray, 7);
  TFTscreen.text( tileValueCharArray, 52, 2); 
}

void erasePoints() {
  TFTscreen.stroke(255,255,255);
  
  tileValueString = String( game_2048.getPoints() );
  tileValueString.toCharArray(tileValueCharArray, 7);
  TFTscreen.text( tileValueCharArray, 52, 2); 
  
  TFTscreen.stroke(0,0,0);
}

void drawGameOver() {
  char gameOverStr[10] = "Game Over";
  char gameOverChar[2] = " ";
  
  int posX;
  
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * 31 + 2 + 12;
    
    tileValueString = String( gameOverStr[col] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    TFTscreen.stroke( 0, 0, 0 );
    TFTscreen.text( tileValueCharArray, posX, 1 * 31 + 30 + 10 ); 
  }
    
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * 31 + 2 + 12;
    
    tileValueString = String( gameOverStr[col + 5 ] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    TFTscreen.stroke( 0, 0, 0 );
    TFTscreen.text( tileValueCharArray, posX, 2 * 31 + 30 + 10 ); 
  }
}

void drawGameWon() {
  char gameOverStr[10] = "You   Win";
  char gameOverChar[2] = " ";
  
  int posX;
  
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * 31 + 2 + 12;
    
    tileValueString = String( gameOverStr[col] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    TFTscreen.stroke( 0, 0, 0 );
    TFTscreen.text( tileValueCharArray, posX, 1 * 31 + 30 + 10 ); 
  }
    
  for (unsigned int col = 0; col < 4; col++) {
    posX = col * 31 + 2 + 12;
    
    tileValueString = String( gameOverStr[col + 5 ] );
    tileValueString.toCharArray(tileValueCharArray, 5);
      
    TFTscreen.stroke( 0, 0, 0 );
    TFTscreen.text( tileValueCharArray, posX, 2 * 31 + 30 + 10 ); 
  }
}

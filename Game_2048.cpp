/*
  Game_2048.cpp - Game 2048 Library for Arduino
  Copyright (c) 2014-2016 Daniel Lorenz <info@tinkering-arduino.de>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Game_2048.h"

unsigned int cells[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
bool availableCells[16];
int countAvailableCells;

unsigned long points = 0;

bool gameChanged = false;

bool gameStarted = false;
bool gameLost = false;
bool gameWon = false;

bool Game_2048::isGameStarted() {
  return gameStarted;
}

bool Game_2048::isGameLost() {
  return gameLost;
}

bool Game_2048::isGameWon() {
  return gameWon;
}

bool Game_2048::needsOutputRefresh() {
  return gameChanged;
}

void Game_2048::outputRefreshed() {
  gameChanged = false;
}

unsigned long Game_2048::getPoints() {
  return points;
}
    
unsigned int Game_2048::getCell( unsigned int row, unsigned int col) {
  return cells[row *4 + col];
}

void Game_2048::moveCellsL() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ 4 * row + 0 ];
    loc.cellInLine[1] = cells[ 4 * row + 1 ];
    loc.cellInLine[2] = cells[ 4 * row + 2 ];
    loc.cellInLine[3] = cells[ 4 * row + 3 ];
    loc = moveCells( loc );
    cells[ 4 * row + 0 ] = loc.cellInLine[0];
    cells[ 4 * row + 1 ] = loc.cellInLine[1];
    cells[ 4 * row + 2 ] = loc.cellInLine[2];
    cells[ 4 * row + 3 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved && !isGameWon() && !isGameLost() ) {
    addRandomTile();
  }
}

void Game_2048::moveCellsR() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ 4 * row + 3 ];
    loc.cellInLine[1] = cells[ 4 * row + 2 ];
    loc.cellInLine[2] = cells[ 4 * row + 1 ];
    loc.cellInLine[3] = cells[ 4 * row + 0 ];
    loc = moveCells( loc );
    cells[ 4 * row + 3 ] = loc.cellInLine[0];
    cells[ 4 * row + 2 ] = loc.cellInLine[1];
    cells[ 4 * row + 1 ] = loc.cellInLine[2];
    cells[ 4 * row + 0 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved && !isGameWon() && !isGameLost() ) {
    addRandomTile();
  }
}

void Game_2048::moveCellsU() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ row + 0 ];
    loc.cellInLine[1] = cells[ row + 4 ];
    loc.cellInLine[2] = cells[ row + 8 ];
    loc.cellInLine[3] = cells[ row + 12 ];
    loc = moveCells( loc );
    cells[ row + 0 ] = loc.cellInLine[0];
    cells[ row + 4 ] = loc.cellInLine[1];
    cells[ row + 8 ] = loc.cellInLine[2];
    cells[ row + 12 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved && !isGameWon() && !isGameLost() ) {
    addRandomTile();
  }
}

void Game_2048::moveCellsD() {
  LineOfCells loc;
  loc.cellMoved = false;
  
  for (unsigned int row = 0; row < 4; row++) {
    loc.cellInLine[0] = cells[ row + 12 ];
    loc.cellInLine[1] = cells[ row + 8 ];
    loc.cellInLine[2] = cells[ row + 4 ];
    loc.cellInLine[3] = cells[ row + 0 ];
    loc = moveCells( loc );
    cells[ row + 12 ] = loc.cellInLine[0];
    cells[ row + 8 ] = loc.cellInLine[1];
    cells[ row + 4 ] = loc.cellInLine[2];
    cells[ row + 0 ] = loc.cellInLine[3];
  }
  
  if ( loc.cellMoved && !isGameWon() && !isGameLost() ) {
    addRandomTile();
  }
}

LineOfCells Game_2048::moveCells( LineOfCells loc ) {
  for (unsigned int arrayPos = 0; arrayPos < 4; arrayPos++) {
    if ( loc.cellInLine[arrayPos] == 0 ) {
      for (unsigned int arrayPosNext = arrayPos; arrayPosNext < 4; arrayPosNext++) {
        if ( loc.cellInLine[arrayPosNext] != 0 ) {
          
          loc.cellMoved = true;
          gameChanged = true;
          
          loc.cellInLine[arrayPos] = loc.cellInLine[arrayPosNext];
          loc.cellInLine[arrayPosNext] = 0;
          
          break;
        }
      }
    }
    if ( loc.cellInLine[arrayPos] > 0 ) {
      for (unsigned int arrayPosNext = arrayPos+1; arrayPosNext < 4; arrayPosNext++) {
        if ( loc.cellInLine[arrayPos] == loc.cellInLine[arrayPosNext] ) {
          
          points += loc.cellInLine[arrayPos] + loc.cellInLine[arrayPosNext];
          
          loc.cellMoved = true;
          gameChanged = true;
          
          loc.cellInLine[arrayPos] = loc.cellInLine[arrayPos] + loc.cellInLine[arrayPosNext];
          loc.cellInLine[arrayPosNext] = 0;
          
          if ( loc.cellInLine[arrayPos] == 2048 ) {
            winGame();
          }
          
          break;
        } else if ( loc.cellInLine[arrayPosNext] > 0 ) {
          break;
        }
      }
    }
  }
  
  return(loc);
}

bool furtherMovesArePossible() {
  int arrayPos;
  
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      arrayPos = row * 4 + col;
      
      // return true if any cell is available to set a new tile
      if ( cells[arrayPos] == 0) {
        return true;
      }
      
      // return true if any right neighbor cell has the same value
      if ( ( col < 3 ) && ( cells[arrayPos] == cells[arrayPos + 1] ) ) {
        return true;
      }
      
      // return true if any lower neighbor cell has the same value
      if ( ( row < 3 ) && ( cells[arrayPos] == cells[arrayPos + 4] ) ) {
        return true;
      }
    }
  }
  return false;
}

// Adds a tile in a random position
void Game_2048::addRandomTile() {
  getAvailableCells();
  
  if ( countAvailableCells > 0 ) {
    unsigned int randomTile = random(10) < 9 ? 2 : 4;
    unsigned int randomPosition = random(countAvailableCells);
    
    insertRandomTileToAvailableCell( randomTile, randomPosition );
  
    getAvailableCells();
  } else {
    loseGame();
  }
}

void Game_2048::insertRandomTileToAvailableCell( unsigned int randomTile, unsigned int randomPosition ) {
  countAvailableCells = 0;
  
  for (unsigned int arrayPos = 0; arrayPos < 16; arrayPos++) {
    if ( availableCells[arrayPos] == true ) {
      if ( randomPosition == countAvailableCells) {
        gameChanged = true;
        cells[arrayPos] = randomTile;
        break;
      }
      
      countAvailableCells += 1;
    }
  }
}

void Game_2048::getAvailableCells() {
  int arrayPos;
  
  countAvailableCells = 0;
  
  for (unsigned int row = 0; row < 4; row++) {
    for (unsigned int col = 0; col < 4; col++) {
      arrayPos = row * 4 + col;
      if ( cells[arrayPos] == 0 ) {
        availableCells[arrayPos] = true;
        countAvailableCells += 1;
      } else {
        availableCells[arrayPos] = false;
      }
    }
  }
};

void Game_2048::startGame() {
  for (unsigned int arrayPos = 0; arrayPos < 16; arrayPos++) {
    cells[arrayPos] = 0;
    availableCells[arrayPos] = true;
  }
  
  addRandomTile();
  addRandomTile();
  
  gameStarted = true;
  gameLost = false;
  gameWon = false;
  
  points = 0;
}

void Game_2048::loseGame() {
  gameStarted = false;
  gameLost = true;
  gameWon = false;
}

void Game_2048::winGame() {
  gameStarted = false;
  gameLost = false;
  gameWon = true;
}

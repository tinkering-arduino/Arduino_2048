/*
  Game_2048.h - Game 2048 Library for Arduino
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

#ifndef Game_2048_h
#define Game_2048_h

typedef struct LineOfCells {
  bool cellMoved;
  unsigned int cellInLine[4];
} LOC;

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class Game_2048 {
  public:
    
    void startGame();
    
    unsigned long getPoints();
    
    unsigned int getCell( unsigned int row, unsigned int col);
    
    void moveCellsU();
    void moveCellsD();
    void moveCellsL();
    void moveCellsR();
    
    bool isGameStarted();
    bool isGameLost();
    bool isGameWon();
    
    bool needsOutputRefresh();
    void outputRefreshed();
  
  private:
    LineOfCells moveCells( LineOfCells loc );
    
    bool furtherMovesArePossible();
    
    void addRandomTile();
    
    void insertRandomTileToAvailableCell( unsigned int randomTile, unsigned int randomPosition );
    
    void getAvailableCells();

    void loseGame();
    void winGame();
};

#endif

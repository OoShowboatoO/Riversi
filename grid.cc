#include "grid.h"
#include "textdisplay.h"
#include "graphicdisplay.h"


// return ture if cell{r, c} is a valid position on the grid, return false otehrwise.
bool checkValid(int r, int c,int gridSize) {
  if (r < 0 || r >= gridSize) {
    return false;
  } else if (c < 0 || c >= gridSize) {
    return false;
  }
  return true;
}


Grid::~Grid() {
  delete td;
  delete ob;
}
  
void Grid::setObserver(Observer<Info, State> *ob) {
  this->ob = ob;
}

// Is the game over, i.e., is the grid full?
bool Grid::isFull() const {
  for (unsigned int i = 0; i < gridSize; ++i){
    for (unsigned int j = 0; j < gridSize; ++j){
      if(theGrid[i][j].getInfo().colour == Colour::NoColour) {
        return false;
      }
    }
  }
  return true;
}


// Who has more pieces when the board is full?
Colour Grid::whoWon() const {
  int Bnum = 0;
  int Wnum = 0;
  for (unsigned int i = 0; i < gridSize; ++i){
    for (unsigned int j = 0; j < gridSize; ++j){
      if(theGrid[i][j].getInfo().colour == Colour::Black) {
        Bnum += 1;
      } else if (theGrid[i][j].getInfo().colour == Colour::White) {
        Wnum += 1;
      }
    }
  }
  if (Bnum == Wnum) {return Colour::NoColour;}
  return (Bnum > Wnum ? Colour::Black : Colour::White);
}

// Sets up an n x n grid.  Clears old grid, if necessary.
// assume n is valid.
void Grid::init(size_t n) {
  this->gridSize = n;
  
  delete td;
  delete ob;
  td = new TextDisplay{(int) n};
  ob = new GraphicDisplay{(int) n};
  
  // initialize all cells on the grid.
  for (unsigned int i = 0; i < gridSize; ++i) {
    std::vector<Cell> new_row;
    theGrid.emplace_back(new_row);
    for (unsigned int j = 0; j < gridSize; ++j) {
      theGrid[i].emplace_back(Cell{i, j});
    }
  }

  // attach all valid observers for every cell on the grid.
  for (unsigned int i = 0; i < gridSize; ++i) {
    for (unsigned int j = 0; j < gridSize; ++j) {
      for (int row = -1; row <= 1; ++row) {
        for (int col = -1; col <= 1; ++col) {
          if (row == 0 && col == 0) continue;
          int neiRow = i + row;
          int neiCol = j + col;
          if (checkValid(neiRow, neiCol, (int) gridSize)){
            theGrid[i][j].attach(&theGrid[neiRow][neiCol]);
          }
        }
      }
    }
  }

  // set the central 4 cells before the game starts
  theGrid[gridSize / 2 - 1][gridSize / 2 - 1].setPiece(Colour::Black);
  td->notify(theGrid[gridSize / 2 - 1][gridSize / 2 - 1]);
  ob->notify(theGrid[gridSize / 2 - 1][gridSize / 2 - 1]);
  
  theGrid[gridSize / 2 - 1][gridSize / 2].setPiece(Colour::White);
  td->notify(theGrid[gridSize / 2 - 1][gridSize / 2]);
  ob->notify(theGrid[gridSize / 2 - 1][gridSize / 2]);
  
  theGrid[gridSize / 2][gridSize / 2 - 1].setPiece(Colour::White);
  td->notify(theGrid[gridSize / 2][gridSize / 2 - 1]);
  ob->notify(theGrid[gridSize / 2][gridSize / 2 - 1]);
  
  theGrid[gridSize / 2][gridSize / 2].setPiece(Colour::Black);
  td->notify(theGrid[gridSize / 2][gridSize / 2]);
  ob->notify(theGrid[gridSize / 2][gridSize / 2]);

}

// Plays piece at row r, col c.
void Grid::setPiece(size_t r, size_t c, Colour colour) {
  if (!checkValid((int) r, (int) c, (int) this->gridSize)) {return;} 
  theGrid[r][c].setPiece(colour);
  theGrid[r][c].notifyObservers();
  for (int i = 0; i < ((int) gridSize); ++i) {
    for (int j = 0; j < ((int) gridSize); ++j) {
      td->notify(theGrid[i][j]);
      ob->notify(theGrid[i][j]);
    }
  }
}

// Flips piece at row r, col c.
void Grid::toggle(size_t r, size_t c) {
  theGrid[r][c].toggle();
}  


std::ostream &operator<<(std::ostream &out, const Grid &g) {
  out << *(g.td);
  return out;
}

bool Grid::isFilled(size_t r, size_t c) {
  Colour cellColour = theGrid[r][c].getInfo().colour;
  if (cellColour == Colour::NoColour) {
    return false;
  }
  return true;
}

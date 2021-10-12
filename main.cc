#include <iostream>
#include <string>
// You may include other allowed headers, as needed
#include "grid.h"
#include "state.h"
using namespace std;

// Do not remove any code; do not add code other than where indicated.

int main(int argc, char *argv[]) {
  cin.exceptions(ios::eofbit|ios::failbit);
  string cmd;
  Grid g;

  // Add code here
  int realSize;
  int step = 0;
  
  try {
    while (true) {
      cin >> cmd;
      if (cmd == "new") {
        int n;
        cin >> n;
        if (n >= 4 && n % 2 == 0) {
          realSize = n;
          g.init(n);
          step = 0;
          std::cout << g;
        } else {
          continue;
        }
      }
      else if (cmd == "play") {
        int r = 0, c = 0;
        cin >> r >> c;
        if (r >= 0 && r < realSize && c >= 0 && c < realSize) {
          if (!g.isFilled(r, c)){
            if (step % 2 == 0) {
              g.setPiece(r, c, Colour::Black);
            } else {
              g.setPiece(r, c, Colour::White);
            }
            step += 1;
            std::cout << g;
            if (g.isFull()) {
              Colour winner = g.whoWon();
              if (winner == Colour::Black) {
                std::cout << "Black Wins!" << std::endl;
              } else if (winner == Colour::White) {
                std::cout << "White Wins!" << std::endl;
              } else {
                std::cout << "Tie!" << std::endl;
              }
              break;
            }
          }
        } else {
          continue;
        }
      }
      
    }
  }
  catch (ios::failure &) {}  // Any I/O failure quits
}

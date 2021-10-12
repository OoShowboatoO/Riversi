#include "textdisplay.h"
#include "cell.h"

TextDisplay::TextDisplay(int n): gridSize{n} {
	for (int i = 0; i < gridSize; ++i){
		std::vector<char> newRow;
		theDisplay.emplace_back(newRow);
		for (int j = 0; j< gridSize; ++j) {
			theDisplay[i].emplace_back('-');
		}
	}
}


void TextDisplay::notify(Subject<Info, State> &whoNotified) {
	Info cellInfo = whoNotified.getInfo();
	int row = cellInfo.row;
	int col = cellInfo.col;
	char curr = '-';
	if (cellInfo.colour == Colour::Black) {
		curr = 'B';
	} else if (cellInfo.colour == Colour::White) {
		curr = 'W';
	}
	theDisplay[row][col] = curr;
}


std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
	int theGridSize = td.gridSize;
	for (int i = 0; i < theGridSize; ++i) {
		for (int j = 0; j < theGridSize; ++j) {
			out << td.theDisplay[i][j]; 
		}
		out << std::endl;
	}
	return out;
}

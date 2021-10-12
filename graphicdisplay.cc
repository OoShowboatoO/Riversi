#include "graphicdisplay.h"
#include "cell.h"

GraphicDisplay::GraphicDisplay(int n) {
	int gridSize = n;
	int fixedSize = 500;
	this->cellSize = fixedSize / n;

	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			xwd.fillRectangle(i * cellSize, j * cellSize, cellSize, cellSize, Xwindow::Blue);
		}
	}
}


void GraphicDisplay::notify(Subject<Info, State> &whoNotified) {
	Info cellInfo = whoNotified.getInfo();
	int row = cellInfo.row;
	int col = cellInfo.col;
	int recColor = Xwindow::Blue;
	if (cellInfo.colour == Colour::Black) {
		recColor = Xwindow::Black;
	} else if (cellInfo.colour == Colour::White) {
		recColor = Xwindow::White;
	}
	xwd.fillRectangle(col * cellSize, row * cellSize, cellSize, cellSize, recColor);
}

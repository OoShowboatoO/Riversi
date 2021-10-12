#include "cell.h"
#include <iostream>


Cell::Cell(size_t r, size_t c): r{r}, c{c} {}


// Place a piece of given colour here.
void Cell::setPiece(Colour colour) {
	this->colour = colour;
	this->setState(State{StateType::NewPiece});
}

// Toggles my colour.    
void Cell::toggle() {
	if(this->colour == Colour::Black) {
		this->colour = Colour::White;
	} else if (this->colour == Colour::White) {
		this->colour = Colour::Black;
	}
}

// return the direction according to rowDiff and colDiff.
Direction relayDirec(int rowDiff, int colDiff) {
	if(rowDiff == -1 && colDiff == -1) {
		return Direction::NW;
	} else if (rowDiff == -1 && colDiff == 0) {
		return Direction::N;
	} else if (rowDiff == -1 && colDiff == 1) {
		return Direction::NE;
	} else if (rowDiff == 0 && colDiff == -1) {
		return Direction::W;
	} else if (rowDiff == 0 && colDiff == 1) {
		return Direction::E;
	} else if (rowDiff == 1 && colDiff == 1) {
		return Direction::SW;
	} else if (rowDiff == 1 && colDiff == 0) {
		return Direction::S;
	} else {
		return Direction::SE;
	}
}

// My neighbours will call this when they've changed state
void Cell::notify(Subject<Info, State> &whoFrom){
	State sourceState = whoFrom.getState();
	Info sourceInfo = whoFrom.getInfo();
	StateType currState;
	Colour sourceColour;
	Direction sourceDirc;

	if (this->colour == Colour::NoColour) {
		return;
	}
	if (sourceState.type == StateType::NewPiece){ // whoFrom is a newPiece
		if (this->colour == Colour::NoColour) { // if I am an empty cell
			return;
		} else if (sourceInfo.colour == this->colour) { // if I have the same colour with whoFrom
			currState = StateType::Reply;
			sourceColour = this->colour;
		} else {  // if I have different colour with whoFrom
			currState = StateType::Relay;
			sourceColour = sourceInfo.colour;
		}
		
		// set Direction for the current cell
		int rowDiff = sourceInfo.row - this->r;
		int colDiff = sourceInfo.col - this->c;
		sourceDirc = relayDirec(rowDiff, colDiff);
		this->setState(State{currState, sourceColour, sourceDirc});
		this->notifyObservers();

	} else if(sourceState.type == StateType::Relay) { // whoFrom is a Relay cell
			int rowDiff = sourceInfo.row - this->r;
			int colDiff = sourceInfo.col - this->c;
			sourceDirc = relayDirec(rowDiff, colDiff);
		if (sourceState.colour == this->colour) { // need to reply
			if (sourceDirc == sourceState.direction) {
				State myState{StateType::Reply, sourceState.colour, sourceDirc};
				this->setState(myState);
				this->notifyObservers();
			}
		} else { // keep relaying
			if (sourceState.direction != sourceDirc) {
				return;
			} else {
				State myState{sourceState.type, sourceState.colour, sourceState.direction};
				this->setState(myState);
				this->notifyObservers();	
			}
		}
	} else { // // whoFrom is a Reply cell
		State myState = this->getState();
		if (myState.type == StateType::Relay) {
			if (this->colour != sourceState.colour &&
				myState.direction == sourceState.direction) {
				this->toggle();
				myState.type = StateType::Reply;
				this->setState(myState);
				this->notifyObservers();
			}
		} else { 
			return;
		}
	}
}


Info Cell::getInfo() const{
	return Info{r, c, colour};
}


#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__
#include "observer.h"
#include "window.h"
#include "state.h"
#include "info.h"

class GraphicDisplay: public Observer<Info, State> {
	Xwindow xwd;
	int cellSize;

public:
	GraphicDisplay(int n);

	void notify(Subject<Info, State> &whoNotified) override;

};

#endif

#pragma once

#include <vector>
#include <string>
#include <map>
#include <SDL.h>
#include "Sys.h"

using namespace std;

typedef struct
{
	string sMsg;
	int x,y;
} RWtextEntry;

typedef struct
{
	int x, y, w, h, color;
} RWboxEntry;

typedef struct 
{
	int x, y;
	int yMin, yMax;
	int iWidth;
	int iFlags;
	map<int,vector<int> > mValues;
	bool bVisible;
} RWlinegraph;

typedef enum
{
	RWG_ANCHOR_CENTER = 1 << 0,
	//RWG_ANCHOR_BOTTOM = 1 << 1,

	RWG_ANCHOR_FLAGS_CT
} RWgraphflags;

class RWtext
{	
protected:
	GLuint hTexture;

	void drawString(int x, int y, string sMsg);
	void drawBox(int x, int y, int w, int h, int color);

	void printx(int x, int y, char *fmt, va_list list);

public:
	vector<RWtextEntry> cEntries;
	vector<RWboxEntry> cBoxes;
	map<string,RWlinegraph> mGraphs;

	RWtext();

	void init();
	void render();
	// print at "character pos" x,y
	void print(int x, int y, char *fmt, ...);
	// print at piXel pos.
	void printx(int x, int y, char *fmt, ...);
	void box(int x, int y, int w, int h, int color = 0xFF00FF);

	void createGraph(const char *sName, int x, int y, int iWidth, int iFlags);
	void addGraphVal(const char *sName, int iColor, int iValue);
};

extern RWtext rwtext;

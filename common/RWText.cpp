#include "RWText.h"
#include "Debug.h"
#include <SDL_image.h>

RWtext rwtext;

int CHAR_SIZE = 16;

RWtext::RWtext()
{
}

/**
// load text from a BMP.
void RWtext::init()
{
	SDL_Surface *sSurface;
	sSurface = SDL_LoadBMP("text.bmp");
	GLenum g;

	glGenTextures(1,&hTexture);
	g = glGetError();
	if (g>0) fatal("oh1: %d", g);

	glBindTexture(GL_TEXTURE_2D, hTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sSurface->w, sSurface->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, sSurface->pixels);
	g = glGetError();
	if (g>0) fatal("oh: %d", g);
}*/

// load text from a transparent TGA.
void RWtext::init()
{
	SDL_Surface *sSurface;
	sSurface = IMG_Load("bigchars.tga"); // SDL_LoadBMP("bigchars.tga");
	GLenum g;

	glGenTextures(1,&hTexture);
	checkOpenGL("No texture for text");

	glBindTexture(GL_TEXTURE_2D, hTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sSurface->w, sSurface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, sSurface->pixels);
	checkOpenGL("Configure text texture");

	SDL_FreeSurface(sSurface);
}

void RWtext::print(int x, int y, char *fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	this->printx(x*CHAR_SIZE, y*CHAR_SIZE, fmt, list);
	va_end(list);
}

void RWtext::printx(int x, int y, char *fmt, va_list list) {
	char res[200];
	vsnprintf(res, sizeof(res), fmt, list);

	RWtextEntry sEntry;

	sEntry.x = x;
	sEntry.y = y;
	sEntry.sMsg = res;
	this->cEntries.push_back(sEntry);
}

void RWtext::printx(int x, int y, char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	this->printx(x, y, fmt, list);
	va_end(list);
}

void RWtext::box(int x, int y, int w, int h, int color)
{
	RWboxEntry sBox;
	sBox.x = x;
	sBox.y = y;
	sBox.w = w;
	sBox.h = h;
	sBox.color = color;
	this->cBoxes.push_back(sBox);
}

void RWtext::render()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 800, 600, 0);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D, hTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	vector<RWtextEntry>::iterator it;
	for(it = cEntries.begin(); it != cEntries.end(); it++)
	{
		this->drawString((*it).x, (*it).y, (*it).sMsg);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	vector<RWboxEntry>::iterator boxIt;
	for(boxIt = cBoxes.begin(); boxIt != cBoxes.end(); boxIt++)
	{
		RWboxEntry box = *boxIt;
		drawBox(box.x,box.y,box.w,box.h,box.color);
	}
	glEnd();

	
	map<string,RWlinegraph>::iterator graphIt;
	for(graphIt = mGraphs.begin(); graphIt != mGraphs.end(); graphIt++)
	{
		const RWlinegraph g = (*graphIt).second;
		int iBaseline;
		int iHeight = g.yMax-g.yMin;
		glBegin(GL_QUADS);
		if (g.iFlags & RWG_ANCHOR_CENTER)
		{
			iBaseline = g.y;// + iHeight/2;
			drawBox(g.x, g.y - iHeight/2, g.iWidth, iHeight, 0x00);
		}
		glEnd();

		//int iDelta = g.iWidth / g.iLen;
		//int y = g.y + (g.yMax-g.yMin)/2;

		glBegin(GL_LINES);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(g.x, iBaseline, 0.05);
		glVertex3f(g.x + g.iWidth, iBaseline, 0.05);

		map<int,vector<int>>::const_iterator itVals;
		for(itVals = g.mValues.begin(); itVals != g.mValues.end(); itVals++)
		{
			const int color = (*itVals).first;
			glColor3ub((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
			vector<int>::const_iterator itVal;
			int iOffs = 0;
			for(itVal = (*itVals).second.begin(); itVal != (*itVals).second.end(); itVal++)
			{
				glVertex3f(iOffs + g.x, iBaseline, 0.1);
				glVertex3f(iOffs + g.x, iBaseline - (*itVal),0.1);
				iOffs += 1;
			}
		}
		glEnd();
		
	}
	

	glPopMatrix();
	glPopAttrib();

	cEntries.clear();
	cBoxes.clear();
}

void RWtext::drawString(int x, int y, string sMsg)
{
	int fs = CHAR_SIZE;
//	int fx = x;// * fs;
//	int fy = y;// * fs;
	
	const GLfloat td = 0.0625f;

	GLfloat tx, ty;
	for(int i = 0; i < sMsg.length(); i++)
	{
		byte c = sMsg[i];
		tx = (c % 16) / 16.0;
		ty = (c / 16) * td;
		glTexCoord2f(tx, ty);       glVertex2d(x,		y);
		glTexCoord2f(tx, ty+td);    glVertex2d(x,		y+fs);
		glTexCoord2f(tx+td, ty+td); glVertex2d(x+fs,	y+fs);
		glTexCoord2f(tx+td, ty);    glVertex2d(x+fs,	y);
	
		x+=fs;
	}
}

void RWtext::drawBox(int x, int y, int w, int h, int color)
{
	glColor3ub((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
	//glColor3f(1,0,0);
	glVertex2d(x, y);
	glVertex2d(x,y+h);
	glVertex2d(x+w,y+h);
	glVertex2d(x+w,y);
}

void RWtext::createGraph(const char *sName, int x, int y, int iWidth, int iFlags)
{
	mGraphs[sName].x = x;
	mGraphs[sName].y = y;
	mGraphs[sName].iWidth = iWidth;
	mGraphs[sName].yMin = -1;
	mGraphs[sName].yMax = 1;
	mGraphs[sName].iFlags = iFlags;
}

void RWtext::addGraphVal(const char *sName, int iColor, int iValue)
{
	if (mGraphs.find(sName) == mGraphs.end()) return;
	mGraphs[sName].mValues[iColor].push_back(iValue);
	if (mGraphs[sName].mValues[iColor].size() > mGraphs[sName].iWidth)
	{
		mGraphs[sName].mValues[iColor].erase(mGraphs[sName].mValues[iColor].begin());
	}
	if (iValue < mGraphs[sName].yMin) mGraphs[sName].yMin = iValue;
	if (iValue > mGraphs[sName].yMax) mGraphs[sName].yMax = iValue;
}

#include "TeapotShadowScene.h"

#include "Debug.h"
#include "teapot.h"

TeapotShadowScene::TeapotShadowScene() {
	const GLcharARB *sprogVert = "\
		varying vec4 vertCol; varying vec4 lightTexPos; \
		void main() { \
			gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; \
			lightTexPos = gl_TextureMatrix[7] * gl_Vertex; \
			lightTexPos = lightTexPos / lightTexPos[3]; \
			vertCol = gl_Color; \
		}";
	const GLcharARB *sprogFrag = "\
		 varying vec4 vertCol; varying vec4 lightTexPos; uniform sampler2D depthTexture;  \
		 void main() { \
		     vec4 lightFactor; \
			 gl_FragColor = vertCol * 0.1;  \
			 if ((lightTexPos[0] >= 0.0) && (lightTexPos[0] <= 1.0) && (lightTexPos[1] >= 0.0) && (lightTexPos[1] <= 1.0)) { \
			 lightFactor = texture2D(depthTexture, vec2(lightTexPos));\
				gl_FragColor = gl_FragColor + vertCol * lightFactor; \
			 } \
		 }";

	//const GLcharARB *sprogFrag = "varying vec4 vertCol; varying vec4 lightTexPos; void main() { gl_FragColor = vertCol; } ";

	this->shadowProg = createShaderFromProgs(sprogVert, sprogFrag);
	checkOpenGL("shadow prog");
}

static GLfloat bias[] = { 0.5f, 0, 0, 0,     0, 0.5f, 0, 0,    0, 0, 0.5f, 0,     0.5f, 0.5f, 0.5f, 1 };

static void scaleAndOffs(float scale, float x, float y, float z) 
{
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(x, y, z);
	glActiveTextureARB(GL_TEXTURE0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(x, y, z);
}

static void unscale() 
{
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPopMatrix();
	glActiveTextureARB(GL_TEXTURE0);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void TeapotShadowScene::render(bool isLit, Spotlight *spotlight) {
	if (isLit) {
		glUseProgramObjectARB(shadowProg);
		glMatrixMode(GL_TEXTURE);
		glActiveTextureARB(GL_TEXTURE7);
		glLoadIdentity();
		glMultMatrixf(bias);		
		glMultMatrixf(spotlight->projMatrix);
		glMultMatrixf(spotlight->mviewMatrix);

		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, spotlight->depthBuffer);
	} else {
		glUseProgramObjectARB(0);
	}

	glMatrixMode(GL_MODELVIEW);

	glColor4f(1.f, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.f,   0.f, -0.f);    
	glVertex3f(100.f, 0.f, 0.f);     
	glVertex3f(0.f,   100.f, -0.f); 
	
	glVertex3f(100.f, 0.f, -0.f);    
	glVertex3f(0.f,   100.f, 0.f);   
	glVertex3f(100.f, 100.f, -0.f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glColor3f(1, 1, 0); glVertex3f(110, 110, 0);
	glColor3f(1, 0, 1); glVertex3f(110, 200, 0);
	glColor3f(0, 1, 1); glVertex3f(200, 110, 0);

	glEnd();

	for(int i = 0; i < 5; i++) 
	{
		for(int j = 0; j < 5; j++) 
		{
			scaleAndOffs(1.5f, 25 + i * 5.f, 25 + j * 5.f, 50);
			drawTeapot();
			unscale();
		}
	}
}

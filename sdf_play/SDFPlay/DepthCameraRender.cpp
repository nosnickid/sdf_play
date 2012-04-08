#include "DepthCameraRenderer.h"
#include "ManualDepthMap.h"
#include "glsl.h"

DepthCameraRenderer::~DepthCameraRenderer() {
	delete this->kin;
	// delete this->depth;
}

void DepthCameraRenderer::init() {
	//this->cam = new SdfCvCamera();
	this->kin = new SdfCvImageKinect();
	this->kin->init();

	this->rgb = (AbstractRgbImage *) this->kin;

	this->depth = new ManualDepthMap();
	this->depth->loadDepthMap();


	const GLcharARB *tprogVert = "\
		varying vec2 texcoord; \
		uniform sampler2D depthTexture; \
		uniform vec4 surfaceNormal; \
		void main() { \
			texcoord = vec2(gl_MultiTexCoord0); \
		    gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);\
			gl_Position += (30 * surfaceNormal * (texture2D(depthTexture, texcoord)[0]));\
		}";
	const GLcharARB *tprogFrag = "\
		varying vec2 texcoord; \
		uniform sampler2D camTexture; \
		void main() { \
			gl_FragColor = texture2D(camTexture, texcoord);\
		}";
	this->depthProg = createShaderFromProgs(tprogVert, tprogFrag);
	glUseProgramObjectARB(this->depthProg);
	this->glslColorTexture = glGetUniformLocationARB(this->depthProg, "camTexture");
	//*
	this->glslDepthTexture = glGetUniformLocationARB(this->depthProg, "depthTexture");
	this->glslSurfaceNormal = glGetUniformLocationARB(this->depthProg, "surfaceNormal");
	checkOpenGL("get surface normal from depth prog");
	GLfloat norm[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fvARB(this->glslSurfaceNormal, 1, norm);
	//*/
	glUniform1iARB(this->glslDepthTexture, 0);
	glUniform1iARB(this->glslColorTexture, 1);


	checkOpenGL("depth prog");
}

void DepthCameraRenderer::prepareFrame() {
	this->kin->prepareFrame();
}

void DepthCameraRenderer::render() {	
	glUseProgramObjectARB(this->depthProg);

	//*
	glActiveTextureARB(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, this->depth->getDepthMapTexture());
	checkOpenGL("bind depth texture");
	//*/
	glActiveTextureARB(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->rgb->getRgbImageTexture());
	checkOpenGL("bind cam texture");

	
	int w = 320;
	int h = 240;
	int s = 1;

	double tistep = 1 / (double) w;
	double tjstep = 1 / (double) h;
	double ti = 0, tj;
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < w; i++, ti += tistep) {
		double ti = i / (double) w;
		int j;
		for(j = 0, tj = 0.0; j < h; j++, tj += tjstep) {
			// 0
			glTexCoord2d(ti, tj); 
			glVertex3i(i, 0, j);

			// 1
			glTexCoord2d(ti + tistep, tj); 
			glVertex3i(i+s, 0, j);

			// 2
			glTexCoord2d(ti + tistep, tj + tjstep);
			glVertex3i(i+s, 0, j+s);

			// 2
			glTexCoord2d(ti + tistep, tj + tjstep);
			glVertex3i(i+s, 0, j+s);

			// 3
			glTexCoord2d(ti, tj + tjstep); 
			glVertex3i(i, 0, j+s);

			// 0
			glTexCoord2d(ti, tj); 
			glVertex3i(i, 0, j);

		}
	}
	glEnd();

	glActiveTextureARB(GL_TEXTURE0);

}

/*
			// 0
			glTexCoord2d(ti, tj); 
			glVertex3i(i, 0, j);

			// 1
			glTexCoord2d(ti, tj + tjstep); 
			glVertex3i(i+s, 0, j);

			// 2
			glTexCoord2d(ti + tistep, tj + tjstep);
			glVertex3i(i+s, 0, j+s);

			// 2
			glTexCoord2d(ti + tistep, tj + tjstep);
			glVertex3i(i+s, 0, j+s);

			// 3
			glTexCoord2d(ti + tistep, tj); 
			glVertex3i(i, 0, j+s);

			// 0
			glTexCoord2d(ti, tj); 
			glVertex3i(i, 0, j);

*/
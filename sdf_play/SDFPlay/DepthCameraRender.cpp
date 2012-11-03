#include "DepthCameraRenderer.h"
#include "ManualDepthMap.h"
#include "glsl.h"

DepthCameraRenderer::DepthCameraRenderer():
	image(NULL), rgb(NULL), depth(NULL) {
}

DepthCameraRenderer::~DepthCameraRenderer() {
	if (this->depth != this->image && this->depth) {
		delete this->depth;
	}
	if (this->image != NULL) {
		delete (this->image);
	}
	
}

void DepthCameraRenderer::init() {
#ifdef _WIN32
	if (SdfCvImageKinect::IsKinectPresent()) {
		this->image = new SdfCvImageKinect();
		this->image->init();

		this->rgb = dynamic_cast<AbstractRgbImage*> (this->image);
		this->depth = dynamic_cast<AbstractDepthMap*> (this->image);
	}
#endif
	if (this->image == NULL) {
		this->depth = new ManualDepthMap();
		(dynamic_cast<ManualDepthMap*> (this->depth))->loadDepthMap();

		this->image = new SdfCvCamera(this->depth);
		this->image->init();

		this->rgb = dynamic_cast<AbstractRgbImage*> (this->image);
	}

	// Build the depth rendering program
	const GLcharARB *tprogVert = "\
		varying vec2 texcoord; \
		uniform sampler2D depthTexture; \
		uniform vec4 surfaceNormal; \
		void main() { \
			texcoord = vec2(gl_MultiTexCoord0); \
			gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex);\
			vec4 depth = texture2D(depthTexture, texcoord);\
			gl_Position += (100.0 * surfaceNormal * (depth.x));\
		}\n";

	int maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
	if (maxVertexTextureImageUnits == 0) {
		warning("Depth camera render will not show depth - no vert shader samplers");
		tprogVert = "\
			varying vec2 texcoord; \
			void main() { \
				texcoord = vec2(gl_MultiTexCoord0); \
				gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex); \
			}";
	}

	const GLcharARB *tprogFrag = "\
		varying vec2 texcoord; \
		uniform sampler2D camTexture; \
		uniform sampler2D depthTexture; \
		void main() { \
			gl_FragColor = texture2D(camTexture, texcoord);\
		}\n";
	

	this->depthProg = createShaderFromProgs(tprogVert, tprogFrag);
	checkOpenGL("create shader prog for depth camera");

	glUseProgramObjectARB(this->depthProg);
	checkOpenGL("use shader program for depth camera");

	GLint glslDepthTexture;
	GLint glslColorTexture;
	GLint glslSurfaceNormal;

	// Get refs to the uniforms in the compiled progam and set the values.
	glslColorTexture = glGetUniformLocationARB(this->depthProg, "camTexture");
	glUniform1iARB(glslColorTexture, 1);
	checkOpenGL("get cam texture location from depth prog");
	glslDepthTexture = glGetUniformLocationARB(this->depthProg, "depthTexture");
	checkOpenGL("get depth texture location from depth prog");
	glUniform1iARB(glslDepthTexture, 0);

	if (maxVertexTextureImageUnits > 0) {
		GLfloat norm[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		glslSurfaceNormal = glGetUniformLocationARB(this->depthProg, "surfaceNormal");
		checkOpenGL("get surface normal from depth prog");
		glUniform4fvARB(glslSurfaceNormal, 1, norm);
	}

	checkOpenGL("depth prog");
}

void DepthCameraRenderer::prepareFrame() {
	if (this->image != NULL) {
		this->image->prepareFrame();
	}
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

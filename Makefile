default: all

SDF_SOURCES=sdf_play/SDFPlay/DepthCameraRender.cpp sdf_play/SDFPlay/Gl2dRender.cpp \
	sdf_play/SDFPlay/ManualDepthMap.cpp sdf_play/SDFPlay/QrCodeParser.cpp \
	sdf_play/SDFPlay/SdfCvCamera.cpp sdf_play/SDFPlay/SdfCvImage.cpp \
	sdf_play/SDFPlay/SdfPlayApp.cpp sdf_play/SDFPlay/SdfPlaySdlAudio.cpp \
	sdf_play/SDFPlay/Spotlight.cpp sdf_play/SDFPlay/TeapotShadowScene.cpp \
	sdf_play/SDFPlay/TextureDebugRenderer.cpp sdf_play/SDFPlay/VertexModel.cpp \
	sdf_play/SDFPlay/glsl.cpp sdf_play/SDFPlay/main.cpp \
	sdf_play/SDFPlay/sdl_glframebufferext.cpp sdf_play/SDFPlay/sdl_glsl.cpp

SOURCES=$(SDF_SOURCES) common/Debug.cpp common/FpsCamera.cpp
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDE=-I3rdparty/include -Icommon -I3rdparty/include/linux
#LIBS=-L3rdparty/lib-cygwin -lSDL -lSDLmain  
LIBS=-L3rdparty/lib-linux -lSDL -lpthread -ldl -lSDL_image -lGL -lGLU -loglconsole -lopencv_core -lopencv_highgui
EXE=play

CXXFLAGS=$(INCLUDE) -Wall -ggdb -DOGLCONSOLE_USE_SDL $(shell sdl-config  --cflags)

CXX=g++

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

all: $(OBJECTS) $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $@

run: $(EXE)
	cd runtime && nice ../$(EXE)

debug: $(EXE)
	cd runtime && ddd --no-disassemble ../$(EXE)

grind: $(EXE)
	cd runtime && valgrind --track-origins=yes ../$(EXE)

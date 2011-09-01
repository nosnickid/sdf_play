default: all

SOURCES=common/Debug.cpp common/FpsCamera.cpp sdf_play/SDFPlay/glsl.cpp  sdf_play/SDFPlay/main.cpp  sdf_play/SDFPlay/sdl_glframebufferext.cpp  sdf_play/SDFPlay/sdl_glsl.cpp sdf_play/SDFPlay/Spotlight.cpp 
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDE=-I3rdparty/include -Icommon -I3rdparty/include/linux 
#LIBS=-L3rdparty/lib-cygwin -lSDL -lSDLmain 
LIBS=-L3rdparty/lib-linux -lSDL -lpthread -ldl -lSDL_image -lGL -lGLU -loglconsole
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

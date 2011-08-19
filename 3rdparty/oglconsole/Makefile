CFLAGS:=$(CFLAGS) -ggdb3 -gstabs+ -Wall
LDFLAGS:=$(LDFLAGS)

lib:  liboglconsole.a
all : oglconsole-sdl.o #oglconsole-glut.o
clean: 
	-rm *.o *.a

liboglconsole.a: oglconsole-sdl.o
	-ar rcs liboglconsole.a oglconsole-sdl.o 


oglconsole-sdl.o : oglconsole.c oglconsole.h ConsoleFont.c
	$(CC) $(CFLAGS) -DOGLCONSOLE_USE_SDL $(shell sdl-config --cflags) -c $< -o $@

oglconsole-glut.o : oglconsole.c oglconsole.h ConsoleFont.c
	$(CC) $(CFLAGS) -DOGLCONSOLE_USE_GLUT -c $< -o $@



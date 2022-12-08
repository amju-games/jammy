all:jammy_exe

clean:
	rm jammy *.o ../../lo-res/*.o ../../microlib/*.o 

run:
	./jammy


CC=clang++

WARNINGS=-Wall -Wno-deprecated-declarations

INCLUDES=\
  -I.. \
  -I../../lo-res \
  -I../../microlib

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++11   -DMACOSX

LDFLAGS=-framework OpenGL -framework GLUT

SOURCES=\
global_palette.cpp \
jammy_game_object.cpp \
main.cpp \
player.cpp \
play_state.cpp \
../../lo-res/font.cpp \
../../lo-res/image.cpp \
../../lo-res/lodepng.cpp \
../../lo-res/palette.cpp \
../../lo-res/screen.cpp \
../../lo-res/sprite.cpp \
../../lo-res/sprite_sheet.cpp \
../../microlib/game.cpp \
../../microlib/game_object.cpp \
../../microlib/message_queue.cpp \
../../microlib/timer.cpp


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy

jammy_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


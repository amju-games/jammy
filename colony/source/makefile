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

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++11   -DMACOSX  -g -O0 

LDFLAGS=-framework OpenGL -framework GLUT -framework Carbon libbass.dylib 

SOURCES=\
enter_hi_score_state.cpp \
game_over_state.cpp \
globals.cpp \
hi_scores.cpp \
hq.cpp \
human.cpp \
human_list.cpp \
jammy_game_object.cpp \
main.cpp \
parallax_bg.cpp \
player.cpp \
play_state.cpp \
rock.cpp \
splash_state.cpp \
../../lo-res/font.cpp \
../../lo-res/image.cpp \
../../lo-res/lodepng.cpp \
../../lo-res/palette.cpp \
../../lo-res/screen.cpp \
../../lo-res/sprite.cpp \
../../lo-res/sprite_sheet.cpp \
../../microlib/directory.cpp \
../../microlib/file_string_utils.cpp \
../../microlib/game.cpp \
../../microlib/game_object.cpp \
../../microlib/message_queue.cpp \
../../microlib/sound_player_bass24.cpp \
../../microlib/timer.cpp


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy

jammy_exe: $(SOURCES) $(EXECUTABLE)
	install_name_tool -add_rpath @executable_path/ $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


all:village_exe

clean:
	rm jammy_village *.o ../../../lo-res/*.o ../../../microlib/*.o 

run:
	./jammy_village


CC=clang++

WARNINGS=-Wall -Wno-deprecated-declarations

INCLUDES=\
  -I.. \
  -I../../../lo-res \
  -I../../../lo-res/third_party \
  -I../../../microlib

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++20 -DMACOSX  -g -O0 

LDFLAGS=-framework OpenGL -framework GLUT -framework Carbon libbass.dylib 

SOURCES=\
collision_player_bullet_rock.cpp \
enter_hi_score_state.cpp \
game_over_state.cpp \
globals.cpp \
hi_scores.cpp \
hq.cpp \
human.cpp \
human_list.cpp \
jammy_collision_mgr.cpp \
jammy_game_object.cpp \
main.cpp \
parallax_bg.cpp \
player.cpp \
player_bullet.cpp \
play_state.cpp \
resources.cpp \
rock.cpp \
splash_state.cpp \
universe.cpp \
walker.cpp \
../../../lo-res/image_8.cpp \
../../../lo-res/image_32.cpp \
../../../lo-res/image_colour_xform.cpp \
../../../lo-res/palette.cpp \
../../../lo-res/render_image_opengl.cpp \
../../../lo-res/sprite.cpp \
../../../lo-res/third_party/lodepng.cpp \
../../../microlib/directory.cpp \
../../../microlib/file_string_utils.cpp \
../../../microlib/game.cpp \
../../../microlib/game_object.cpp \
../../../microlib/message_queue.cpp \
../../../microlib/sound_player_bass24.cpp \
../../../microlib/string_utils.cpp \
../../../microlib/timer.cpp


OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy_village

village_exe: $(SOURCES) $(EXECUTABLE)
	install_name_tool -add_rpath @executable_path/ $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


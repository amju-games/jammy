all:jammy_tests_exe

clean:
	rm jammy_tests *.o ../*.o

run:
	./jammy_tests


CC=clang++

WARNINGS=-Wall -Wno-deprecated-declarations

INCLUDES=\
  -I.. \
  -I../../../../microlib \
  -I../../../../lo-res \
  -I../../../../lo-res/third_party

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++2a -DMACOSX -DUNIT_TEST

LDFLAGS=-framework Carbon
#-framework OpenGL -framework GLUT

# (some code is just in .h files as templated, so not many .cpp files!)
SOURCES= \
anim_controller_tests.cpp \
auto_repeat_suppressor_tests.cpp \
brute_force_tests.cpp \
catch_test_main.cpp \
hi_scores_tests.cpp \
player_progress_tests.cpp \
resources_tests.cpp \
sweep_and_prune_tests.cpp \
vel_controller_tests.cpp \
../anim_controller.cpp \
../hi_scores.cpp \
../nice_font.cpp \
../resources.cpp \
../vel_controller.cpp \
../../../../lo-res/image_8.cpp \
../../../../lo-res/image_32.cpp \
../../../../lo-res/image_colour_xform.cpp \
../../../../lo-res/image_scale.cpp \
../../../../lo-res/palette.cpp \
../../../../lo-res/third_party/lodepng.cpp \
../../../../microlib/config_file.cpp \
../../../../microlib/directory.cpp \
../../../../microlib/file.cpp \
../../../../microlib/file_string_utils.cpp \
../../../../microlib/game.cpp \
../../../../microlib/game_object.cpp\
../../../../microlib/string_utils.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy_tests

jammy_tests_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


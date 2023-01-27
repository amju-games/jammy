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
  -I../../../../lo-res

CFLAGS=-c $(WARNINGS) $(INCLUDES) -std=c++17   -DMACOSX

LDFLAGS=-framework OpenGL -framework GLUT

# (some code is just in .h files as templated, so not many .cpp files!)
SOURCES= \
brute_force_tests.cpp \
catch_test_main.cpp \
resources_tests.cpp \
sweep_and_prune_tests.cpp \
../resources.cpp \
../../../../lo-res/image_8.cpp \
../../../../lo-res/image_32.cpp \
../../../../lo-res/palette.cpp \
../../../../lo-res/third_party/lodepng.cpp \
../../../../microlib/file_string_utils.cpp \
../../../../microlib/game.cpp \
../../../../microlib/game_object.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy_tests

jammy_tests_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


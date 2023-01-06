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

SOURCES= \
brute_force_tests.cpp \
catch_test_main.cpp \
sweep_and_prune_tests.cpp \
../../../../microlib/game.cpp \
../../../../microlib/game_object.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jammy_tests

jammy_tests_exe: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


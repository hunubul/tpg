CC = g++
CFLAGS = -Wall -fexceptions -O2
INCLDIR = -Ilibtcod/include -Ilodepng
EXTRLIB = libtcod/lib/libtcod-mingw.a lodepng/liblodepng.a

CPP_SRCS = $(wildcard src/*.cpp)
OBJS = $(subst src,obj,${CPP_SRCS:.cpp=.o})

DOXYLOC = C:/Program Files/doxygen/bin/doxygen.exe

compile: $(OBJS)
	$(CC) -s obj/*.o -o bin/main $(EXTRLIB) -mwindows

obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $(INCLDIR) $< -o $@

doxygen:
	$(DOXYLOC) doxygen/doxyfile
	@echo.
	@echo Finished generating documentation in doxygen folder

clean:
	@rm -f -r doxygen/html obj/Debug obj/Release bin/Debug bin/Release
	@rm -f obj/*.o bin/main.exe bin/error.log

.PHONY: doxygen clean
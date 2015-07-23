CC = gcc
CFLAGS = -Wall
INCLDIR = -Ilibtcod/include -Iincludes
EXTRLIB = libtcod/lib/libtcod-mingw.a includes/libImage2ASCIIcolors.a

DOXYLOC = C:/Program Files/doxygen/bin/doxygen.exe

compile: obj/%.o
	$(CC) $(CFLAGS) obj/*.o -o bin/main $(EXTRLIB)

obj/%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $(INCLDIR) $< -o $@

doxygen: src/*.cpp
	$(DOXYLOC) doxygen/doxyfile
	@echo.
	@echo Finished generating documentation in doxygen folder

clean:
	@rm -f -r doxygen/html obj/Debug obj/Release bin/Debug bin/Release
	@rm -f obj/*.o bin/main.exe

.PHONY: doxygen clean
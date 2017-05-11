CC=x86_64-w64-mingw32-gcc
LD=ld
CFLAGS= -c -Iheaders -Wall -Wextra -std=c99 -D DEVELOP
# Change -lgdi32 to -mwindows for console-less mode.
LDFLAGS=-lglew32 -Lsmallwin -lsmallwin -lopengl32 -lgdi32
OUTNAME=test.exe
OFILES=file_io.o shaders.o load_mesh.o main.o

all:
	$(CC) $(CFLAGS) util/file_io.c
	$(CC) $(CFLAGS) shaders.c
	$(CC) $(CFLAGS) main.c
	$(CC) $(CFLAGS) loading/load_mesh.c
	$(CC) $(OFILES) -o $(OUTNAME) $(LDFLAGS)

clean:
	rm -rf *.o


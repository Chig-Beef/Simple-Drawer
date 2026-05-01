all: build run

build:
	gcc *.c -o "dev.exe" -lraylib -lgdi32 -lwinmm

run:
	dev

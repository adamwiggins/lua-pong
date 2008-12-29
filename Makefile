BIN=pong

run: pong
	./pong

pong: pong.c
	clear
	cc pong.c -llua -llualib `pkg-config sdl --libs --cflags` -framework OpenGL -I/opt/local/include -o $(BIN)

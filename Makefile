BIN=pong
TESTBIN=test

run: pong
	./pong

pong: pong.c Makefile
	clear
	cc pong.c -llua -llualib `pkg-config sdl --libs --cflags` -framework OpenGL -I/opt/local/include -o $(BIN)

test: buildtest
	clear
	./$(TESTBIN)

buildtest: test.c Makefile
	clear
	cc test.c -llua -llualib `pkg-config sdl --libs --cflags` -framework OpenGL -I/opt/local/include -o $(TESTBIN)

clean:
	rm -f $(BIN) $(TESTBIN)


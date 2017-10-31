#include <SDL.h>
#include <SDL_opengl.h>
#include <lua.h>
#include <lauxlib.h>

#define bool int
#define false 0
#define true 1

#define max(a, b) (a < b ? b : a)

lua_State *L;

static int draw_rectangle(lua_State *L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);
	float width = lua_tonumber(L, 3);
	float height = lua_tonumber(L, 4);
	float red = lua_tonumber(L, 5);
	float green = lua_tonumber(L, 6);
	float blue = lua_tonumber(L, 7);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(width, 0.0f);
	glColor3f(max(0, red - 0.7), max(0, green - 0.7), max(0, blue - 0.7));
	glVertex2f(width, height);
	glVertex2f(0.0f, height);
	glEnd();
}

static int key_pressed(int key)
{
	Uint8 *keystate = SDL_GetKeyState(NULL);
	int pressed = 0;
	if (keystate[key])
		pressed = 1;
	lua_pushboolean(L, pressed);
	return 1;
}

static int up_pressed() { return key_pressed(SDLK_UP); }
static int down_pressed() { return key_pressed(SDLK_DOWN); }
static int right_pressed() { return key_pressed(SDLK_RIGHT); }
static int left_pressed() { return key_pressed(SDLK_LEFT); }

void pulse_via_lua()
{
	lua_getglobal(L, "pulse");
	if (lua_pcall(L, 0, 0, 0) != 0) {
		printf("error running function: %s\n", lua_tostring(L, -1));
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("usage: engine [script.lua]\n");
		exit(1);
	}

	L = lua_open();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	lua_register(L, "draw_rectangle", draw_rectangle);
	lua_register(L, "up_pressed", up_pressed);
	lua_register(L, "down_pressed", down_pressed);
	lua_register(L, "right_pressed", right_pressed);
	lua_register(L, "left_pressed", left_pressed);

	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Surface *screen = SDL_SetVideoMode(400, 400, 32, SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_OPENGL);

	glViewport(0, 0, 400, 400);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		printf("no such file as %s\n", argv[1]);
		exit(1);
	}
	char buff[10000];
        int size = fread(buff, 1, sizeof(buff)-1, fp);
	buff[size] = 0;
	fclose(fp);

	if (luaL_loadbuffer(L, buff, strlen(buff), "pong") != 0 ||
	    lua_pcall(L, 0, 0, 0) != 0) {
		printf("error loading: %s\n", lua_tostring(L, -1));
		exit(1);
	}

	bool done = false;
	while (!done)
	{
		SDL_Event event;
		SDL_keysym key;
		while (SDL_PollEvent(&event) && !done)
			switch (event.type)
			{
				case SDL_KEYDOWN:
					key = event.key.keysym;
					switch (key.sym)
					{
						case SDLK_ESCAPE: done = true; break;
					}
					break;

				case SDL_QUIT:
					done = true;
					break;
			}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pulse_via_lua();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	lua_close(L);

	return 0;
}

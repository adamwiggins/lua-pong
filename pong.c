#include <SDL.h>
#include <OpenGL/gl.h>
#include <lua.h>
#include <lauxlib.h>

#define bool int
#define false 0
#define true 1

lua_State *L;

static int draw_rectangle(lua_State *L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.3f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();

	SDL_GL_SwapBuffers();
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

int main(int argc, char*argv[])
{
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

	FILE *fp = fopen("pong.lua", "r");
	char buff[10000];
	int size = fread(buff, 1, 10000, fp);
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

		pulse_via_lua();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);
	}

	lua_close(L);

	return 0;
}

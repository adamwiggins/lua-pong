#include <SDL.h>
#include <OpenGL/gl.h>
#include <lua.h>

#define bool int
#define false 0
#define true 1

lua_State *L;

static int draw_rectangle(lua_State *L)
{
//	float theta = lua_tonumber(L, 1);
	float theta = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glRotatef(theta, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
//	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.87f, -0.5f);
//	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(-0.87f, -0.5f);
	glEnd();

	SDL_GL_SwapBuffers();
}

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
	L = luaL_newstate();

	lua_register(L, "draw_rectangle", draw_rectangle);

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
	fread(buff, 10000, 1, fp);
	fclose(fp);

	luaL_loadbuffer(L, buff, strlen(buff), "line");
	if (lua_pcall(L, 0, 0, 0) != 0) {
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

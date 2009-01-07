#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

lua_State *L;

int main(int argc, char*argv[])
{
	L = lua_open();
	luaopen_base(L);
	luaopen_table(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

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

	lua_getglobal(L, "all_tests");
	if (lua_pcall(L, 0, 0, 0) != 0) {
		printf("error running function: %s\n", lua_tostring(L, -1));
		exit(1);
	}

	lua_close(L);

	return 0;
}

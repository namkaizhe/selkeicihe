#include "LuaScript.h"
#include <fstream>
#include <sstream>

namespace Scripting {

	LuaScript::LuaScript(const std::string& file) {
		source = file;
		Reload();
	}
	LuaScript::~LuaScript() {
		lua_close(state);
	}

	void LuaScript::Reload() {

		std::ifstream ifs { source };
		std::stringstream content {};

		content << ifs.rdbuf();
		state = luaL_newstate();
		luaL_openlibs(state);
		luaL_loadbuffer(state, content.str().c_str(), content.str().length(), "CONTENT");
		lua_pcall(state, 0, 0, 0);
		arguments = returns = 0;
	}

}

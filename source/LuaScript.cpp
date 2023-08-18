#include "LuaScript.h"
#include <fstream>
#include <sstream>

namespace Scripting {

	int LuaScript::PushVariable(const std::string& name) {
		PushContainingTable(name);
		std::string member_name = LeafMember(name);
		lua_getfield(state, -1, member_name.c_str());
		lua_remove(state, -2);
		return 1;
	}

	int LuaScript::PushContainingTable(std::string name) {
		lua_pushglobaltable(state);
		while (name.find('.') != std::string::npos) {
			std::string member_name = name.substr(0, name.find_first_of('.'));
			name = name.substr(name.find_first_of('.') + 1);
			lua_getfield(state, -1, member_name.c_str());
			lua_remove(state, -2);
		}
		return 1;
	}

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

	template <>
	float LuaScript::GetTableElement<float>(const std::string& name) {
		PushVariable(name);
		float out = (float) lua_tonumber(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	void LuaScript::SetTableElement<float>(const std::string& name, const float& value) {
		PushContainingTable(name);
		lua_pushstring(state, LeafMember(name).c_str());
		lua_pushnumber(state, value);
		lua_settable(state, -3);
		lua_pop(state, 1);
	}

	template <>
	void LuaScript::PushValue<float>(const float& value) {
		lua_pushnumber(state, value);
	}

	template <>
	float LuaScript::PopValue<float>() {
		float out = (float) lua_tonumber(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	int LuaScript::GetTableElement<int>(const std::string& name) {
		PushVariable(name);
		float out = (int) lua_tointeger(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	void LuaScript::SetTableElement<int>(const std::string& name, const int& value) {
		PushContainingTable(name);
		lua_pushstring(state, LeafMember(name).c_str());
		lua_pushinteger(state, value);
		lua_settable(state, -3);
		lua_pop(state, 1);
	}

	template <>
	void LuaScript::PushValue<int>(const int& value) {
		lua_pushinteger(state, value);
	}

	template <>
	int LuaScript::PopValue<int>() {
		float out = (float) lua_tointeger(state, -1);
		lua_pop(state, 1);
		return out;
	}
}

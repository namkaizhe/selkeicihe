#include "LuaLibrary.h"
namespace Scripting {
	std::string LeafMember(const std::string& name) {
		if (name.find('.') == std::string::npos) return name;
		return name.substr(name.find_last_of('.') + 1);
	}

	int PushVariable(lua_State* state, const std::string& name) {
		PushContainingTable(state, name);
		std::string member_name = LeafMember(name);
		lua_getfield(state, -1, member_name.c_str());
		lua_remove(state, -2);
		return 1;
	}

	int PushContainingTable(lua_State* state, std::string name) {
		lua_pushglobaltable(state);
		while (name.find('.') != std::string::npos) {
			std::string member_name = name.substr(0, name.find_first_of('.'));
			name = name.substr(name.find_first_of('.') + 1);
			lua_getfield(state, -1, member_name.c_str());
			lua_remove(state, -2);
		}
		return 1;
	}

	template <>
	float GetTableElement<float>(lua_State* state, const std::string& name) {
		PushVariable(state, name);
		float out = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	void SetTableElement<float>(lua_State* state, const std::string& name, const float& value) {
		PushContainingTable(state, name);
		lua_pushstring(state, LeafMember(name).c_str());
		lua_pushnumber(state, value);
		lua_settable(state, -3);
		lua_pop(state, 1);
	}

	template <>
	void PushValue<float>(lua_State* state, const float& value) {
		lua_pushnumber(state, value);
	}

	template <>
	float PopValue<float>(lua_State* state) {
		float out = (float)lua_tonumber(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	int GetTableElement<int>(lua_State* state, const std::string& name) {
		PushVariable(state, name);
		float out = (int)lua_tointeger(state, -1);
		lua_pop(state, 1);
		return out;
	}

	template <>
	void SetTableElement<int>(lua_State* state, const std::string& name, const int& value) {
		PushContainingTable(state, name);
		lua_pushstring(state, LeafMember(name).c_str());
		lua_pushinteger(state, value);
		lua_settable(state, -3);
		lua_pop(state, 1);
	}

	template <>
	void PushValue<int>(lua_State* state, const int& value) {
		lua_pushinteger(state, value);
	}

	template <>
	int PopValue<int>(lua_State* state) {
		float out = (float)lua_tointeger(state, -1);
		lua_pop(state, 1);
		return out;
	}
}

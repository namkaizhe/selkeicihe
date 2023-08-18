#pragma once
#include <string>
#include <lua/lua.hpp>
namespace Scripting {
	
	int PushVariable(lua_State* state, const std::string& name);

	int PushContainingTable(lua_State* state, std::string name);

	template <typename T>
	T GetTableElement(lua_State* state, const std::string& name);

	template <typename T>
	void SetTableElement(lua_State* state, const std::string& name, const T& value);

	template <typename T>
	void PushValue(lua_State* state, const T& value);

	template <typename T>
	T PopValue(lua_State* state);

}
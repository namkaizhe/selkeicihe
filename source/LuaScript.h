#pragma once

#include <lua/lua.hpp>
#include <string>
#include <memory>
#include <vector>
#include "LuaLibrary.h"

namespace Scripting {

	class LuaScript;
	class BoundScript;

	/*
	* The base class for a binding between a variable in C++ and
	* a variable in Lua.
	*
	*/
	class ScriptVariableBase {
	protected:
		// The name of the variable in Lua.
		std::string name;
		// A pointer to the LuaScript object. 
		BoundScript* script;
	public:
		virtual void MirrorToScript() = 0;
		virtual void MirrorToEngine() = 0;

		ScriptVariableBase(BoundScript& script, const std::string& name) : script(&script), name(name) {}
	};

	template <typename T>
	class ScriptVariable : public ScriptVariableBase {
	private:
		T* variable;
	public:
		virtual void MirrorToScript() override;
		virtual void MirrorToEngine() override;
		T* Get() {
			return variable;
		};

		ScriptVariable(BoundScript& script, T& variable, const std::string& name) : ScriptVariableBase(script, name), variable(&variable)  {
		}
	};

	class LuaScript {
	private:
		std::string source;
		lua_State* state;

		int arguments;
		int returns;

	public:

		LuaScript(const std::string& file);
		~LuaScript();

		void Reload();

		template <typename T>
		void SetVariable(const std::string& name, const T& value) {
			SetTableElement(state, name, value);
		}

		template <typename T>
		T GetVariable(const std::string& name) {
			return GetTableElement<T>(state, name);
		}

		template <typename T>
		void PushArgument(T argument) {
			if (returns > 0) lua_pop(state, returns);
			returns = 0;
			PushValue(state, argument);
			++arguments;
		};

		virtual void CallFunction(const std::string& name, int returns = 0) {
			PushVariable(state, name);
			lua_rotate(state, - arguments - 1, 1);
			lua_pcall(state, arguments, returns, 0);
			arguments = 0;
		}

		template <typename T>
		T PopReturnValue() {
			if (returns == 0) throw std::exception;
			returns--;
			return PopValue<T>();
		}

	};

	class BoundScript : public LuaScript {
		std::vector<std::unique_ptr<ScriptVariableBase>> variables;

		void MirrorToScript() {
			for (std::unique_ptr<ScriptVariableBase>& variable : variables) {
				variable->MirrorToScript();
			}
		}

		void MirrorToEngine() {
			for (std::unique_ptr<ScriptVariableBase>& variable : variables) {
				variable->MirrorToEngine();
			}
		}
	public:

		virtual void CallFunction(const std::string& name, int returns = 0) {
			MirrorToScript();
			LuaScript::CallFunction(name, returns);
			MirrorToEngine();
		}

		template <typename T>
		void BindVariable(T& variable, std::string name) {
			variables.push_back(std::make_unique<ScriptVariable<T>>(*this, variable, name));
		}

		BoundScript(const std::string& file) : LuaScript(file) { }
	};

	template <typename T>
	void ScriptVariable<T>::MirrorToScript() {
		script->SetVariable(name, *variable);
	};
	template <typename T>
	void ScriptVariable<T>::MirrorToEngine() {
		*variable = script->GetVariable<T>(name);
	};
}
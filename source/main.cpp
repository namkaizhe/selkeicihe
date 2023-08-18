#include "LuaScript.h"
#include <iostream>

int main(void) {

	float num = 24;

	Scripting::BoundScript script{ "test.txt" };

	script.SetVariable("multiplier", 2);
	
	script.BindVariable(num, "num");
	script.PushArgument(1.4f);
	script.CallFunction("demo");
	std::cout << num;
}
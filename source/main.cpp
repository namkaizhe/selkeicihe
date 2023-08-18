#include "LuaScript.h"
#include <iostream>

int main(void) {

	int num = 1095;

	Scripting::BoundScript script{ "test.txt" };

	script.SetVariable("multiplier", 2);
	
	script.BindVariable(num, "num");
	script.PushArgument(19);
	script.CallFunction("demo");
	std::cout << num;
}
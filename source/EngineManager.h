#pragma once

#include "Manager.h"

class EngineManager : public Manager {
	// sole instance of manager
	static EngineManager instance;

public:
	// Initialize the manager. 
	// For the Engine Manager: call other managers' initialization functions.
	void Init() override; 

	void PreUpdate() override;

	void Update() override;
	
	void PostUpdate() override;

	void Cleanup() override;
};

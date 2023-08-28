#pragma once

class Manager {
public:
	// Initialize manager functions. 
	// e.g. Graphics manager should initialize GLFW, GLEW, etc.
	virtual void Init() = 0;
	// Called before Update.
	// e.g. cache game state, etc.
	virtual void PreUpdate() = 0;
	// Main update section.
	// e.g. process AI, reorder graphics objects, do physics calculations, etc.
	virtual void Update() = 0;
	// Called after Update.
	// e.g. render graphics objects to screen, etc.
	virtual void PostUpdate() = 0;
	// Clean up; delete/free all allocated memory, etc.
	virtual void Cleanup() = 0;
};

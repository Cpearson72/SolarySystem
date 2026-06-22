#pragma once
// settings
unsigned int SCR_WIDTH = 1200;
unsigned int SCR_HEIGHT = 850;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int cubemapTexture;
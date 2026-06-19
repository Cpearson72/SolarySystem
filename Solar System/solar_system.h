#pragma once

#include "Sphere.h"


// Solar System Body Declerations

Sphere sun((1.0f, 72, 36, true, 2));

struct Planets
{
	Sphere mercury{ 1.0f, 72, 36, true, 2 };
	Sphere venus{ 1.0f, 72, 36, true, 2 };
	Sphere earth{ 1.0f, 72, 36, true, 2 };
	Sphere mars{ 1.0f, 72, 36, true, 2 };
	Sphere jupiter{ 1.0f, 72, 36, true, 2 };
	Sphere saturn{ 1.0f, 72, 36, true, 2 };
	Sphere uranus{ 1.0f, 72, 36, true, 2 };
	Sphere neptune{ 1.0f, 72, 36, true, 2 };
};

struct Moons
{

};

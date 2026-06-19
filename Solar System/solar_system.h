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

void set_mass(Planets& solarSystem, double* masses)
{
	solarSystem.mercury.mass = masses[0];
	solarSystem.venus.mass = masses[1];
	solarSystem.earth.mass = masses[2];
	solarSystem.mars.mass = masses[3];
	solarSystem.jupiter.mass = masses[4];
	solarSystem.saturn.mass = masses[5];
	solarSystem.uranus.mass = masses[6];
	solarSystem.neptune.mass = masses[7];
}
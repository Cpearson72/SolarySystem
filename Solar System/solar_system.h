#pragma once

#include "Sphere.h"


// Solar System Body Declerations

struct Planets
{	
	Sphere sun{ 10.92, 72, 36, true, 2 };
	Sphere mercury{ 0.0383, 72, 36, true, 2 };
	Sphere venus{ 0.0949, 72, 36, true, 2 };
	Sphere earth{ 0.1, 72, 36, true, 2 };
	Sphere mars{ 0.0532, 72, 36, true, 2 };
	Sphere jupiter{ 1.1209, 72, 36, true, 2 };
	Sphere saturn{ 0.9449, 72, 36, true, 2 };
	Sphere uranus{ 0.4007, 72, 36, true, 2 };
	Sphere neptune{ 0.3883, 72, 36, true, 2 };
	Sphere pluto{ 0.01868, 72, 36, true, 2 };


	// Add a member function to easily get them as a vector
	std::vector<Sphere> to_vector() const {
		return { sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto };
	}
};

struct Moons
{

};

void set_mass(Planets& solarSystem)
{
	solarSystem.sun.mass = 1989100.0;
	solarSystem.mercury.mass = 0.33;
	solarSystem.venus.mass = 4.87;
	solarSystem.earth.mass = 5.97;
	solarSystem.mars.mass = 0.64;
	solarSystem.jupiter.mass = 1898.60;
	solarSystem.saturn.mass = 568.46;
	solarSystem.uranus.mass = 86.62;
	solarSystem.neptune.mass = 102.43;
	solarSystem.pluto.mass = 0.01;
}

int distance_scale = 20;

void set_position(Planets& solarSystem)
{
	solarSystem.sun.position = glm::vec3(0.0 * distance_scale, -2.0f, -10.0f);
	solarSystem.mercury.position = glm::vec3(0.39 * distance_scale, -2.0f, -10.0f);
	solarSystem.venus.position = glm::vec3(0.72 * distance_scale, -2.0f, -10.0f);
	solarSystem.earth.position = glm::vec3(1.0 * distance_scale, -2.0f, -10.0f);
	solarSystem.mars.position = glm::vec3(1.52 * distance_scale, -2.0f, -10.0f);
	solarSystem.jupiter.position = glm::vec3(5.2f * distance_scale, -2.0f, -10.0f);
	solarSystem.saturn.position = glm::vec3(9.54f * distance_scale, -2.0f, -10.0f);
	solarSystem.uranus.position = glm::vec3(19.20f * distance_scale, -2.0f, -10.0f);
	solarSystem.neptune.position = glm::vec3(30.06f * distance_scale, -2.0f, -10.0f);
	solarSystem.pluto.position = glm::vec3(39.48f * distance_scale, -2.0f, -10.0f);
}

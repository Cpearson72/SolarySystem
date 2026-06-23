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
	std::vector<Sphere> planet_to_vector() const {
		return { mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto };
	}

	void from_planet_vector(const std::vector<Sphere>& planets)
	{
		mercury = planets[0];
		venus = planets[1];
		earth = planets[2];
		mars = planets[3];
		jupiter = planets[4];
		saturn = planets[5];
		uranus = planets[6];
		neptune = planets[7];
		pluto = planets[8];
	}
	void from_vector(const std::vector<Sphere>& planets)
	{
		sun = planets[0];
		mercury = planets[1];
		venus = planets[2];
		earth = planets[3];
		mars = planets[4];
		jupiter = planets[5];
		saturn = planets[6];
		uranus = planets[7];
		neptune = planets[8];
		pluto = planets[9];
	}



};

struct Moons
{

};


void set_mass(Planets& solarSystem)
{
	solarSystem.sun.mass = 1.9891e30; // 1989100.0 * 1e24
	solarSystem.mercury.mass = 0.33e24;
	solarSystem.venus.mass = 4.87e24;
	solarSystem.earth.mass = 5.97e24;
	solarSystem.mars.mass = 0.64e24;
	solarSystem.jupiter.mass = 1898.60e24;
	solarSystem.saturn.mass = 568.46e24;
	solarSystem.uranus.mass = 86.62e24;
	solarSystem.neptune.mass = 102.43e24;
	solarSystem.pluto.mass = 0.01e24;
}

int distance_scale = 5;

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

constexpr double AU = 1.496e11;

void calc_initial_orbital_velocities(Planets& solarSystem) {
	std::vector<Sphere> planetVector = solarSystem.planet_to_vector();
	for (auto& planet : planetVector) {
		// AU scale to meters for gravity calculations
		// -------------------------------------------
		double radius_AU = glm::distance(solarSystem.sun.position, planet.position);
		double r_meter = radius_AU * AU;

		glm::vec3 gravityDir = glm::normalize(solarSystem.sun.position - planet.position);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 velocityDir = glm::normalize(glm::cross(gravityDir, up));

		float orbitalSpeed = std::sqrt((6.6743e-11f * solarSystem.sun.mass) / r_meter);
		float orbitalSpeed_world = orbitalSpeed / AU;
		planet.velocity = velocityDir * orbitalSpeed_world;
	}
	solarSystem.from_planet_vector(planetVector);
}

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};


unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};


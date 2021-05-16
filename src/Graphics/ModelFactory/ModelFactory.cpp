#include "pch.h"
#include "ModelFactory.h"

std::unique_ptr<IModel> ModelFactory::create(unsigned int index)
{
	switch (index)
	{
	case 0:
		return std::make_unique<Quad>();
	case 1:
		return std::make_unique<Triangle>();
	case 2:
		return std::make_unique<Circle>();
	case 3:
		return std::make_unique<Lines>();
	case 4:
		return std::make_unique<LightQuad>();
	case 5:
		return std::make_unique<Sphere>();
	case 6:
	  return std::make_unique<Plane_>();
	case 7:
	  return std::make_unique<Cube>();
	case 8:
	  return std::make_unique<Cylinder>();
	case 9:
	  return std::make_unique<Cone>();
	case 10:
	  return std::make_unique<Torus>();
	case 11:
	  return std::make_unique<Pyramid>();
	default:
		throw std::string("Invalid Model!");
	}
}

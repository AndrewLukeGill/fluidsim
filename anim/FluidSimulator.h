#pragma once
#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Particle.h"
#include "GlobalResourceManager.h"

#include <string>

class FluidSimulator : public BaseSimulator
{
public:

	FluidSimulator(const std::string& name, BaseSystem* target);
	~FluidSimulator();

	int step(double time);
	int init(double time)
	{
		val[0] = time;
		m_object->setState(val);
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv);

protected:
	double val[1];

	BaseSystem* m_object;
	Particle* particle;

};
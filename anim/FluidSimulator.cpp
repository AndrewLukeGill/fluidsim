#include "fluidSimulator.h"

FluidSimulator::FluidSimulator(const std::string& name, BaseSystem* target) :
	BaseSimulator(name),
	m_object(target)
{
	val[0] = 0;
	particle = NULL;
}

FluidSimulator::~FluidSimulator()
{
}

int FluidSimulator::step(double time)
{
	val[0] = time;
	m_object->setState(val);

	return 0;

}

int FluidSimulator::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0) {
		if (argc == 2) {
			particle = static_cast<Particle*>(GlobalResourceManager::use()->getSystem(argv[1]));
			if (particle) {
			}
			else {
				animTcl::OutputMessage("System Not Found!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> link <sys name>");
		}
	}
	else if (strcmp(argv[0], "fix") == 0) {
		if (argc == 2) {
			if (particle) {
				particle->fix(std::stoi(argv[1]));
			}
			else {
				animTcl::OutputMessage("System Not Initialized!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: system <name> dim <num of particles>");
		}
	}
	else if (strcmp(argv[0], "integration") == 0) {
		if (argc == 3) {
			if (particle) {
				if (strcmp(argv[1], "euler") == 0) {
					particle->setIntegration(0);
					GlobalResourceManager::use()->setTimestep(std::stod(argv[2]));
				}
				else if (strcmp(argv[1], "symplectic") == 0) {
					particle->setIntegration(1);
					GlobalResourceManager::use()->setTimestep(std::stod(argv[2]));
				}
				else if (strcmp(argv[1], "verlet") == 0) {
					particle->setIntegration(2);
					GlobalResourceManager::use()->setTimestep(std::stod(argv[2]));
				}
				else {
					animTcl::OutputMessage("Usage: simulator <sim_name> integration <euler|symplectic|verlet> <time step>");
				}
			}
			else {
				animTcl::OutputMessage("System Not Initialized!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> integration <euler|symplectic|verlet> <time step>");
		}
	}
	else if (strcmp(argv[0], "ground") == 0) {
		if (argc == 3) {
			if (particle) {
				particle->setGround(std::stod(argv[1]), std::stod(argv[2]));
			}
			else {
				animTcl::OutputMessage("System Not Initialized!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> ground <ks> <kd>");
		}
	}
	else if (strcmp(argv[0], "gravity") == 0) {
		if (argc == 2) {
			if (particle) {
				particle->setGravity(std::stod(argv[1]));
			}
			else {
				animTcl::OutputMessage("System Not Initialized!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> gravity <g>");
		}
	}
	else if (strcmp(argv[0], "drag") == 0) {
		if (argc == 2) {
			if (particle) {
				particle->setDrag(std::stod(argv[1]));
			}
			else {
				animTcl::OutputMessage("System Not Initialized!");
			}
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> drag <kdrag>");
		}
	}
	glutPostRedisplay();
	return TCL_OK;
}

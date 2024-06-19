#pragma once
#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"
#include <sstream>
#include <random>

class Particle : public BaseSystem
{
	// linked list, hold data for various points
	struct points {
		points(double x, double y, double z, double vx, double vy, double vz, double mass, double radius, bool nailed, bool render);
		
		points* next;

		double x;
		double y;
		double z;

		// previous position, needed for verlet
		double x_p;
		double y_p;
		double z_p;

		// next position, allows positions to all be updated simulatenously 
		double x_n;
		double y_n;
		double z_n;

		// initial position, for reset
		double x_0;
		double y_0;
		double z_0;

		// velocity 
		double vx;
		double vy;
		double vz;

		double mass;

		double radius;

		// nailed = dont move, render prevents null particles from being rendered and causing crashes
		bool nailed;
		bool render;
	};

	// linked list, hold bounding walls
	struct wall {
		wall(double x, double y, double z, double norm_x, double norm_y, double norm_z, double wall_ks, double wall_kd);

		wall* next;

		double x;
		double y;
		double z;

		double norm_x;
		double norm_y;
		double norm_z;

		double wall_ks;
		double wall_kd;
	};

public:
	Particle(const std::string& name);
	virtual void getState(double* p);
	virtual void setState(double* p);
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void setDim(int num_particles);

	void setParticle(int index, double x, double y, double z, double vx, double vy, double vz, double mass, double radius);

	void addWall(double x, double y, double z, double norm_x, double norm_y, double norm_z, double wall_ks, double norm_kd);

	void append(double x, double y, double z, double vx, double vy, double yz, double mass, double radius, bool nailed, bool render);

	void genPoints(double max_x, double min_x, double max_y, double min_y, double max_z, double min_z, int num_points, bool zero_velocity);

	void setVelocity(double vx, double vy, double vz);
	void fix(int index);

	void findWallForce(Vector fextern);
	void Particle::findCollision(Vector fcollision);
	void findForce(Vector fnet, Vector fextern, Vector fcollision);

	void forwardEuler(double time);
	void symplectic(double time);
	void verlet(double time);

	void setIntegration(int num);
	void setGround(double ks, double kd);
	void setGravity(double g);
	void setDrag(double drag);

	int command(int argc, myCONST_SPEC char** argv);

protected:
	points* headPoint;
	points* currPoint;

	//std::vector<std::vector<points>> buckets;

	wall* headWall;
	wall* currWall;

	double lastTime;
	double kdrag;

	double gravity;
	double k_s;
	double k_d;

	int integration;
};


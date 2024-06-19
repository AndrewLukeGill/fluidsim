#include "Particle.h"

// constructor for particles, default everything to 0
Particle::points::points(double x = 0, double y = 0, double z = 0, double vx = 0, double vy = 0, double vz = 0, double mass = 0, double radius = 0, bool nailed = false, bool render = false) {
	this->x = x;
	this->y = y;
	this->z = z;

	this->x_p = x;
	this->y_p = y;
	this->z_p = z;

	this->x_n = x;
	this->y_n = y;
	this->z_n = z;

	this->x_0 = x;
	this->y_0 = y;
	this->z_0 = z;

	this->vx = vx;
	this->vy = vy;
	this->vz = vz;

	this->mass = mass;
	this->radius = radius; 

	this->nailed = nailed;
	this->render = render;

	this->next = NULL;
}

// contructor for walls
Particle::wall::wall(double x, double y, double z, double norm_x, double norm_y, double norm_z, double wall_ks, double wall_kd) {
	this->next = NULL;

	this->x = x;
	this->y = y;
	this->z = z;

	this->norm_x = norm_x;
	this->norm_y = norm_y;
	this->norm_z = norm_z;

	this->wall_ks = wall_ks;
	this->wall_kd = wall_kd;
}

// contructor for the particle class, extends base system
Particle::Particle(const std::string& name) : BaseSystem(name) {
	headPoint = NULL;
	currPoint = NULL;

	headWall = new wall(0, 1, 0, 0.32535625036333, 0.970142500145332, 0, 5000, 300);
	//headWall = new wall(0, 2, 0, 0.7, 0.8, 0, 5000, 300);
	//headWall = new wall(0, 0.5, 0, 0, 1, 0, 5000, 300);
	currWall = NULL;

	lastTime = 0;
	kdrag = 0.05;
	gravity = -10;

	k_s = 5000;
	k_d = 300;

	integration = 0;
}

void Particle::setDim(int num) {
	if (num > 0) {
		headPoint = new points();
		currPoint = headPoint;
		
		for (int i = 1; i < num; i++) {
			currPoint->next = new points();
			currPoint = currPoint->next;
		}
	}
	else {
		animTcl::OutputMessage("Number of Particles Too Low!");
	}
}

// sets the location, mass, and velocity of the partical at the given index
void Particle::setParticle(int index, double x, double y, double z, double vx, double vy, double vz, double mass, double radius = 0.2) {
	if (index < 0) {
		animTcl::OutputMessage("Invalid Index!");
	}
	else {
		currPoint = headPoint;
		for (int i = 0; i < index; i++) {
			if (currPoint->next) {
				currPoint = currPoint->next;
			}
			else {
				animTcl::OutputMessage("Exceeded List!");
				return;
			}
		}
		currPoint->x = x;
		currPoint->y = y;
		currPoint->z = z;

		currPoint->x_p = x;
		currPoint->y_p = y;
		currPoint->z_p = z;
		
		currPoint->x_0 = x;
		currPoint->y_0 = y;
		currPoint->z_0 = z;

		currPoint->vx = vx;
		currPoint->vy = vy;
		currPoint->vz = vz;

		currPoint->mass = mass;
		currPoint->radius = radius;

		currPoint->render = true;

		animTcl::OutputMessage("Particle Set");
	}
}

void Particle::genPoints(double max_x, double min_x, double max_y, double min_y, double max_z, double min_z, int num_points, bool zero_velocity) {
	setDim(num_points);

	double x_pos;
	double y_pos;
	double z_pos;

	double v_x;
	double v_y;
	double v_z;

	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_real_distribution<double> x_dist(min_x, max_x);
	std::uniform_real_distribution<double> y_dist(min_y, max_y);
	std::uniform_real_distribution<double> z_dist(min_z, max_z);

	std::uniform_real_distribution<double> x_vel_dist(2 * min_x, 2 * max_x);
	std::uniform_real_distribution<double> y_vel_dist(2 * min_y, 2 * max_y);
	std::uniform_real_distribution<double> z_vel_dist(2 * min_z, 2 * max_z);

	for (int i = 0; i < num_points; i++) {
		x_pos = x_dist(mt);
		y_pos = y_dist(mt);
		z_pos = z_dist(mt);

		if (zero_velocity) {
			v_x = 0;
			v_y = 0;
			v_z = 0;
		}
		else {
			v_x = x_vel_dist(mt);
			v_y = y_vel_dist(mt);
			v_z = z_vel_dist(mt);
		}
		setParticle(i, x_pos, y_pos, z_pos, v_x, v_y, v_z, 5);
	}
}

void Particle::append(double x, double y, double z, double vx, double vy, double vz, double mass, double radius, bool nailed, bool render) {
	currPoint = headPoint;
	while (currPoint->next) {
		currPoint = currPoint->next;
	}
	currPoint->next = new points(x, y, z, vx, vy, vz, mass, radius, nailed, render);
}

void Particle::addWall(double x, double y, double z, double norm_x, double norm_y, double norm_z, double wall_ks, double wall_kd) {
	currWall = headWall;
	while (currWall->next) {
		currWall = currWall->next;
	}
	currWall->next = new wall(x, y, z, norm_x, norm_y, norm_z, wall_ks, wall_kd);
	animTcl::OutputMessage("Wall Set");
}

// sets the velocity of all particles. Doesnt seem to work well with verlet
void Particle::setVelocity(double vx, double vy, double vz) {
	currPoint = headPoint;
	while (currPoint) {
		currPoint->vx = vx;
		currPoint->vy = vy;
		currPoint->vz = vz;
		currPoint = currPoint->next;
	}
	animTcl::OutputMessage("Updated Velocity");
}

// fixes a the particle at the given index in place
void Particle::fix(int index) {
	if (index < 0) {
		animTcl::OutputMessage("Invalid Index!");
	}
	else {
		currPoint = headPoint;
		for (int i = 0; i < index; i++) {
			if (currPoint->next) {
				currPoint = currPoint->next;
			}
			else {
				animTcl::OutputMessage("Exceeded List!");
				return;
			}
		}
		currPoint->nailed = !currPoint->nailed;
		animTcl::OutputMessage("Point Fixed");
	}
}

void Particle::findWallForce(Vector fextern) {
	zeroVector(fextern);
	Vector N;
	Vector P;
	Vector x;
	Vector v;
	Vector result;
	Vector springForce;
	Vector dampingForce;
	Vector sumForce;

	currWall = headWall;
	while (currWall) {
		setVector(N, currWall->norm_x, currWall->norm_y, currWall->norm_z);
		setVector(P, currWall->x, currWall->y, currWall->z);
		setVector(x, currPoint->x, currPoint->y, currPoint->z);
		setVector(v, currPoint->vx, currPoint->vy, currPoint->vz);

		VecSubtract(result, x, P);
		if (VecDotProd(result, N) < 0) {
			VecCopy(springForce, N);
			VecScale(springForce, -1 * currWall->wall_ks * VecDotProd(result, N));

			VecCopy(dampingForce, N);
			VecScale(dampingForce, -1 * currWall->wall_kd * VecDotProd(v, N));

			VecAdd(sumForce, springForce, dampingForce);
			VecAdd(fextern, fextern, sumForce);
		}
		currWall = currWall->next;
	}
}

void Particle::findCollision(Vector fcollision) {
	zeroVector(fcollision);
	Vector N;
	Vector P;
	Vector x;
	Vector v;
	Vector result;
	Vector springForce;
	Vector dampingForce;
	Vector sumForce;

	points* temp = headPoint;
	while (temp) {
		if (temp->render) {
			if (temp != currPoint) {
				setVector(N, temp->x - currPoint->x, temp->y - currPoint->y, temp->z - currPoint->z);
				VecNormalize(N);

				setVector(P, temp->x, temp->y, temp->z);
				setVector(x, currPoint->x, currPoint->y, currPoint->z);
				setVector(v, temp->vx - currPoint->vx, temp->vy - currPoint->vy, temp->vz - currPoint->vz);

				VecSubtract(result, x, P);
				double len = VecDotProd(result, N);
				if (len > -1 * currPoint->radius) {

					VecCopy(springForce, N);
					VecScale(springForce, 1 * 1000 * len);

					VecCopy(dampingForce, N);
					VecScale(dampingForce, 1 * 50 * VecDotProd(v, N));

					VecAdd(sumForce, springForce, dampingForce);
					VecAdd(fcollision, fcollision, sumForce);
				}
			}
		}
		temp = temp->next;
	}
}

// finds the sum of the spring, external, and gravity forces acting on the particle i
void Particle::findForce(Vector fnet, Vector fextern, Vector fcollision) {
	zeroVector(fnet);
	findWallForce(fextern);
	findCollision(fcollision);

	fnet[0] = (-1 * kdrag * currPoint->vx) + fextern[0] + fcollision[0];
	fnet[1] = (-1 * kdrag * currPoint->vy) + (currPoint->mass * gravity) + fextern[1] + fcollision[1];
	fnet[2] = (-1 * kdrag * currPoint->vz) + fextern[2] + fcollision[2];
}


// simplest integration method
void Particle::forwardEuler(double time) {
	Vector fnet, fextern, fcollision;
	
	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed && currPoint->render) {
			findForce(fnet, fextern, fcollision);

			currPoint->x_n = currPoint->x + ((time - lastTime) * currPoint->vx);
			currPoint->y_n = currPoint->y + ((time - lastTime) * currPoint->vy);
			currPoint->z_n = currPoint->z + ((time - lastTime) * currPoint->vz);

			currPoint->vx = currPoint->vx + (((time - lastTime) * fnet[0]) / currPoint->mass);
			currPoint->vy = currPoint->vy + (((time - lastTime) * fnet[1]) / currPoint->mass);
			currPoint->vz = currPoint->vz + (((time - lastTime) * fnet[2]) / currPoint->mass);
		}
		currPoint = currPoint->next;
	}

	// update all positions at once. If done one at a time in the above loop causes drifting
	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed) {
			currPoint->x = currPoint->x_n;
			currPoint->y = currPoint->y_n;
			currPoint->z = currPoint->z_n;
		}
		currPoint = currPoint->next;
	}

	lastTime = time;
}

// symplectic euler integration. Essentially the same as the above forward euler, but finds the new velocities before new positions 
void Particle::symplectic(double time) {
	Vector fnet, fextern, fcollision;

	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed) {
			findForce(fnet, fextern, fcollision);

			currPoint->vx = currPoint->vx + (((time - lastTime) * fnet[0]) / currPoint->mass);
			currPoint->vy = currPoint->vy + (((time - lastTime) * fnet[1]) / currPoint->mass);
			currPoint->vz = currPoint->vz + (((time - lastTime) * fnet[2]) / currPoint->mass);

			currPoint->x_n = currPoint->x + ((time - lastTime) * currPoint->vx);
			currPoint->y_n = currPoint->y + ((time - lastTime) * currPoint->vy);
			currPoint->z_n = currPoint->z + ((time - lastTime) * currPoint->vz);
		}
		currPoint = currPoint->next;
	}

	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed) {
			currPoint->x = currPoint->x_n;
			currPoint->y = currPoint->y_n;
			currPoint->z = currPoint->z_n;
		}
		currPoint = currPoint->next;
	}

	lastTime = time;
}

// more complext verlet integration
void Particle::verlet(double time) {
	Vector fnet, fextern, fcollision;

	if (time == 0) {
		// update current positions so verlet integration can be calculated in next step, also avoid divide by zero crash from t = 0
		currPoint = headPoint;
		while (currPoint)
		{
			currPoint->x = currPoint->x - (0.01 * currPoint->vx);
			currPoint->y = currPoint->y - (0.01 * currPoint->vy);
			currPoint->z = currPoint->z - (0.01 * currPoint->vz);

			currPoint = currPoint->next;
		}
		return;
	}

	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed) {
			findForce(fnet, fextern, fcollision);

			currPoint->x_n = (2 * currPoint->x) - currPoint->x_p + (pow(time - lastTime, 2) * fnet[0] / currPoint->mass);
			currPoint->y_n = (2 * currPoint->y) - currPoint->y_p + (pow(time - lastTime, 2) * fnet[1] / currPoint->mass);
			currPoint->z_n = (2 * currPoint->z) - currPoint->z_p + (pow(time - lastTime, 2) * fnet[2] / currPoint->mass);

			currPoint->vx = (currPoint->x_n - currPoint->x_p) / (2 * (time - lastTime));
			currPoint->vy = (currPoint->y_n - currPoint->y_p) / (2 * (time - lastTime));
			currPoint->vz = (currPoint->z_n - currPoint->z_p) / (2 * (time - lastTime));

		}
		currPoint = currPoint->next;
	}

	// update previous position at the same time as next position
	currPoint = headPoint;
	while (currPoint) {
		if (!currPoint->nailed) {
			currPoint->x_p = currPoint->x;
			currPoint->y_p = currPoint->y;
			currPoint->z_p = currPoint->z;

			currPoint->x = currPoint->x_n;
			currPoint->y = currPoint->y_n;
			currPoint->z = currPoint->z_n;
		}
		currPoint = currPoint->next;
	}

	lastTime = time;
}

// declare which integration method to use
void Particle::setIntegration(int num) {
	integration = num;
}

// sets ground forces
void Particle::setGround(double ks, double kd) {
	k_s = ks;
	k_d = kd;
}

// set gravity acceleration
void Particle::setGravity(double g) {
	gravity = g;
}

// set global drag
void Particle::setDrag(double drag) {
	if (drag > 0) {
		kdrag = drag;
	}
}

// unused method inherited from base system
void Particle::getState(double* p) {

}

// called every tick by simulator. P contains total runtime
void Particle::setState(double* p) {
	if (integration == 0) {
		forwardEuler(p[0]);
	}
	else if (integration == 1) {
		symplectic(p[0]);
	}
	else if (integration == 2) {
		verlet(p[0]);
	}
}

void Particle::display(GLenum mode) {
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_COLOR_MATERIAL);

	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);

	// draw ground surface
	glVertex3f(10, 0, 10);
	glVertex3f(-10, 0, 10);
	glVertex3f(-10, 0, -10);
	glVertex3f(10, 0, -10);
	glEnd();

	// draw particles
	glColor3f(0, 0, 0);
	glPointSize(30.0);
	glBegin(GL_POINTS);
	currPoint = headPoint;
	int flip = 0;
	while(currPoint){
		if (currPoint->render) {
			if (flip == 0) {
				flip = 1;
				glColor3f(1, 1, 1);
			}
			else {
				flip = 0;
				glColor3f(0, 0, 0);
			}
			glVertex3d(currPoint->x, currPoint->y, currPoint->z);
		}
		currPoint = currPoint->next;
	}
	glEnd();
	glPopAttrib();
}

// allows console interaction, weakly protected against bad commands
int Particle::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "dim") == 0) {
		if (argc == 2) {
			setDim(std::stoi(argv[1]));
		}
		else {
			animTcl::OutputMessage("Usage: system <name> dim <num of particles>");
		}
	}
	else if (strcmp(argv[0], "particle") == 0) {
		if (argc == 9) {
			setParticle(std::stoi(argv[1]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]), std::stod(argv[2]));
		}
		else {
			animTcl::OutputMessage("Usage: system <sys_name> particle <index> <mass> <x y z vx vy vz>");
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0) {
		if (argc == 4) {
			setVelocity(std::stoi(argv[1]), std::stod(argv[2]), std::stod(argv[3]));
		}
		else {
			animTcl::OutputMessage("Usage: system <sys_name> all_velocities  <vx vy vz>");
		}
	}
	else if (strcmp(argv[0], "wall") == 0) {
		if (argc == 9) {
			addWall(std::stoi(argv[1]), std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]));
		}
		else {
			animTcl::OutputMessage("Usage: system <sys_name> wall <x y z> <nx ny nz> <ks kd>");
		}
	}
	else if (strcmp(argv[0], "gen") == 0) {
		if (argc == 9) {
			genPoints(std::stoi(argv[1]), std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]));
		}
		else {
			animTcl::OutputMessage("Usage: system <sys_name> gen <xmax xmin ymax ymin zmax zmin> <num> <0/1 rand velocity>");
		}
	}
	else if (strcmp(argv[0], "new") == 0) {
		if (argc == 11) {
			append(std::stoi(argv[1]), std::stod(argv[2]), std::stod(argv[3]), std::stod(argv[4]), std::stod(argv[5]), std::stod(argv[6]), std::stod(argv[7]), std::stod(argv[8]), std::stod(argv[9]), std::stod(argv[10]));
		}
		else {
			animTcl::OutputMessage("Usage: system <sys_name> new <x y z> <vx vy vz> <mass> <radius> <nailed> <render>");
		}
	}

	glutPostRedisplay();
	return TCL_OK;
}

// reset sim state
void Particle::reset(double time) {
	lastTime = 0;
	currPoint = headPoint;
	while (currPoint) {
		currPoint->x = currPoint->x_0;
		currPoint->y = currPoint->y_0;
		currPoint->z = currPoint->z_0;
		currPoint = currPoint->next;
	}
}
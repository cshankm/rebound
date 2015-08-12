/**
 * Overstability in Saturn Rings
 * 
 * A narrow box of Saturn's rings is simulated to study the viscous 
 * overstability. Collisions are resolved using the plane-sweep method.
 *
 * It takes about 30 orbits for the overstability to occur. You can 
 * speed up the calculation by turning off the visualization. Just press
 * `d` while the simulation is running. Press `d` again to turn it back on.
 *
 * You can change the viewing angle of the camera with your mouse or by pressing
 * the `r` key. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rebound.h"

extern double OMEGA;
extern double OMEGAZ;

double coefficient_of_restitution(const struct reb_simulation*r, double v){
	return 0.5;
}

int main(int argc, char* argv[]){
	struct reb_simulation* r = reb_create_simulation();
	// Setup constants
	r->ri_sei.OMEGA 		= 1.;
	r->ri_sei.OMEGAZ 		= 3.6;
	r->dt				= 2e-3*2.*M_PI;
	double particle_r 		= 1;
	double tau			= 1.64;
	r->coefficient_of_restitution 	= coefficient_of_restitution;
	r->integrator			= REB_INTEGRATOR_SEI;
	r->collision			= REB_COLLISION_TREE;
	r->gravity			= REB_GRAVITY_NONE;
	r->boundary			= REB_BOUNDARY_SHEAR;

	reb_configure_box(r,1.,200,5,20);
	r->nghostx = 1; 	r->nghosty = 1; 	r->nghostz = 0;

	// Initial conditions
	double _N = tau * r->boxsize.x * r->boxsize.y/(M_PI*particle_r *particle_r);
	while (r->N<_N){
		struct reb_particle p;
		p.x 	= ((double)rand()/(double)RAND_MAX-0.5)*r->boxsize.x;
		p.y 	= ((double)rand()/(double)RAND_MAX-0.5)*r->boxsize.y;
		p.z 	= 10.0*((double)rand()/(double)RAND_MAX-0.5)*particle_r;
		p.vx 	= 0;
		p.vy 	= -1.5*p.x; // shear
		p.vz 	= 0;
		p.ax 	= 0; p.ay 	= 0; p.az 	= 0;
		p.m 	= 1.;
		p.r 	= particle_r;
		reb_add(r, p);
	}
	reb_integrate(r,INFINITY);
}

void heartbeat(struct reb_simulation* r){
	if (reb_output_check(r,2.*M_PI)){
		reb_output_timing(r,0);
	}
}

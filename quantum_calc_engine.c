/**
 * @file quantum_calc_engine.c
 * @brief High-Performance Simulation & Compute Node - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_PARTICLES 1000

// Particle structure for physics and simulation calculations
typedef struct {
    double x, y, z;
    double vx, vy, vz;
    double mass;
} Particle;

// Initialize a basic physics or simulation state
int initialize_simulation_space(Particle *particles, int count) {
    if (!particles || count <= 0 || count > MAX_PARTICLES) return 0;
    
    for (int i = 0; i < count; i++) {
        particles[i].x = (double)(i * 1.5);
        particles[i].y = (double)(i * 2.0);
        particles[i].z = (double)(i * 0.5);
        particles[i].vx = 0.1 * i;
        particles[i].vy = 0.2 * i;
        particles[i].vz = 0.0;
        particles[i].mass = 1.0 + (i * 0.1);
    }
    return 1;
}

// Run a lightweight, high-speed physics / vector calculation step
void step_simulation(Particle *particles, int count, double dt) {
    for (int i = 0; i < count; i++) {
        // Apply basic Newtonian velocity and acceleration updates
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
        particles[i].z += particles[i].vz * dt;
    }
}

int main(int argc, char *argv[]) {
    int particle_count = 100;
    if (argc > 1) {
        particle_count = atoi(argv[1]);
        if (particle_count <= 0 || particle_count > MAX_PARTICLES) {
            particle_count = MAX_PARTICLES;
        }
    }

    Particle *space = (Particle *)malloc(particle_count * sizeof(Particle));
    if (!space) {
        fprintf(stderr, "Memory allocation failed for simulation space.\n");
        return 1;
    }

    if (initialize_simulation_space(space, particle_count)) {
        printf("Simulation space initialized with %d particles.\n", particle_count);
        
        // Execute 10 simulation steps
        for (int step = 1; step <= 10; step++) {
            step_simulation(space, particle_count, 0.016); // ~60fps delta time
        }
        printf("Simulation step execution completed successfully (Pure C Engine).\n");
    }

    free(space);
    return 0;
}

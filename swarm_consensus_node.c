/**
 * @file swarm_consensus_node.c
 * @brief Decentralized Swarm Intelligence & Node Synchronization Engine - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SWARM_NODES 64

// Structure representing an individual worker node in the swarm network
typedef struct {
    int node_id;
    double local_state_vector;
    int is_active;
} SwarmNode;

// Initialize the swarm network topology
int initialize_swarm_network(SwarmNode *swarm, int count) {
    if (!swarm || count <= 0 || count > MAX_SWARM_NODES) return 0;

    for (int i = 0; i < count; i++) {
        swarm[i].node_id = i + 1;
        swarm[i].local_state_vector = 10.0 + (i * 2.5); // Initial decentralized states
        swarm[i].is_active = 1;
    }
    return 1;
}

// Execute a decentralized consensus step (averaging neighbor states toward alignment)
double compute_swarm_consensus(SwarmNode *swarm, int count) {
    if (!swarm || count <= 0) return 0.0;

    double sum = 0.0;
    int active_count = 0;

    for (int i = 0; i < count; i++) {
        if (swarm[i].is_active) {
            sum += swarm[i].local_state_vector;
            active_count++;
        }
    }

    if (active_count == 0) return 0.0;
    double global_mean = sum / active_count;

    // Convergence step: pull each node slightly toward the global consensus
    for (int i = 0; i < count; i++) {
        if (swarm[i].is_active) {
            swarm[i].local_state_vector += 0.1 * (global_mean - swarm[i].local_state_vector);
        }
    }

    return global_mean;
}

int main(int argc, char *argv[]) {
    printf("Initializing Swarm Intelligence & Synchronization Engine (Domain 5)...\n");

    int node_count = 8;
    if (argc > 1) {
        node_count = atoi(argv[1]);
        if (node_count <= 0 || node_count > MAX_SWARM_NODES) {
            node_count = MAX_SWARM_NODES;
        }
    }

    SwarmNode *swarm = (SwarmNode *)malloc(node_count * sizeof(SwarmNode));
    if (!swarm) {
        fprintf(stderr, "Memory allocation failed for swarm network.\n");
        return 1;
    }

    if (initialize_swarm_network(swarm, node_count)) {
        printf("Swarm network initialized with %d active nodes.\n", node_count);

        // Run 5 consensus iteration cycles
        for (int cycle = 1; cycle <= 5; cycle++) {
            double consensus = compute_swarm_consensus(swarm, node_count);
            printf("Consensus Cycle %d -> Global Mean State Vector: %.4f\n", cycle, consensus);
        }
        printf("Swarm consensus convergence completed successfully (Pure C Engine).\n");
    }

    free(swarm);
    return 0;
}

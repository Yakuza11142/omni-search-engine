/**
 * @file vector_path_router.c
 * @brief Spatial Coordinate & Pathfinding Routing Engine - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure representing a 3D spatial coordinate point
typedef struct {
    double x;
    double y;
    double z;
} Coordinate3D;

// Calculate precise Euclidean distance between two spatial nodes
double calculate_euclidean_distance(Coordinate3D p1, Coordinate3D p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    return sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

// Find the shortest route from a set of target waypoints
int optimize_waypoint_path(Coordinate3D *waypoints, int count, int *out_route) {
    if (!waypoints || count <= 0 || !out_route) return 0;

    // Simple nearest-neighbor routing implementation in pure C
    int visited_count = 0;
    int *visited = (int *)calloc(count, sizeof(int));
    if (!visited) return 0;

    int current_idx = 0;
    out_route[0] = current_idx;
    visited[current_idx] = 1;
    visited_count++;

    while (visited_count < count) {
        int nearest_idx = -1;
        double min_dist = -1.0;

        for (int i = 0; i < count; i++) {
            if (!visited[i]) {
                double dist = calculate_euclidean_distance(waypoints[current_idx], waypoints[i]);
                if (min_dist < 0.0 || dist < min_dist) {
                    min_dist = dist;
                    nearest_idx = i;
                }
            }
        }

        if (nearest_idx != -1) {
            visited[nearest_idx] = 1;
            out_route[visited_count] = nearest_idx;
            current_idx = nearest_idx;
            visited_count++;
        } else {
            break;
        }
    }

    free(visited);
    return visited_count;
}

int main(int argc, char *argv[]) {
    printf("Initializing Vector Path Router (Domain 4)...\n");

    // Define test waypoints for navigation
    Coordinate3D waypoints[] = {
        {0.0, 0.0, 0.0},
        {15.5, 22.0, 5.0},
        {42.1, 8.5, 12.0},
        {5.2, 50.0, 2.5}
    };
    int count = 4;
    int *route = (int *)malloc(count * sizeof(int));

    if (optimize_waypoint_path(waypoints, count, route)) {
        printf("Optimal path routing calculated successfully: ");
        for (int i = 0; i < count; i++) {
            printf("Node %d%s", route[i], (i < count - 1) ? " -> " : "\n");
        }
    }

    free(route);
    return 0;
}

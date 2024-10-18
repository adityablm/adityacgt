#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// Generate a random degree sequence
void generateRandomGraphicSequence(int *degrees, int n) {
    for (int i = 0; i < n; ++i) {
        degrees[i] = rand() % n;
    }
    // Sort the sequence in non-increasing order
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (degrees[i] < degrees[j]) {
                int temp = degrees[i];
                degrees[i] = degrees[j];
                degrees[j] = temp;
            }
        }
    }
}

// Havel-Hakimi algorithm to construct the graph
bool havelHakimi(int *degrees, int n, int **graph) {
    while (1) {
        // Sort degrees in descending order
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (degrees[i] < degrees[j]) {
                    int temp = degrees[i];
                    degrees[i] = degrees[j];
                    degrees[j] = temp;
                }
            }
        }

        if (degrees[0] == 0) break; // All degrees are 0, graph is valid
        int d = degrees[0];
        if (d >= n) return false; // Degree too large to construct a valid graph

        // Reduce degrees and connect vertices
        degrees[0] = 0;
        for (int i = 1; i <= d; ++i) {
            degrees[i]--;
            if (degrees[i] < 0) return false;
            graph[0][i] = graph[i][0] = 1; // Add edge
        }
    }
    return true;
}

// Check if the graph is Eulerian
bool isEulerian(int **graph, int n) {
    int oddDegreeCount = 0;
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j) {
            if (graph[i][j]) degree++;
        }
        if (degree % 2 != 0) oddDegreeCount++;
    }
    return (oddDegreeCount == 0 || oddDegreeCount == 2);
}

// Fleury's algorithm to find Euler path or circuit
void fleury(int u, int **graph, int n, int *path, int *pathIndex) {
    for (int v = 0; v < n; ++v) {
        if (graph[u][v]) {
            graph[u][v] = graph[v][u] = 0; // Remove edge
            fleury(v, graph, n, path, pathIndex);
        }
    }
    path[(*pathIndex)++] = u;
}

// Dijkstra’s algorithm for shortest paths
void dijkstra(int **graph, int n, int src, int *dist) {
    bool visited[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[src] = 0;

    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum distance vertex
        int min = INT_MAX, u = -1;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }

        visited[u] = true;

        // Update distances of adjacent vertices
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] && !visited[v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

// Prim's algorithm to find Minimum Spanning Tree (MST)
void prim(int **graph, int n) {
    bool inMST[n];
    int parent[n], key[n];

    for (int i = 0; i < n; ++i) {
        key[i] = INT_MAX;
        inMST[i] = false;
    }
    key[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum key vertex
        int min = INT_MAX, u = -1;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        inMST[u] = true;

        // Update key values of adjacent vertices
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    printf("Minimum Spanning Tree:\n");
    for (int i = 1; i < n; ++i) {
        printf("%d - %d\n", parent[i], i);
    }
}

// Main function
int main() {
    srand(time(0));
    int n;

    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    int *degrees = (int *)malloc(n * sizeof(int));
    int **graph = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        graph[i] = (int *)calloc(n, sizeof(int));
    }

    // Generate random degree sequence and construct graph
    generateRandomGraphicSequence(degrees, n);
    printf("Generated degree sequence: ");
    for (int i = 0; i < n; ++i) printf("%d ", degrees[i]);
    printf("\n");

    if (!havelHakimi(degrees, n, graph)) {
        printf("Graph cannot be constructed with this degree sequence.\n");
        return 0;
    }
    printf("Graph constructed successfully.\n");

    // Check if the graph is Eulerian
    if (isEulerian(graph, n)) {
        printf("The graph is Eulerian.\n");
        int *eulerPath = (int *)malloc(n * sizeof(int));
        int pathIndex = 0;
        fleury(0, graph, n, eulerPath, &pathIndex);
        printf("Eulerian path/circuit: ");
        for (int i = 0; i < pathIndex; ++i) printf("%d ", eulerPath[i]);
        printf("\n");
        free(eulerPath);
    } else {
        printf("The graph is not Eulerian.\n");
    }

    // Assign random weights and run Dijkstra's algorithm
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[i][j]) {
                graph[i][j] = graph[j][i] = rand() % 10 + 1;
            }
        }
    }

    int src;
    printf("Enter the source vertex for shortest path: ");
    scanf("%d", &src);

    int *dist = (int *)malloc(n * sizeof(int));
    dijkstra(graph, n, src, dist);
    printf("Shortest distances from vertex %d: ", src);
    for (int i = 0; i < n; ++i) printf("%d ", dist[i]);
    printf("\n");

    // Find MST using Prim's algorithm
    prim(graph, n);

    free(degrees);
    free(dist);
    for (int i = 0; i < n; ++i) free(graph[i]);
    free(graph);

    return 0;
}
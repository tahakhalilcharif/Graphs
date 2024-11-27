#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct Edge {
    int to;
    int weight;
    struct Edge* next;
} Edge;

typedef struct Graph {
    int nodes;
    Edge** adjList;
} Graph;

Edge* createEdge(int to, int weight) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->to = to;
    edge->weight = weight;
    edge->next = NULL;
    return edge;
}

Graph* createGraph(int nodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nodes = nodes;
    graph->adjList = (Edge**)malloc(nodes * sizeof(Edge*));
    int i;
    for (i = 0; i < nodes; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, int from, int to, int weight) {
    Edge* edge = createEdge(to, weight);
    edge->next = graph->adjList[from];
    graph->adjList[from] = edge;
}

void dfs(Graph* graph, int current, int destination, int currentCost, int* currentPath, int pathLength, 
         int* bestPath, int* bestPathLength, int* minCost, bool* visited) {
    if (current == destination) {
        if (currentCost < *minCost) {
            *minCost = currentCost;
            int i;
            for (i = 0; i < pathLength; i++) {
                bestPath[i] = currentPath[i];
            }
            *bestPathLength = pathLength;
        }
        return;
    }

    visited[current] = true;

    Edge* edge = graph->adjList[current];
    while (edge != NULL) {
        if (!visited[edge->to]) {
            currentPath[pathLength] = edge->to;
            dfs(graph, edge->to, destination, currentCost + edge->weight, currentPath, pathLength + 1, 
                bestPath, bestPathLength, minCost, visited);
        }
        edge = edge->next;
    }

    visited[current] = false;
}

int main() {
    int n, m;
    printf("Enter the number of nodes: ");
    scanf("%d", &n);
    printf("Enter the number of edges: ");
    scanf("%d", &m);

    Graph* graph = createGraph(n);

    printf("Enter the edges (from to weight):\n");
    int i;
    FILE* graphFile = fopen("graph.csv", "w");
    fprintf(graphFile, "From,To,Weight\n");
    for (i = 0; i < m; i++) {
        int from, to, weight;
        scanf("%d %d %d", &from, &to, &weight);
        addEdge(graph, from, to, weight);
        fprintf(graphFile, "%d,%d,%d\n", from, to, weight);
    }
    fclose(graphFile);

    int source, destination;
    printf("Enter the source node: ");
    scanf("%d", &source);
    printf("Enter the destination node: ");
    scanf("%d", &destination);

    bool visited[n];
    int currentPath[n];
    int bestPath[n];
    int pathLength = 0, bestPathLength = 0;
    int minCost = INT_MAX;

    for (i = 0; i < n; i++) {
        visited[i] = false;
    }

    currentPath[0] = source;
    dfs(graph, source, destination, 0, currentPath, 1, bestPath, &bestPathLength, &minCost, visited);

    FILE* resultFile = fopen("results.csv", "w");
    fprintf(resultFile, "Source,Destination,ShortestPath,Cost\n");

    if (minCost == INT_MAX) {
        printf("No path found from %d to %d.\n", source, destination);
        fprintf(resultFile, "%d,%d,No Path Found,Infinity\n", source, destination);
    } else {
        printf("Shortest path: ");
        fprintf(resultFile, "%d,%d,", source, destination);
        for (i = 0; i < bestPathLength; i++) {
            printf("%d", bestPath[i]);
            fprintf(resultFile, "%d", bestPath[i]);
            if (i < bestPathLength - 1) {
                printf(" -> ");
                fprintf(resultFile, " -> ");
            }
        }
        printf("\nTotal cost: %d\n", minCost);
        fprintf(resultFile, ",%d\n", minCost);
    }

    fclose(resultFile);

    for (i = 0; i < n; i++) {
        Edge* edge = graph->adjList[i];
        while (edge != NULL) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);

    return 0;
}

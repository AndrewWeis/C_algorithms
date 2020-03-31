// Count the number of nodes at given level in a tree using BFS.

#include <stdio.h>
#include <stdlib.h>

struct Graph
{
	int V;
	struct adjList* adj;
};

struct adjList
{
	int cur_size;
	int capacity;
	struct growingArr* grArr;
};

struct growingArr
{
	int vertice;
};

struct Graph* createGraph(int V);
void freeGraph(struct Graph* graph, int V);
void addToList(struct Graph* graph, int from, int to);
void printAdjList(struct Graph* graph, int V);

int BFS(struct Graph* graph, int s, int V, int level);

struct Queue* createQueue(unsigned capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int dequeue(struct Queue* queue);
int front(struct Queue* queue);

struct Queue
{
	int front, rear, size;
	unsigned capacity;
	int* array;
};

struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;
	queue->rear = capacity - 1;
	queue->array = (int*)malloc(queue->capacity * sizeof(int));
	return queue;
}

int isFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

void enqueue(struct Queue* queue, int item)
{
	if (isFull(queue))
		return;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
}

int dequeue(struct Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

int front(struct Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	return queue->array[queue->front];
}

struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;
	graph->adj = (struct adjList*)malloc(sizeof(struct adjList) * V);

	for (int i = 0; i < V; i++)
	{
		graph->adj[i].grArr = (struct adjList*)malloc(sizeof(struct adjList) * 2);
		graph->adj[i].capacity = 2;
		graph->adj[i].cur_size = 0;
	}

	return graph;
}

void addToList(struct Graph* graph, int from, int to)
{
	int cur_size = graph->adj[from].cur_size; 
	int capacity = graph->adj[from].capacity; 

	if (cur_size == capacity)
	{
		struct growingArr* grArr = (struct growingArr*)malloc(sizeof(struct growingArr*) * capacity * 2);

		for (int i = 0; i < cur_size; i++)  
			grArr[i] = graph->adj[from].grArr[i];

		grArr[cur_size].vertice = to;  

		free(graph->adj[from].grArr); 

		graph->adj[from].grArr = grArr;
		graph->adj[from].cur_size++;
		graph->adj[from].capacity *= 2;
	}
	else
	{
		graph->adj[from].grArr[cur_size].vertice = to;
		graph->adj[from].cur_size++;
	}
}

void printAdjList(struct Graph* graph, int V)
{
	for (int i = 0; i < V; i++)
	{
		printf("%d: ", i);
		for (int j = 0; j < graph->adj[i].cur_size; j++)
			printf("%d; ", graph->adj[i].grArr[j].vertice);
		printf("\n");
	}
}

int BFS(struct Graph* graph, int s, int V, int l)
{
	// Mark all the vertices as not visited  
	int* visited = (int*)malloc(sizeof(int) * V);
	int* level = (int*)malloc(sizeof(int) * V);
	for (int i = 0; i < V; i++)
	{
		visited[i] = 0;
		level[i] = 0;
	}

	// Create a queue for BFS 
	struct Queue* queue = createQueue(V);

	// Mark the current node as visited and enqueue it 
	visited[s] = 1;
	enqueue(queue, s);
	level[s] = 0;

	// 'vertice' will be used to get all adjacent vertices of a vertex 
	int vertice = 0;
	while (!isEmpty(queue))
	{
		s = front(queue);
		dequeue(queue);

		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for (int j = 0; j < graph->adj[s].cur_size; j++)
		{
			vertice = graph->adj[s].grArr[j].vertice;
			if (!visited[vertice])
			{
				level[vertice] = level[s] + 1;
				visited[vertice] = 1;
				enqueue(queue, vertice);
			}
		}
	}

	int count = 0;
	for (int i = 0; i < V; i++)
		if (level[i] == l)
			count++;
	
	free(level);
	free(visited);
	free(queue->array);
	free(queue);

	return count;
}

void freeGraph(struct Graph* graph, int V)
{
	for (int i = 0; i < V; i++)
		free(graph->adj[i].grArr);
	free(graph->adj);
	free(graph);
}

int main()
{
	int V = 6;
	struct Graph* graph = createGraph(V);
	addToList(graph, 0, 1);
	addToList(graph, 0, 2);
	addToList(graph, 1, 3);
	addToList(graph, 2, 4);
	addToList(graph, 2, 5);

	int level = 2;
	printf("Vertices by giving level: %d", BFS(graph, 0, V, level));

	freeGraph(graph, V);

	return 0;
}

/*
	    (0)     -- level 0
	  /	\ 
	(1)     (2) -- level 1
	 |    /  | 
        (3) (5)	(4) -- level 2
*/

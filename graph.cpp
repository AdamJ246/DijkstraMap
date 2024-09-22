using namespace std;

// This file contains the implementations of the member functions
// for class Graph that are given in the text.
const int NULL_EDGE = 0;

template<class VertexType>
Graph<VertexType>::Graph()
// Post: Arrays of size 50 are dynamically allocated for
//       marks and vertices. numVertices is set to 0;
//       maxVertices is set to 50.
{
    numVertices = 0;
    maxVertices = 50;
    ClearMarks();
}

template<class VertexType>
bool Graph<VertexType>::IsFull() const
{
	return (numVertices == maxVertices);
}

template<class VertexType>
bool Graph<VertexType>::IsEmpty() const
{
	return (numVertices == 0);
}

template<class VertexType>
void Graph<VertexType>::ClearMarks()
{
    for (int i = 0; i < maxVertices;  i++)
		marks[i] = false;
}


template<class VertexType>
void Graph<VertexType>::MarkVertex(VertexType s)
{
	int i = 0;  bool found = false;

	while ((i < maxVertices) && (! found))
	{
			if (vertices[i] == s)
	                { 	marks[i] = true;  found = true;  }
			i++;
	}
}	

template<class VertexType>
bool Graph<VertexType>::IsMarked(VertexType s) const
{
	int i = 0;  bool found = false;

	while ((i < maxVertices))
	{
			if (vertices[i] == s)
	                 	return (marks[i]);
			i++;
	}
	return false;
}

template<class VertexType>
Graph<VertexType>::Graph(int maxV)
// Post: Arrays of size maxV are dynamically allocated for
//       marks and vertices.
//       numVertices is set to 0; maxVertices is set to maxV.
{
    numVertices = 0;
    maxVertices = maxV;

    for (int i = 0; i < maxVertices;  i++)
		marks[i] = false;
    ClearMarks();
}

template<class VertexType>
Graph<VertexType>::~Graph()
// Post: arrays for vertices and marks have been deallocated.
//       (to be used only when constructor uses dynamic allocation)
{
    //delete [] vertices;
    //delete [] marks;
}

template<class VertexType>
void Graph<VertexType>::AddVertex(VertexType vertex)
// Post: vertex has been stored in vertices.
//       Corresponding row and column of edges have been set
//       to NULL_EDGE.
//       numVertices has been incremented.
{
    vertices[numVertices] = vertex;
    for (int index = 0; index < numVertices; index++)
    {
	edges[numVertices][index] = NULL_EDGE;
	edges[index][numVertices] = NULL_EDGE;
    }
    numVertices++;
}

template<class VertexType>
int Graph<VertexType>::IndexIs(VertexType vertex) const
// Post: Function value = index of vertex in vertices.
{
    int index = 0;
    while (! (vertex == vertices[index]))
	index++;
    return index;
}

template<class VertexType>
void Graph<VertexType>::AddEdge(VertexType fromVertex,
     VertexType toVertex, int weight)
// Post: Edge (fromVertex, toVertex) is stored in edges.
{
    int row;
    int col;
    row = IndexIs(fromVertex);
    col = IndexIs(toVertex);
    edges[row][col] = weight;
}

template<class VertexType>
int Graph<VertexType>::WeightIs(VertexType fromVertex,
    VertexType toVertex) const
// Post: Function value = weight associated with the edge
//       (fromVertex, toVertex).
{
    int row;
    int col;
    row = IndexIs(fromVertex);
    col = IndexIs(toVertex);
    return edges[row][col];
}

template<class VertexType>
void Graph<VertexType>::GetToVertices(VertexType vertex,
     Queue<VertexType>& adjvertexQ) const
{
    int fromIndex;
    int toIndex;

    fromIndex = IndexIs(vertex);
    for (toIndex = 0; toIndex < numVertices; toIndex++)
	if (edges[fromIndex][toIndex] != NULL_EDGE)
	    adjvertexQ.enqueue(vertices[toIndex]);
}

// This file contains the definition of class Graph that
// represents the Graph ADT.

#include "queue.h"

template<class VertexType>
// Assumption: VertexType is a type for which the "=",
// "==", and "<<" operators are defined

class Graph
{
public:
    Graph();          // constructor, default of 50 vertices.
    Graph(int maxV);  // parameterized constructor, maxV <= 50.
    ~Graph();         // destructor
    void MakeEmpty();
    bool IsEmpty() const;
    bool IsFull() const;
    void AddVertex(VertexType);
    void AddEdge(VertexType, VertexType, int);
    int WeightIs(VertexType, VertexType) const;
    void GetToVertices(VertexType, Queue<VertexType>&) const;
    void ClearMarks();
    void MarkVertex(VertexType);
    bool IsMarked(VertexType) const;

private:
    int numVertices;
    int maxVertices;
    VertexType vertices[50];
    int edges[50][50];
    bool marks[50]; // marks[i] is mark for vertices[i].

    int IndexIs(VertexType vertex) const;
};

#include "graph.cpp"




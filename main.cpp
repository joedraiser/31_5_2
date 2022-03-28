#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

class IGraph
{

public:

    virtual ~IGraph() {}

    IGraph() {};

    IGraph(IGraph *_oth) {};

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;

    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;

};

class ListGraph : public IGraph
{
private:
    std::vector< std::vector<int> > graph;
public:
    ListGraph(ListGraph* oth)
    {
        graph=oth->graph;
    }

    virtual void AddEdge(int from, int to)
    {
        if(graph.size()<from || graph.size()<to)
        {
            graph.insert(graph.end(), (from > to ? from : to)-graph.size(), std::vector<int> ());
        }

        graph[from-1].push_back(to);
    }

    virtual int VerticesCount() const
    {
        int sum;
        for(int i=0;i<graph.size();i++)
        {
            if(graph[i].size()!=0)
                sum++;
        }

        return sum;
    };

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const
    {
        vertices.insert(vertices.end(), graph[vertex-1].begin(), graph[vertex-1].end() );
    };

    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const
    {
        for(int i=0;i<graph.size();i++)
        {
            if(std::find(graph[i].begin(), graph[i].end(), vertex)!=graph[i].end())
                vertices.push_back(i+1);
        }
    };

    ~ListGraph() {};
};

class MatrixGraph : public IGraph
{
private:
    std::vector< std::vector<int> > graph;
public:
    
    virtual void AddEdge(int from, int to)
    {
        if(graph.size()<from || graph.size()<to)
        {
            graph.insert(graph.end(), (from > to ? from : to)-graph.size(), std::vector<int> ());
        }

        graph[from-1].push_back(to);
    }
};

int main()
{
    ListGraph a;
    std::vector<int> b;

    a.AddEdge(2, 3);
    a.AddEdge(8,2);
    a.AddEdge(2,4);
    a.AddEdge(1,2);
    a.AddEdge(7,2);

    //a.GetNextVertices(2, b);
    a.GetPrevVertices(2,b);
    return 0;
}

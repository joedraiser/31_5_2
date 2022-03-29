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

class MatrixGraph;

class ListGraph : public IGraph
{
private:
    std::vector< std::vector<int> > graph;
public:
    ListGraph() {};

    ListGraph(ListGraph* oth)
    {
        graph=oth->graph;
    }

    ListGraph(MatrixGraph* oth);

    virtual void AddEdge(int from, int to)
    {
        if(graph.size()<from || graph.size()<to)
        {
            graph.insert(graph.end(), (from > to ? from : to)-graph.size(), std::vector<int> ());
        }
        if(std::find(graph[from-1].begin(), graph[from-1].end(), to)==graph[from-1].end())
            graph[from-1].push_back(to);
    }

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

    virtual int VerticesCount() const
    {
        int sum=0;
        std::vector<int> temp;
        for(int i=0;i<graph.size();i++)
        {
            this->GetNextVertices(i+1, temp);
            this->GetPrevVertices(i+1, temp);
            if(!temp.empty())
                sum++;

            temp.clear();
        }

        return sum;
    };

    int getMaxVertexNum() const
    {
        return graph.size();
    }

    ListGraph& operator=(const MatrixGraph& oth);

    ~ListGraph() {};
};

class MatrixGraph : public IGraph
{
private:
    std::vector< std::vector<int> > graph;
public:
    MatrixGraph() {};

    MatrixGraph(MatrixGraph* oth)
    {
        graph=oth->graph;
    }

    MatrixGraph(ListGraph* oth)
    {
        int size=oth->VerticesCount();
        std::vector<int> temp;

        for(int i=oth->getMaxVertexNum();i>0;i--)
        {
            oth->GetNextVertices(i, temp);
            while(!temp.empty())
            {
                this->AddEdge(i, *(temp.end()-1));
                temp.pop_back();
            }
            oth->GetPrevVertices(i, temp);
            while(!temp.empty())
            {
                this->AddEdge(*(temp.end()-1), i);
                temp.pop_back();
            }
        }
    }

    MatrixGraph& operator=(const ListGraph& oth)
    {
        graph.clear();

        int size=oth.VerticesCount();
        std::vector<int> temp;

        for(int i=oth.getMaxVertexNum();i>0;i--)
        {
            oth.GetNextVertices(i, temp);
            while(!temp.empty())
            {
                this->AddEdge(i, *(temp.end()-1));
                temp.pop_back();
            }
            oth.GetPrevVertices(i, temp);
            while(!temp.empty())
            {
                this->AddEdge(*(temp.end()-1), i);
                temp.pop_back();
            }
        }

        return *this;
    }

    virtual void AddEdge(int from, int to)
    {
        if(graph.size()<from || graph.size()<to)
        {
            graph.insert(graph.end(), (from > to ? from : to)-graph.size(), std::vector<int> ());
            for(int i=0;i<graph.size();i++)
            {
                graph[i].insert(graph[i].end(), graph.size()-graph[i].size(), 0);
            }
        }

        graph[from-1][to-1]=1;
    }

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const
    {
        for(int i=0;i<graph.size();i++)
        {
            if(graph[vertex-1][i]!=0)
            {
                vertices.insert(vertices.end(), 1, i+1);
            }
        }
    };

    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const
    {
        for(int i=0;i<graph.size();i++)
        {
            if(graph[i][vertex-1]==1)
            {
                vertices.insert(vertices.end(), 1, i+1);
            }
        }
    };

    virtual int VerticesCount() const
    {
        int sum=0;
        std::vector<int> temp;
        for(int i=0;i<graph.size();i++)
        {
            this->GetNextVertices(i+1, temp);
            this->GetPrevVertices(i+1, temp);
            if(!temp.empty())
                sum++;

            temp.clear();
        }

        return sum;
    };

    int getMaxVertexNum() const
    {
        return graph.size();
    }

    ~MatrixGraph() {};
};

ListGraph::ListGraph(MatrixGraph *oth)
{
    int size=oth->VerticesCount();
    std::vector<int> temp;

    for(int i=oth->getMaxVertexNum();i>0;i--)
    {
        oth->GetNextVertices(i, temp);
        while(!temp.empty())
        {
            this->AddEdge(i, *(temp.end()-1));
            temp.pop_back();
        }
        oth->GetPrevVertices(i, temp);
        while(!temp.empty())
        {
            this->AddEdge(*(temp.end()-1), i);
            temp.pop_back();
        }
    }
}

ListGraph& ListGraph::operator=(const MatrixGraph& oth)
{
    graph.clear();

    int size=oth.VerticesCount();
    std::vector<int> temp;

    for(int i=oth.getMaxVertexNum();i>0;i--)
    {
        oth.GetNextVertices(i, temp);
        while(!temp.empty())
        {
            this->AddEdge(i, *(temp.end()-1));
            temp.pop_back();
        }
        oth.GetPrevVertices(i, temp);
        while(!temp.empty())
        {
            this->AddEdge(*(temp.end()-1), i);
            temp.pop_back();
        }
    }

    return *this;
}

int main()
{
    ListGraph LG;
    std::vector<int> a;
    int count;

    LG.AddEdge(2, 3);
    LG.AddEdge(8,2);
    LG.AddEdge(2,4);
    LG.AddEdge(1,2);
    LG.AddEdge(7,2);

    //a.GetNextVertices(2, b);
    LG.GetNextVertices(2,a);
    count=LG.VerticesCount();

    MatrixGraph MG;
    std::vector<int> b;

    MG.AddEdge(1,2);
    MG.AddEdge(5,2);
    MG.AddEdge(8,2);
    MG.AddEdge(2,4);
    MG.AddEdge(7,2);
    MG.AddEdge(6,11);
    MG.GetNextVertices(2,b);
    count=MG.VerticesCount();

    ListGraph newLG(&MG);
    MatrixGraph newMG(&LG);

    newLG=newMG;
    LG=MG;
    newMG=LG;

    return 0;
}

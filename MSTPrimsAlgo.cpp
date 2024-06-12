#include <iostream>
#include <vector>
#include <iterator>
#include <climits>

#define V  5

std::vector<int> visit(V,false);
std::vector<int> edges(V, INT_MAX);
std::vector<int> path(V, INT_MAX);
auto get_min_edge_vertex()
{
    int min_edge_vertex, min_edge = INT_MAX;
    for(int i = 0; i< V; ++i )
        if ( !visit[i] && edges[i] < min_edge )
            min_edge = edges[i], min_edge_vertex = i;
    return min_edge_vertex;
}

void prism_mst(int graph[V][V])
{
    edges[0] = 0;
    path[0]=-1;
    for(int count = 0; count < V-1; ++count)
    {
        int u = get_min_edge_vertex();
        visit[u] = true;
        for(int v=0; v<V; ++v)
        {
            if ( graph[u][v] && !visit[v] && graph[u][v] < edges[v])
                edges[v] = graph[u][v], path[v]=u;
        }
    }
    std::copy(edges.begin(), edges.end(), std::ostream_iterator<int>(std::cout, " "));std::cout<<'\n';

    for(int i = 1; i<V; ++i )
    {
        std::cout<<path[i] <<"-->"<<i<<" - "<< graph[i][path[i]]<<'\n';
    }
}

int main()
{
    int graph[V][V] = 
    { 
        { 0, 2, 0, 6, 0 },
        { 2, 0, 3, 8, 5 },
        { 0, 3, 0, 0, 7 },
        { 6, 8, 0, 0, 9 },
        { 0, 5, 7, 9, 0 } 
    };
    prism_mst(graph);
 
    return 0;
}

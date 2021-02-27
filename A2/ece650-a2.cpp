#include <iostream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <list>

using namespace std;
class Graph
{
    int V;    // No. of vertices


    // Pointer to an array containing adjacency lists
    vector<int> *adj;
    //Graph(int x)
public:
    void set(int val);
    string path;
    //Graph(int val);//:adj(new vector[V]) {}; // Constructor

    // add an ege from vertex v to w
    void addEge(int v, int w);


    //bool isReachable(int s, int d);
   bool BFS( int src, int dest, int v, int pred[], int dist[]);
   void printShortestDistance( int s,int dest);
};



bool Graph::BFS( int src, int dest, int v, int pred[], int dist[])
{
    // a queue to maintain queue of vertices whose
    // adjacency list is to be scanned as per normal
    // DFS algorithm
    list<int> queue;

    // boolean array visited[] which stores the
    // information whether ith vertex is reached
    // at least once in the Breadth first search
    bool visited[v];

    // initially all vertices are unvisited
    // so v[i] for all i is false
    // and as no path is yet constructed
    // dist[i] for all i set to infinity
    for (int i = 0; i < v; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
        pred[i] = -1;
    }

    // now source is first to be visited and
    // distance from source to itself should be 0
    visited[src] = true;
    dist[src] = 0;
    queue.push_back(src);

    // standard BFS algorithm
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++) {
            if (visited[adj[u][i]] == false) {
                visited[adj[u][i]] = true;
                dist[adj[u][i]] = dist[u] + 1;
                pred[adj[u][i]] = u;
                queue.push_back(adj[u][i]);

                // We stop BFS when we find
                // destination.
                if (adj[u][i] == dest)
                   return true;
            }
        }
    }

    return false;
}
// utility function to print the shortest distance
// between source vertex and destination vertex
void Graph::printShortestDistance( int s,int dest)
{
    // predecessor[i] array stores predecessor of
    // i and distance array stores distance of i
    // from s
    int pred[V], dist[V];

    if (BFS(s, dest, V, pred, dist) == false)
    {
        cout << "Error: Given source and destination are not connected\n";
        return;
    }

    // vector path stores the shortest path
    vector<int> path;
    int crawl = dest;
    path.push_back(crawl);
    while (pred[crawl] != -1) {
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }

    // distance from source is in distance array
    //cout << "Shortest path length is : " << dist[dest];

    // printing path from source to destination
    //cout << "\nPath is::\n";
    for (int i = path.size() - 1; i >= 0; i--)
        if(i==0)
            cout << path[i]<<endl;
        else
            cout << path[i] << "-";
}

void Graph::set(int val)
{
     V = val;
     adj = new vector<int>[V];
}

//vector<int> v;
//vector<int> adj[] = new vector<int>[5];


 void Graph::addEge(int v, int w)
{
     try
     {
         // cout<<"in";
         for (int i :adj[v])
         {
             if(i==w)
                 throw "Error: Invalid edge input\n";
             //cout<<i<<endl;
         }
     }
     catch(const char* msg)
     {
         //cerr<<msg<<endl;
         throw msg;
     }
    adj[v].push_back(w); // Add w to v’s list.
    adj[w].push_back(v); // Add v to w’s list.
}

string command;
string final_data;
 int size=0;
 Graph g;
string splitInput(string in)
{
    string word;
    string out;
    istringstream splits(in, istringstream::in);


    splits>>command;

    if(command.compare("E")==0)
    {

       g.set(size);
        splits >> word ;
        int n = word.length();
        char arr [n+1];

        strcpy(arr, word.c_str());
        string x="",y="";
        for (int i=0;i<n;i++)
        {
            if(arr[i]=='<' )
            {

                while(arr[++i]!=',')
                {

                    x.push_back(arr[i]);

                }

                while(arr[++i]!='>')
                {

                    y.push_back(arr[i]);


                }

                 stringstream x_val(x);
                 stringstream y_val(y);
                 int x_value=0,y_value=0;
                 x_val>>x_value;
                 y_val>>y_value;
                 try
                 {
                     if(x_value>=size ||x_value<0 || y_value>=size ||y_value<0)
                         throw "Error: Invalid input\n";

                     g.addEge(x_value, y_value);
                 }

                 catch(const char* msg)
                 {
                      g.set(0);
                     cerr << msg << endl;
                    // return "";
                 }
                    x="";
                    y="";


            }


        }

    }
    else if (command.compare("s")==0)
    {

        string start;
        string end;
        int x=0,y=0;
        splits>>start;

         stringstream value(start);
         value>>x;

        splits>>end;
       stringstream value2(end);
         value2>>y;
         try
         {
            g.printShortestDistance(x, y);
         }
         catch(...)
         {
             cerr<<"Error: you are not allowed" << endl;
         }
    }
     else if (command.compare("V")==0)
     {
         splits >> word ;
         stringstream value(word);
         value>>size;


     }
     else
     {
    	 cout<<"Error: Invalid command" << endl;
     }


    return out;
}
int main(int argc, char** argv) {

    string input;
    string data;


    while(true)
    {

        try
        {

        	//cout<<"Enter your option \n";
            getline(cin, input);

            if (cin.eof())
            {
                break;
            }
           data = splitInput(input);

        }
        catch(...)
        {
            cerr << "Error: Incorrect Execution." << endl;
        }
    }

    return 0;
}

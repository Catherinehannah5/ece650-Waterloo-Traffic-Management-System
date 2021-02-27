#include <minisat/core/Solver.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>
#include <regex>
#include <cstdlib>
#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define RUNNING true
#define FINISHED false

#define DEBUG std::getenv("DEBUG") != NULL
#define VERBOSE std::getenv("VERBOSE") != NULL
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)
#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

// thread related shared variables
pthread_t io_thread, cnf_sat_vc_thread, approx_vc_1_thread, approx_vc_2_thread;
long long  cnf_sat_thread_time, approx_vc_1_thread_time, approx_vc_2_thread_time;
std::vector<int> cnf_sat_vc, approx_vc_1, approx_vc_2;
bool cnf_sat_vc_thread_status, cnf_thread_timeout = false;
static Minisat::Solver *solver;

std::vector<std::pair<int, int>> parse_edges(std::string str)
{
    std::pair<int, int> edge;
    std::vector<std::pair<int, int>> edges;

    // using regex
    try
    {
        std::regex re("-?[0-9]+"); //match consectuive numbers
        std::sregex_iterator next(str.begin(), str.end(), re);
        std::sregex_iterator end;
        while (next != end)
        {
            std::smatch match1;
            std::smatch match2;

            match1 = *next;
            next++;
            // iterate to next match
            if (next != end)
            {
                match2 = *next;
                edge.first = std::stoi(match1.str());
                edge.second = std::stoi(match2.str());
                edges.push_back(edge);
                next++;
            }
        }
    }
    catch (std::regex_error &e)
    {
        edges.clear();
    }
    return edges;
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool pair_exist_in_vector(std::vector<std::pair<int, int>> vecOfPairs, std::pair<int, int> pair)
{
    auto it = std::find(vecOfPairs.begin(), vecOfPairs.end(), pair);
    if (it != vecOfPairs.end())
        return true;
    return false;
}
// This class represents a undirected graph using adjacency list
class Graph
{
    int V; // Number of vertices

    // an array containing adjacency lists
    std::list<int> *adj;

public:
    Graph(int V)
    {
        this->V = V;
        adj = new std::list<int>[V];
    }

    // for adding edge
    void add_edge(int v, int w)
    {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    int get_vertices_count()
    {
        return V;
    }

    std::vector<std::pair<int, int>> get_edges()
    {
        std::pair<int, int> edge;
        std::vector<std::pair<int, int>> edges;
        for (int i = 0; i < V; i++)
        {
            for (int j : adj[i])
            {
                if (!pair_exist_in_vector(edges, std::make_pair(i, j)) && !pair_exist_in_vector(edges, std::make_pair(j, i)))
                {
                    edges.push_back((std::pair<int, int>)std::make_pair(i, j));
                }
            }
        }
        return edges;
    }

    std::vector<std::vector<bool>> get_edges_boolean_matrix()
    {
        // create VxV matrix with default value as false
        std::vector<std::vector<bool>> bool_mat(V, std::vector<bool>(V, false));

        for (int i = 0; i < V; i++)
        {
            for (int j : adj[i])
            {
                bool_mat[i][j] = true;
            }
        }
        return bool_mat;
    }
};

void printLiteralsMatrix(std::vector<std::vector<Minisat::Lit>> mat)
{
    for (unsigned int i = 0; i < mat.size(); ++i)
    {
        for (unsigned int j = 0; j < mat[i].size(); ++j)
        {
            std::cout << Minisat::toInt(mat[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void printEdgesBooleanMatrix(std::vector<std::vector<bool>> bool_mat)
{
    for (auto i : bool_mat)
    {
        for (auto j : i)
        {
            std::cout << j << " ";
        }
        std::cout << '\n';
    }
}

void printVertexCover(std::vector<int> vc)
{
    if (vc.size() == 0)
    {
        return;
    }
	std::sort(vc.begin(),vc.end());
    auto vc_size = vc.size();
    for (unsigned int i = 0; i < vc_size; i++)
    {
        if (i + 1 == vc_size)
            std::cout << vc[i];
        else
            std::cout << vc[i] << ",";
    }
}

std::vector<int> findVertexCoverByCNFSAT(Graph graph, int cover_size)
{
    int V = graph.get_vertices_count();
    std::vector<std::vector<Minisat::Lit>> matrix(V);
    solver = new Minisat::Solver();

    if (VERBOSE)
    {
        solver->verbosity = 1;
    }

    for (int n = 0; n < V; ++n)
    {
        for (int k = 0; k < cover_size; ++k)
        {
            Minisat::Lit literal = Minisat::mkLit(solver->newVar());
            matrix[n].push_back(literal);
        }
    }
    // printLiteralsMatrix(matrix);

    // Rule 1: at least (exactly only one) one vertex is the ith vertex in the vertex cover, i in [1,k]
    for (int k = 0; k < cover_size; ++k)
    {
        Minisat::vec<Minisat::Lit> literals;
        for (int n = 0; n < V; ++n)
        { // n is changing first
            literals.push(matrix[n][k]);
        }
        solver->addClause(literals);
        literals.clear();
    }

    // Rule 2: no one vertex can appear twice in a vertex cover
    for (int n = 0; n < V; ++n)
    {
        for (int p = 0; p < cover_size - 1; ++p)
        {
            for (int q = p + 1; q < cover_size; ++q)
            {
                solver->addClause(~matrix[n][p], ~matrix[n][q]);
            }
        }
    }

    // Rule 3: no more than one vertex appears in the mth position of the vertex cover
    for (int m = 0; m < cover_size; ++m)
    {
        for (int p = 0; p < V - 1; ++p)
        {
            for (int q = p + 1; q < V; ++q)
            {
                solver->addClause(~matrix[p][m], ~matrix[q][m]);
            }
        }
    }

    std::vector<std::pair<int, int>> edges = graph.get_edges();
    if (edges.size() == 0)
    {
        // a graph having empty set of edges will never satisfy for minimum vertex cover
        return {-1};
    }

    //Rule 4: every edge is incident to at least one vertex in the vertex cover
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
        Minisat::vec<Minisat::Lit> literals;
        for (int k = 0; k < cover_size; ++k)
        {
            literals.push(matrix[it->first][k]);
            literals.push(matrix[it->second][k]);
        }
        solver->addClause(literals);
        literals.clear();
    }

    Minisat::vec<Minisat::Lit> dummy;
    Minisat::lbool satisfy = solver->solveLimited(dummy);

    if (satisfy == Minisat::l_True)
    {
        std::vector<int> result;

        for (int n = 0; n < V; n++)
        {
            for (int k = 0; k < cover_size; ++k)
            {
                if (Minisat::toInt(solver->modelValue(matrix[n][k])) == 0)
                {
                    result.push_back(n);
                }
            }
        }
        return result;
    }
    else
    {
        return {-1};
    }
}

std::vector<int> findMinLengthVertexCoverByCNFSAT(Graph graph)
{
    VERBOSE &&std::cerr << "Started CNF-SAT-VC algorithm" << std::endl;
    std::vector<int> result;
    std::vector<int> temp_result;

    int start = 1;
    int end = graph.get_vertices_count();

    while (start <= end && !cnf_thread_timeout)
    {
        // We are using Divide & Conquer for searching minimum vertex cover fast
        int mid = floor((start + end) / 2);

        VERBOSE &&std::cerr << "Finding VC of size " << mid << " for graph by CNF-SAT" << std::endl;
        temp_result = findVertexCoverByCNFSAT(graph, mid);

        bool coverNotExists = std::equal(temp_result.begin(), temp_result.end(), (std::vector<int>{-1}).begin());

        if (coverNotExists)
        {
            VERBOSE &&std::cerr << "Cover not exists of size " << mid << std::endl;
            start = mid + 1;
        }
        else
        {
            VERBOSE &&std::cerr << "Cover exists of size " << mid << std::endl;
            end = mid - 1;
            result.clear();
            result = temp_result;
        }
    }

    if (result.size() >= 1 && result[0] != -1)
    {
        std::sort(result.begin(), result.end());
        VERBOSE &&std::cerr << "We found VC for graph by CNF-SAT" << std::endl;
        VERBOSE &&std::cerr << "Completed CNF-SAT-VC algorithm" << std::endl;
        return result;
    }
    else
    {
        VERBOSE &&std::cerr << "We don't find any VC for graph by CNF-SAT" << std::endl;
        VERBOSE &&std::cerr << "Completed CNF-SAT-VC algorithm" << std::endl;
        return std::vector<int>{};
    }
}
std::vector<int> findMinLengthVertexCoverByApprox1(Graph graph)
{
    VERBOSE &&std::cerr << "Started APPROX-VC-1 algorithm" << std::endl;
    int V = graph.get_vertices_count();
    std::vector<std::vector<bool>> bool_mat = graph.get_edges_boolean_matrix();
    std::vector<int> result;

    int max_degree, degree, max_degree_vertex;

    while (true)
    {
        max_degree = 0;
        max_degree_vertex = -1;
        for (int i = 0; i < V; i++)
        {
            degree = 0;
            for (int j = 0; j < V; j++)
            {
                if (bool_mat[i][j] == true)
                {
                    degree++;
                }
            }
            if (max_degree < degree)
            {
                max_degree = degree;
                max_degree_vertex = i;
            }
        }
        if (max_degree_vertex != -1)
        {
            VERBOSE &&std::cerr << "Adding vertex to APPROX-VC-1 set : " << max_degree_vertex << std::endl;
            result.push_back(max_degree_vertex);
            for (int i = 0; i < V; i++)
            {
                bool_mat[max_degree_vertex][i] = false;
                bool_mat[i][max_degree_vertex] = false;
            }
        }
        else
        {
            break;
        }
    }

    VERBOSE &&std::cerr << "Completed APPROX-VC-1 algorithm" << std::endl;
    return result;
}
std::vector<int> findMinLengthVertexCoverByApprox2(Graph graph)
{
    VERBOSE &&std::cerr << "Started APPROX-VC-2 algorithm" << std::endl;
    int V = graph.get_vertices_count();
    std::vector<std::vector<bool>> bool_mat = graph.get_edges_boolean_matrix();
    std::vector<int> result;

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (bool_mat[i][j] == true)
            {
                VERBOSE &&std::cerr << "Adding vertex to APPROX-VC-2 set : " << i << std::endl;
                VERBOSE &&std::cerr << "Adding vertex to APPROX-VC-2 set : " << j << std::endl;
                result.push_back(i);
                result.push_back(j);
                for (int k = 0; k < V; k++)
                {
                    bool_mat[i][k] = false;
                    bool_mat[j][k] = false;
                    bool_mat[k][i] = false;
                    bool_mat[k][j] = false;
                }
            }
        }
    }

    VERBOSE &&std::cerr << "Completed APPROX-VC-2 algorithm" << std::endl;
    return result;
}

void printVCResultForAlgo(std::vector<int> vc, std::string algo_name)
{
    std::cout << algo_name << ": ";
    if (algo_name == "CNF-SAT-VC" && cnf_thread_timeout)
    {
        std::cout << "timeout";
    }
    else
    {
        printVertexCover(vc);
    }

    std::cout << std::endl;
}

Graph g(0);

static void pclock(char *msg, clockid_t cid)
{
    struct timespec ts;

    printf("%s", msg);
    if (clock_gettime(cid, &ts) == -1)
        handle_error("clock_gettime");
    printf("%4ld.%03ld s\n", ts.tv_sec, ts.tv_nsec / 1000000);
}

clockid_t get_cpu_clockid_for_thread(pthread_t thread_id)
{
    clockid_t cid;
    if (pthread_getcpuclockid(thread_id, &cid))
    {
        std::cerr << "Error: can't get the cpu clock" << std::endl;
    }
    return cid;
}

void thread_timeout_watcher(pthread_t thread_id, int time_limit)
{
    clockid_t cid = get_cpu_clockid_for_thread(thread_id);
    struct timespec tspec;

    while (cnf_sat_vc_thread_status == RUNNING)
    {
        clock_gettime(cid, &tspec);
        if (tspec.tv_sec > time_limit)
        {
            cnf_thread_timeout = true;
            solver->interrupt();
            if (solver->verbosity > 0)
            {
                solver->printStats();
            }
            VERBOSE &&std::cerr << "CNF-SAT-VC Thread timeout called after " << tspec.tv_sec << " secs" << std::endl;
            return;
        }
    }
}

static long double get_clocktime(clockid_t cid)
{
    struct timespec ts;
    if (clock_gettime(cid, &ts) == -1)
        std::cerr << "Error: can't get the time of clock" << std::endl;
    return (ts.tv_sec / 1000000) + (ts.tv_nsec / 1000);
}

void *cnf_sat_vc_thread_handler(void *arg)
{

    auto start = std::chrono::high_resolution_clock::now();

    cnf_sat_vc_thread_status = RUNNING;
    cnf_sat_vc = findMinLengthVertexCoverByCNFSAT(g);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    cnf_sat_thread_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    // clockid_t cid = get_cpu_clockid_for_thread(pthread_self());
    // cnf_sat_thread_time = get_clocktime(cid);

    cnf_sat_vc_thread_status = FINISHED;
    return NULL;
}

void *approx_vc_1_thread_handler(void *arg)
{
    auto start = std::chrono::high_resolution_clock::now();
    approx_vc_1 = findMinLengthVertexCoverByApprox1(g);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    approx_vc_1_thread_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    // clockid_t cid = get_cpu_clockid_for_thread(pthread_self());
    // approx_vc_1_thread_time = get_clocktime(cid);
    return NULL;
}

void *approx_vc_2_thread_handler(void *arg)
{
    auto start = std::chrono::high_resolution_clock::now();

    approx_vc_2 = findMinLengthVertexCoverByApprox2(g);

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    approx_vc_2_thread_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    // clockid_t cid = get_cpu_clockid_for_thread(pthread_self());
    // approx_vc_2_thread_time = get_clocktime(cid);

    return NULL;
}

void *input_output_thread_handler(void *arg)
{
    std::string line;
    char cmd;
    int vertices_count;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::string> line_split;
    bool graph_created = true;

    while (std::getline(std::cin, line))
    {
        cmd = line[0];
        switch (cmd)
        {
        case 'V':
        case 'v':
            /* code for adding vertices to new graph */
            // V 15
            vertices_count = stoi(line.substr(2, std::string::npos), nullptr, 10);
            g = Graph(vertices_count);
            break;

        case 'E':
        case 'e':
            /* code for adding edges to new graph*/
            // E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}
            edges = parse_edges(line.substr(2, std::string::npos));
            for (unsigned int i = 0; i < edges.size(); i++)
            {
                int v = edges[i].first;
                int w = edges[i].second;
                if (v >= vertices_count || w >= vertices_count)
                {
                    graph_created = false;
                    std::cerr << "Error : edge (" << v << "," << w << ") is invalid" << std::endl;
                    break;
                }
                g.add_edge(v, w);
            }

            if (graph_created)
            {

                pthread_create(&cnf_sat_vc_thread, NULL, &cnf_sat_vc_thread_handler, NULL);
                pthread_create(&approx_vc_1_thread, NULL, &approx_vc_1_thread_handler, NULL);
                pthread_create(&approx_vc_2_thread, NULL, &approx_vc_2_thread_handler, NULL);

                // we will set timeout for CNF-SAT algorithm thread
                thread_timeout_watcher(cnf_sat_vc_thread, 120);

                pthread_join(cnf_sat_vc_thread, NULL);
                pthread_join(approx_vc_1_thread, NULL);
                pthread_join(approx_vc_2_thread, NULL);

                printVCResultForAlgo(cnf_sat_vc, "CNF-SAT-VC");
                printVCResultForAlgo(approx_vc_1, "APPROX-VC-1");
                printVCResultForAlgo(approx_vc_2, "APPROX-VC-2");

                DEBUG &&std::cout << "CNF-SAT-VC CPU time:   " << cnf_sat_thread_time << " µs" << std::endl;
                DEBUG &&std::cout << "APPROX-VC-1 CPU time:   " << approx_vc_1_thread_time << " µs" << std::endl;
                DEBUG &&std::cout << "APPROX-VC-2 CPU time:   " << approx_vc_2_thread_time << " µs" << std::endl;
            }

            break;
        default:
            std::cerr << "Error: command not recognized" << std::endl;
            break;
        }
    }
    return NULL;
}

int main()
{
    int retcode;
    std::string input;

    try
    {
        retcode = pthread_create(&io_thread, NULL, &input_output_thread_handler, NULL);
        if (retcode)
        {
            return 0;
        }

        retcode = pthread_join(io_thread, NULL);
        if (DEBUG)
        {
            pclock("Process total CPU time: ", CLOCK_PROCESS_CPUTIME_ID);
            int s;
            clockid_t cid;
            s = pthread_getcpuclockid(pthread_self(), &cid);
            if (s != 0)
                handle_error_en(s, "pthread_getcpuclockid");
            pclock("Main thread CPU time:   ", cid);
        }

        if (retcode)
        {
            return 0;
        }
    }
    catch (...)
    {
        std::cerr << "Error: Incorrect Execution." << std::endl;
    }
    return 0;
}

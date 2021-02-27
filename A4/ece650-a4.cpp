#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <list>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <minisat/core/Solver.h>


std::string replacestring(std::string str, const std::string& replace, char ch) {

    size_t found = str.find_first_of(replace);



    while (found != std::string::npos) {				 // While our position in the sting is in range.
        str[found] = ch;								 // Change the character at position.
        found = str.find_first_of(replace, found+1);	// Relocate again.

    }

    return str;											// return our new string.

}



class Graph

{

    int V; // number of vertices provided
public:

    std::list<int> *adj; 
    
	Graph(int Val)
	{
    this->V = Val;
    adj = new std::list<int> [V];
	}

    //void addEdge(int v, int w);
	void addEdge(int v, int w)  // function to add an edge to graph
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

};


std::vector<int> findVetexCover(int num, int numVertexCover, std::vector<int> intVertices) {

    Minisat::Solver solver;
    std::vector <std::vector<Minisat::Lit>> matrix(num);
	std::vector<int> first;
    std::vector<int> last;

//creating literals
    for (int n = 0; n < num; ++n) 
	{
        for (int k = 0; k < numVertexCover; ++k) 
		{
            Minisat::Lit literal = Minisat::mkLit(solver.newVar());
            matrix[n].push_back(literal);
        }
    }

// 1 -> At least one vertex is the ith vertex in the vertex cover: ∀i ∈ [1,k], a clause (x1,i ∨x2,i ∨···∨xn,i)

    for (int k = 0; k < numVertexCover; ++k) 
	{
        Minisat::vec<Minisat::Lit> literals;
        for (int n = 0; n < num; ++n) 
		{ // n is changing first
            literals.push(matrix[n][k]);
        }
        solver.addClause(literals);
        literals.clear();
    }

// 2 ->  No one vertex can appear twice in a vertex cover: ∀m ∈ [1,n],∀p,q ∈ [1,k] with p < q,a clause (¬Xm,p ∨ ¬Xm,q)

    for (int m = 0; m < num; ++m) 
	{
        for (int p = 0; p < numVertexCover - 1; ++p) 
		{
            for (int q = p + 1; q < numVertexCover; ++q) 
			{
                solver.addClause(~matrix[m][p], ~matrix[m][q]); // use variables as mentioned in encoding to prevent confusion Cat!!!
            }
        }
    }

    // 3 -> No more than one vertex appears in the mth position of the vertex cover : ∀m ∈ [1,k],∀p,q ∈ [1,n] with p < q, a clause (¬Xp,m ∨ ¬Xq,m)

    for (int m = 0; m < numVertexCover; ++m) 
	{
        for (int p = 0; p < num - 1; ++p) 
		{
            for (int q = p + 1; q < num; ++q) 
			{
                solver.addClause(~matrix[p][m], ~matrix[q][m]);
            }
        }
    }



    for (int i = 0; i < intVertices.size(); ++i) 
	{
        if (i % 2 == 0) 
		{
            first.push_back(intVertices[i]);
        } else 
		{
            last.push_back(intVertices[i]);
        }
    }

    //Rule 4: every edge is incident to at least one vertex in the vertex cover

    for (int i = 0; i < first.size(); ++i) 
	{
        Minisat::vec<Minisat::Lit> literals;
        for (int k = 0; k < numVertexCover; ++k) 
		{
            literals.push(matrix[first[i]][k]);
            literals.push(matrix[last[i]][k]);
        }
        solver.addClause(literals);
        literals.clear();
    }

    bool sat = solver.solve();

    if (sat) 
	{
        std::vector<int> result;
        for (int i = 0; i < num; i++) 
		{
            for (int j = 0; j < numVertexCover; ++j) 
			{
                if (Minisat::toInt(solver.modelValue(matrix[i][j])) == 0) 
				{
                    result.push_back(i);
                }
            }
        }
        return result;
    } else {
        return {-1};
    }
}


std::vector<std::string> split(std::string str, std::string token){
    std::vector<std::string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos)
		{
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else
		{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void Create_graph(std::string allVertice, int num, Graph graph) {
	

    std::string edges;

    bool set_flag = true;
	std::string word;
    std::string out;
    std::istringstream splits(allVertice, std::istringstream::in);
	splits >> word ;
        int no = word.length();
        char arr [no+1];

        strcpy(arr, word.c_str());
        std::string x="",y="";
        for (int i=0;i<no;i++)
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

                 std::stringstream x_val(x);
                 std::stringstream y_val(y);
                 int x_value=0,y_value=0;
                 x_val>>x_value;
                 y_val>>y_value;
                
                 
                if(x_value>=num ||x_value<0 || y_value>=num ||y_value<0){
                    throw "Error: Invalid input ***my case\n";
				}
				graph.addEdge(x_value, y_value);
				    x="";
                    y="";
			}
		}
	//replace( allVertice.begin(), allVertice.end(), "<", ' ' );
	//replace( allVertice.begin(), allVertice.end(), ">", ' ' );
	//replace( allVertice.begin(), allVertice.end(), "{", ' ' );
	//replace( allVertice.begin(), allVertice.end(), "}", ' ' );
	//std::cout<< allVertice ;
    edges = replacestring( allVertice, "<", ' ' );
    edges = replacestring( edges, ">", ' ' );
    edges = replacestring( edges, "{", ' ' );
    edges = replacestring( edges, "}", ' ' );

    std::vector<std::string> vertices = split(edges, ","); // ==> ["0","1","2","3"]
    std::vector<int> intVertices;


    for (auto &s : vertices) {
		int x = 0;
        std::stringstream parse_vert(s);
        parse_vert >> x;
        intVertices.push_back(x);
    }


    for (unsigned index = 0; index < vertices.size(); ++index) 
	{
        if (index % 2 == 0) 
		{
            if ((std::stoi(vertices[index]) >= num) || (std::stoi(vertices[index + 1]) >= num)) 
			{
                set_flag = false;
				std::cerr << "Error: vertice ID is larger than the size of graph" << std::endl;
            } else {

				//graph.addEdge(x_value, y_value);
				continue;
            }
        } else {
            continue;
        }
    }


    if (set_flag) {

		int initial = 1;
		int finall = num;
        std::vector<int> result;
        std::vector<int> finalResult;
        std::vector<int> tmp;

        tmp = {-1};

        while (initial <= finall)
		{
            int mid = floor((initial + finall) / 2);
            result = findVetexCover(num, mid, intVertices);
            bool check = std::equal(result.begin(), result.end(), tmp.begin());

            if (not check) { 

                finall = mid - 1;
                finalResult.clear();
                finalResult = result;

            } else {
                initial = mid + 1;
            }
        }

        sort(finalResult.begin(), finalResult.end());

		for(int vertex2: finalResult){
            std::cout << vertex2 << " ";
        }
		std::cout<<"\n";
    }
}



int main() {

    Graph g(0);

    using Minisat::mkLit;

    using Minisat::lbool;

    int num = 0;
	std::string input;
	std::string data;



    while (true) {
		try
        {

			std::vector<std::string> inp;
			std::string input_string;
			std::getline(std::cin, input_string);
			//std::getline(std::cin, input);
			
			if (std::cin.eof())
            {
                break;
            }
			
			//data = splitInput(input);

			inp = split(input_string, " ");

			

				if (inp[0] == "V") {

					num = std::stoi(inp[1]);
					g = Graph(num);
				} else if (inp[0] == "E") {
					
					if (inp[1] == "{}")
					{
						std::cout<<"\n" << std::endl;
						continue;
					}
						
					Create_graph(inp[1], num, g);
					
				} else {
					std::cout << "Error: Invalid input!" << std::endl;
				}

			
		}
		catch(...)
        {
            std::cerr << "Error: Invalid input provided!." << std::endl;
        }


    }

    return 0;

}
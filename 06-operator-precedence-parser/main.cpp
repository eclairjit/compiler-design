#include<bits/stdc++.h>
using namespace std;

/* ---------- Global Variables ------------ */

int n;
vector<vector<int>> adj;
vector<bool> vis;
vector<int> maxVis;

/* ---------------------------------------- */

class NodeHandler {
    int counter;
    map<string, int> f, g;

    public:

        NodeHandler() {
            this -> counter = 0;
        }

        void makeNode(string sym) {
            this -> counter++;
            this -> f[sym] = this -> counter;

            this -> counter++;
            this -> g[sym] = this -> counter;
        }

        int getFNode(string sym) {
            return this -> f[sym];
        }

        int getGNode(string sym) {
            return this -> g[sym];
        }

        void mergeNodes(string sym1, string sym2) {
            // pehle ka f dusre ka g
            this -> g.erase(sym2);
            this -> g[sym2] = this -> f[sym1];
        }

        void print() {
            for(auto i : this -> f) {
                cout << i.first << " " << i.second << endl;
            }

            cout << endl;

            for(auto i : this -> g) {
                cout << i.first << " " << i.second << endl;
            }            
        }
};

// initiate node handler

NodeHandler nh;

void init() {
 
    vector<vector<string>> operationRelationTable {
        {"", "id", "+", "*", "$"},
        {"id", "=", ">", ">", ">"},
        {"+", "<", ">", "<", ">"},
        {"*", "<", ">", ">", ">"},
        {"$", "<", "<", "<", ">"}
    };

    n = 4;

    // initiate the adjacency list and other containers for graph traversal

    adj.resize(4*n+1);
    vis.resize(4*n+1, false);
    maxVis.resize(4*n+1, 0);

    // make all nodes
    for(int i=1; i<=n; i++) {
        // cout << operationRelationTable[0][i] << endl;
        nh.makeNode(operationRelationTable[0][i]);
    }

    // assigning common nodes
    for(int i=1; i<=n; i++) {
        for(int j=1; j<=n; j++) {

            if(operationRelationTable[i][j] == "=") {
                nh.mergeNodes(operationRelationTable[i][0], operationRelationTable[0][j]);
            }

        }
    }

    // making adjacency list
    for(int i=1; i<=n; i++) {
        for(int j=1; j<=n; j++) {

            if(operationRelationTable[i][j] == "=") continue;

            int u = nh.getFNode(operationRelationTable[i][0]);
            int v = nh.getGNode(operationRelationTable[0][j]);

            if(operationRelationTable[i][j] == ">") {
                adj[u].push_back(v);
            } else {
                adj[v].push_back(u);
            }

        }
    }

}

int calc(int node) {

    vis[node] = true;

    for(auto adjNode : adj[node]) {

        if(vis[adjNode]) {
            maxVis[node] = max(maxVis[node], 1 + maxVis[adjNode]);
        } else {
            maxVis[node] = max(maxVis[node], 1 + calc(adjNode));
        }

    }

    return maxVis[node];

}

int main() {

    init();

    for(int node=0; node<=4*n; node++) {
        if(!vis[node]) calc(node);
    }

    // nh.print();

    vector<string> ls {"id", "+", "*", "$"};

    cout << "\t";
    for(auto s : ls) cout << s << "\t";
    cout << endl;

    cout << "f\t";

    for(auto s : ls) {
        int node = nh.getFNode(s);
        cout << maxVis[node] << "\t";
    }

    cout << endl;

    cout << "g\t";

    for(auto s : ls) {
        int node = nh.getGNode(s);
        cout << maxVis[node] << "\t";
    }

    return 0;
}
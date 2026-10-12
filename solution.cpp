#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>

using namespace std;

class DSU {
private:
    vector<int> parent;
    vector<int> rank;
    
public:
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return;
        
        if (rank[px] < rank[py]) {
            parent[px] = py;
        } else if (rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
    }
};

struct Constraint {
    long long i, j;
    int e;
};

int main() {
    int t;
    scanf("%d", &t);
    
    while (t--) {
        int n;
        scanf("%d", &n);
        
        vector<Constraint> constraints;
        constraints.reserve(n);
        vector<long long> vars;
        vars.reserve(n * 2);
        
        // Read all constraints and collect variables
        for (int k = 0; k < n; k++) {
            long long i, j;
            int e;
            scanf("%lld %lld %d", &i, &j, &e);
            
            constraints.push_back({i, j, e});
            vars.push_back(i);
            vars.push_back(j);
        }
        
        // Coordinate compression
        sort(vars.begin(), vars.end());
        vars.erase(unique(vars.begin(), vars.end()), vars.end());
        
        map<long long, int> var_to_idx;
        for (int i = 0; i < (int)vars.size(); i++) {
            var_to_idx[vars[i]] = i;
        }
        
        DSU dsu(vars.size());
        
        // Process equality constraints first
        for (const auto& cons : constraints) {
            if (cons.e == 1) {
                int idx_i = var_to_idx[cons.i];
                int idx_j = var_to_idx[cons.j];
                dsu.unite(idx_i, idx_j);
            }
        }
        
        // Check inequality constraints for conflicts
        bool possible = true;
        for (const auto& cons : constraints) {
            if (cons.e == 0) {
                int idx_i = var_to_idx[cons.i];
                int idx_j = var_to_idx[cons.j];
                if (dsu.find(idx_i) == dsu.find(idx_j)) {
                    possible = false;
                    break;
                }
            }
        }
        
        printf("%s\n", possible ? "YES" : "NO");
    }
    
    return 0;
}
#include <iostream>
#include <vector>
#include <unordered_map>
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
    int i, j, e;
};

int main() {
    int t;
    scanf("%d", &t);
    
    while (t--) {
        int n;
        scanf("%d", &n);
        
        vector<Constraint> constraints;
        unordered_map<long long, int> var_to_idx;
        int idx_count = 0;
        
        // Read all constraints and map variables to indices
        for (int k = 0; k < n; k++) {
            long long i, j;
            int e;
            scanf("%lld %lld %d", &i, &j, &e);
            
            if (var_to_idx.find(i) == var_to_idx.end()) {
                var_to_idx[i] = idx_count++;
            }
            if (var_to_idx.find(j) == var_to_idx.end()) {
                var_to_idx[j] = idx_count++;
            }
            
            constraints.push_back({var_to_idx[i], var_to_idx[j], e});
        }
        
        DSU dsu(idx_count);
        
        // Process equality constraints first
        for (const auto& cons : constraints) {
            if (cons.e == 1) {
                dsu.unite(cons.i, cons.j);
            }
        }
        
        // Check inequality constraints for conflicts
        bool possible = true;
        for (const auto& cons : constraints) {
            if (cons.e == 0) {
                if (dsu.find(cons.i) == dsu.find(cons.j)) {
                    possible = false;
                    break;
                }
            }
        }
        
        printf("%s\n", possible ? "YES" : "NO");
    }
    
    return 0;
}
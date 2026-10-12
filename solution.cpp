#include <bits/stdc++.h>
using namespace std;

class DSU {
private:
    vector<int> parent;
    vector<int> rank;
    
public:
    DSU(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }
    
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    scanf("%d", &t);
    
    while (t--) {
        int n;
        scanf("%d", &n);
        
        vector<pair<long long, long long>> equality_constraints;
        vector<pair<long long, long long>> inequality_constraints;
        vector<long long> vars;
        
        equality_constraints.reserve(n);
        inequality_constraints.reserve(n);
        vars.reserve(n * 2);
        
        // Read all constraints and categorize them
        for (int k = 0; k < n; k++) {
            long long i, j;
            int e;
            scanf("%lld %lld %d", &i, &j, &e);
            
            vars.push_back(i);
            vars.push_back(j);
            
            if (e == 1) {
                equality_constraints.push_back({i, j});
            } else {
                inequality_constraints.push_back({i, j});
            }
        }
        
        // Coordinate compression
        sort(vars.begin(), vars.end());
        vars.erase(unique(vars.begin(), vars.end()), vars.end());
        
        unordered_map<long long, int> var_to_idx;
        var_to_idx.reserve(vars.size() * 2);
        for (int i = 0; i < (int)vars.size(); i++) {
            var_to_idx[vars[i]] = i;
        }
        
        DSU dsu(vars.size());
        
        // Process equality constraints
        for (const auto& cons : equality_constraints) {
            dsu.unite(var_to_idx[cons.first], var_to_idx[cons.second]);
        }
        
        // Check inequality constraints for conflicts
        bool possible = true;
        for (const auto& cons : inequality_constraints) {
            if (dsu.find(var_to_idx[cons.first]) == dsu.find(var_to_idx[cons.second])) {
                possible = false;
                break;
            }
        }
        
        printf("%s\n", possible ? "YES" : "NO");
    }
    
    return 0;
}
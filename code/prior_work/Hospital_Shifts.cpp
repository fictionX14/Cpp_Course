
// Same engine, different cover story, less hand-holding than last time.

// You're scheduling shifts for a hospital. There are n available workers. Each worker has a cost, 
// and a capability string: "10" = can do triage only, "01" = can do surgery only, "11" = can do both. You need at least k workers capable of triage 
// and at least k workers capable of surgery (the same "both" worker can count toward both totals). Return the minimum total cost, or -1 if impossible.

// Signature:

// int minCost(int n, vector<int> costs, vector<string> caps, int k);

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int minCost(int n, vector<int> costs, vector<string> caps, int k) {
    
    vector<int> triageOnly;
    vector<int> surgeryOnly;
    vector<int> both;
    // create buckets relating costs and caps 
    for (int i = 0; i < n; i++) { 
        if (caps[i] == "10") { 
            triageOnly.push_back(costs[i]); 
        } 
         if (caps[i] == "01") { 
            surgeryOnly.push_back(costs[i]); 
        } 
         if (caps[i] == "11") { 
            both.push_back(costs[i]); 
        } 
    }

    // sort each cost vector before prefix summing
    sort(triageOnly.begin(), triageOnly.end());
    sort(surgeryOnly.begin(), surgeryOnly.end());
    sort(both.begin(), both.end());

    // find prefixsums of each sorted vecror 
    vector<int> prefixTriage;
    if (!triageOnly.empty()){ 
    prefixTriage.push_back(triageOnly[0]);
    for (int i = 1; i < (int)triageOnly.size(); i++) { 
        prefixTriage.psuh_back(prefixTriage[i-1]+ triageOnly[i]);
    }
    }

    vector<int> prefixSurgery;
    if (!surgeryOnly.empty()){ 
    prefixSurgery.push_back(surgeryOnly[0]);
    for (int i = 1; i < (int)surgeryOnly.size(); i++) { 
        prefixSurgery.push_back(prefixSurgery[i-1]+ surgeryOnly[i]);
    }
    }

    vector<int> prefixBoth;
    if (!both.empty()){ 
    prefixBoth.push_back(both[0]);
    for (int i = 1; i < (int)both.size(); i++) { 
        prefixBoth.push_back(prefixBoth[i-1]+ both[i]);
    }
    }   

    // for each x through k, is there enough models to satisfy the conditions
    int ans = INT_MAX; 
    for (int x = 0; x <= k ; x++) { 
        int need = k-x;

        
        // skip if we don't have enough models in any bucket
        if (x    > (int)both.size())  continue;
        if (need > (int)triageOnly.size()) continue;
        if (need > (int)surgeryOnly.size()) continue;

        // cost of x cheapest "both" models
        // x==0 means we use none → cost is 0
        // otherwise prefixBoth[x-1] = sum of first x elements (index x-1)
        int costBoth = (x    == 0) ? 0 : prefixBoth[x - 1];

        // cost of "need" cheapest onlyA models
        // need==0 means we need none → cost is 0
        // otherwise prefixA[need-1] = sum of first "need" elements
        int costSurgery   = (need == 0) ? 0 : prefixSurgery[need - 1];

        // same logic for onlyB
        int costTriage    = (need == 0) ? 0 : prefixTriage[need - 1];

        // track the cheapest valid combination seen so far
        ans = min(ans, costBoth + costSurgery + costTriage);
    }

    // ── Step 5: return answer, or -1 if no valid combo exists ───────
    return (ans == INT_MAX) ? -1 : ans;

    }
    


int main() {
    int n = 4, k = 2;
    vector<int> costs    = {10, 12, 25, 8};
    vector<string> feats = {"10","01","11","10"};
    cout << minCost(n, costs, feats, k) << endl;
    // onlyA sorted = [8, 10], onlyB = [12], both = [25]
    // x=0: need=2, onlyA[0+1]=18, onlyB only has 1 → skip
    // x=1: need=1, both[0]=25, onlyA[0]=8, onlyB[0]=12 → 45
    // x=2: need=0, only 1 "both" model → skip
    // answer = 45
}
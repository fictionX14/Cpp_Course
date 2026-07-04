// You are given a list of AI models. Each model has:

// A cost (integer)
// A boolean: does it support feature A?
// A boolean: does it support feature B?
// Write a function that returns the minimum total cost 
// to have both feature A and feature B covered. You can buy one model that has both, or two separate models. If it's impossible, return -1


#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int minCost(int n, vector<int> costs, vector<string> features, int k) {
    
    // ── Step 1: separate models into three buckets ──────────────────
    vector<int> onlyA, onlyB, both;

    for (int i = 0; i < n; i++) {
        if (features[i] == "10") onlyA.push_back(costs[i]);
        if (features[i] == "01") onlyB.push_back(costs[i]);
        if (features[i] == "11") both.push_back(costs[i]);
    }

    // ── Step 2: sort cheapest first ─────────────────────────────────
    sort(onlyA.begin(), onlyA.end());
    sort(onlyB.begin(), onlyB.end());
    sort(both.begin(),  both.end());

    // ── Step 3: prefix sums ──────────────────────────────────────────
    // prefixA[i] = total cost of the cheapest (i+1) onlyA models
    // e.g. onlyA = [5, 8, 15] → prefixA = [5, 13, 28]
    // so "cheapest 2 onlyA models" = prefixA[1] = 13
    
    vector<int> prefixA, prefixB, prefixBoth;

    if (!onlyA.empty()) {
        prefixA.push_back(onlyA[0]);
        for (int i = 1; i < (int)onlyA.size(); i++)
            prefixA.push_back(prefixA[i-1] + onlyA[i]);
    }

    if (!onlyB.empty()) {
        prefixB.push_back(onlyB[0]);
        for (int i = 1; i < (int)onlyB.size(); i++)
            prefixB.push_back(prefixB[i-1] + onlyB[i]);
    }

    if (!both.empty()) {
        prefixBoth.push_back(both[0]);
        for (int i = 1; i < (int)both.size(); i++)
            prefixBoth.push_back(prefixBoth[i-1] + both[i]);
    }

    // ── Step 4: try every value of x (# of "both" models used) ──────
    // x=0 → use 0 "both" models, need k onlyA + k onlyB
    // x=1 → use 1 "both" model,  need k-1 onlyA + k-1 onlyB
    // x=k → use k "both" models, need 0 onlyA + 0 onlyB
    
    int ans = INT_MAX;

    for (int x = 0; x <= k; x++) {
        int need = k - x;   // how many onlyA and onlyB still needed

        // skip if we don't have enough models in any bucket
        if (x    > (int)both.size())  continue;
        if (need > (int)onlyA.size()) continue;
        if (need > (int)onlyB.size()) continue;

        // cost of x cheapest "both" models
        // x==0 means we use none → cost is 0
        // otherwise prefixBoth[x-1] = sum of first x elements (index x-1)
        int costBoth = (x    == 0) ? 0 : prefixBoth[x - 1];

        // cost of "need" cheapest onlyA models
        // need==0 means we need none → cost is 0
        // otherwise prefixA[need-1] = sum of first "need" elements
        int costA    = (need == 0) ? 0 : prefixA[need - 1];

        // same logic for onlyB
        int costB    = (need == 0) ? 0 : prefixB[need - 1];

        // track the cheapest valid combination seen so far
        ans = min(ans, costBoth + costA + costB);
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

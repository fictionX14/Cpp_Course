// Input:  [[1,3],[2,6],[8,10],[15,18]]
// Output: [[1,6],[8,10],[15,18]]
// Input:  [[1,4],[4,5]]
// Output: [[1,5]]
#include <vector>
#include <iostream>
using namespace std;

vector<vector<int>> merge(vector<vector<int>>& intervals) {

    vector<vector<int>> ans_intervals; 
    int num_ranges = (int)intervals.size();
    cout << num_ranges << endl;

    cout << intervals[0][0] << endl;
    cout << intervals[0][1] << endl;

    for(int x = 0; x<num_ranges; x++) {
        int y = x+1;
        if (y<num_ranges){
            if (intervals[x][1] > intervals[x+1][0] && intervals[x][1] < intervals[x+1][1]) {
                ans_intervals.push_back({intervals[x][0],intervals[x+1][1]});
                cout << ans_intervals[0][0] << ", " << ans_intervals[0][1] << endl;
            }
        }

    }

    return intervals;
}

int main() { 
    vector<vector<int>> Input =  {{1,3},{2,6},{8,10},{15,18}};
    merge(Input);

    return 0;
}
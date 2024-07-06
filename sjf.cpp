#include <bits/stdc++.h>

using namespace std;

typedef pair<long long, pair<long long, long long>> pii;
typedef pair<long long, long long> pi;

long long maxi(long long a,long long b)
{
    if(a>b) return a;
    else return b;
}

class Solution {
public:
    vector<int> getOrder(vector<vector<int>>& tasks) {
        vector<pii> v;
        for (int i = 0; i < tasks.size(); i++) {
            v.push_back({tasks[i][0], {tasks[i][1], i}});
        }
        sort(v.begin(), v.end());

        vector<int> ans;
        vector<long long> start_time(tasks.size());
        vector<long long> end_time(tasks.size());
        vector<long long> waiting_time(tasks.size());
        vector<string> cpu_behavior;

        long long time = 0;
        priority_queue<pii, vector<pii>, greater<pii>> pq;
        int n = v.size();
        int i = 0;

        while (i < n && v[i].first <= time) {
            pq.push({v[i].second.first, {v[i].second.second, v[i].first}});
            i++;
        }

        if (!pq.empty()) {
            time = v[0].first;
        } else if (i < n) {
            time = v[i].first;
        }

        while (!pq.empty() || i < n) {
            if (pq.empty() && i < n && time < v[i].first) {
                for (long long t = time; t < v[i].first; t++) {
                    cpu_behavior.push_back("idle");
                }
                time = v[i].first;
            }

            while (i < n && v[i].first <= time) {
                pq.push({v[i].second.first, {v[i].second.second, v[i].first}});
                i++;
            }

            if (!pq.empty()) {
                pii temp = pq.top();
                pq.pop();
                int idx = temp.second.first;
                ans.push_back(idx);
                start_time[idx] = time;
                waiting_time[idx] = maxi(0,time -temp.second.second);
                for (long long t = time; t < time + temp.first; t++) {
                    cpu_behavior.push_back("P" + to_string(idx+1));
                }
                time += temp.first;
                end_time[idx] = time;
            }
        }

        // Print waiting time, start and end timings
        for (int i = 0; i < ans.size(); i++) {
            int idx = ans[i];
            cout << "Process " << idx+1 << ": Waiting Time = " << waiting_time[idx] 
                 << ", Start Time = " << start_time[idx] << ", End Time = " << end_time[idx] << endl;
        }

        // Print CPU behavior
        cout << "CPU Behavior: ";
        for (const string& state : cpu_behavior) {
            cout << state << " ";
        }
        cout << endl;

        return ans;
    }
};

int main(int argc, char*argv[]) {
    Solution sol;

   cout<<argc<<endl;
    if (argc < 3) {
        cerr << "Error: Not enough arguments provided." << std::endl;
        return 1;
    }
    
    if((argc-1)%2==1)
    {
        cout<<"Please enter valid numver of process"<<endl;
        return 0;
    }
	// Number of process
    int n = (argc-1)/2;
    cout<<"TOtal number of process to be processed: "<< n;
    

    cout<<"Enter to SJF hooo  algo"<<endl;
    //lets make the priority queue -- as user is not going to give input on the basis of sorted order

    vector<vector<int>> inp;

    for(int i = 1 ; i < argc ; i+=2){
       
        inp.push_back({stoi(argv[i]),stoi(argv[i+1])});
    }

    sol.getOrder(inp);
    return 0;
}

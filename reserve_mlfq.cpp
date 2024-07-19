#include <bits/stdc++.h>
using namespace std;

struct process {
    float at, bt, st, bt_left;
    int wt, ft, tat, no;
    // waiting time will keep track of waiting in each queue
};

class Solution {
public:
    static bool compare(const process &p1, const process &p2) {
        return p1.at < p2.at;
    }
    
    void getOrder() {   
        int n;

        cout << "Enter the Number of the Processes: " << endl;
        cin >> n;

        vector<process> p(n);

        cout << "Enter the Arrival time of the processes: " << endl;
        for (int i = 0; i < n; i++) {
            cin >> p[i].at;
            p[i].no = i;
        }
       
        cout << "Enter the Burst time of the processes: " << endl;
        for (int i = 0; i < n; i++) {
            cin >> p[i].bt;
            p[i].bt_left = p[i].bt;
        }

        queue<process> q1, q2, q3;

        float quanta1 = 2;
        float quanta2 = 4;

        sort(p.begin(), p.end(), compare);

        for (auto &it: p) {
            q1.push(it);    
        }

        float time = 0;

        // Round Robin with time quanta quanta1
        while (!q1.empty()) {
            process temp = q1.front();
            q1.pop();
            p[temp.no].st = time;

            if (temp.bt_left <= quanta1) {
                p[temp.no].ft = time + temp.bt_left;
                time += temp.bt_left;
                temp.bt_left = 0;
            } else {
                temp.bt_left -= quanta1;
                time += quanta1;
                q2.push(temp);
            }
        }

        // Round Robin with time quanta quanta2 
        while (!q2.empty()) {
            process temp = q2.front();
            q2.pop();

            if (temp.bt_left <= quanta2) {
                p[temp.no].ft = time + temp.bt_left;
                time += temp.bt_left;
                temp.bt_left = 0;
            } else {
                temp.bt_left -= quanta2;
                time += quanta2;
                q3.push(temp);
            }
        }

        // FCFS for the remaining processes
        while (!q3.empty()) {
            process temp = q3.front();
            q3.pop();

            p[temp.no].ft = time + temp.bt_left;
            time += temp.bt_left;
            temp.bt_left = 0;
        }

        // Calculate the waiting time and turnaround time
        float total_wt = 0;
        float total_tat = 0;

        for (auto &it : p) {
            it.tat = it.ft - it.at;
            it.wt = it.tat - it.bt;
            total_wt += it.wt;
            total_tat += it.tat;
        }

        cout << "Process no\tArrival Time\tBurst Time\tStarting time\tFinishing time\tWaiting time\tTurnaround Time" << endl;
        for (int i = 0; i < n; i++) {
            cout << p[i].no << "\t\t" << p[i].at << "\t\t" << p[i].bt << "\t\t" << p[i].st << "\t\t" << p[i].ft << "\t\t" << p[i].wt << "\t\t" << p[i].tat << endl;
        }
  
        cout << "Average waiting time: " << total_wt / n << endl;
        cout << "Average Turnaround time: " << total_tat / n << endl;
    }
};

int main() {
    Solution sol;
    sol.getOrder();
    return 0;
}

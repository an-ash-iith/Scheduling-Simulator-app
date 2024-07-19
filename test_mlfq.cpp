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
        float leftquanta2 = 4; // it will keep the track record of quanta left for the preempted process in q2

        sort(p.begin(), p.end(), compare);

        // next will keep track of the next process arrival time
        float time = 0;
        int left_process = n;
        float next = INT_MAX;

        int enter = 0; // will keep track record of the process that has been entered

        bool flag2 = false; // flag for 2nd queue
        bool flag3 = false; // flag for 3rd queue

        while (left_process > 0) {

            // Round Robin with time quanta quanta1
            while (!q1.empty()) {
                process temp = q1.front();
                q1.pop(); // move pop outside the conditional

                p[temp.no].st = time;

                if (temp.bt_left <= quanta1) {
                    p[temp.no].ft = time + temp.bt_left;
                    time += temp.bt_left;
                    left_process--;
                    temp.bt_left = 0;
                } else {
                    temp.bt_left -= quanta1;
                    time += quanta1;
                    q2.push(temp);
                }

                flag2 = true;
            }

            if (time >= next) { // checking that if any process have come till now
                q1.push(p[enter]);
                enter++;
                if (enter < n) {
                    next = p[enter].at;
                } else {
                    next = INT_MAX;
                }

                flag2 = false;
                flag3 = false;
            } else {
                if (q1.empty() && q2.empty() && q3.empty() && enter != n) {
                    time = p[enter].at;
                    q1.push(p[enter]);
                    enter++;
                    if (enter < n) {
                        next = p[enter].at;
                    } else {
                        next = INT_MAX;
                    }
                }
            }

            // Round Robin with time quanta quanta2
            while (!q2.empty() && flag2) {
                process temp = q2.front();
                q2.pop(); // move pop outside the conditional

                if (next >= time + leftquanta2 || next >= time + temp.bt_left) {
                    if (temp.bt_left <= leftquanta2) {
                        p[temp.no].ft = time + temp.bt_left;
                        time += temp.bt_left;
                        temp.bt_left = 0;
                        left_process--;
                    } else {
                        temp.bt_left -= leftquanta2;
                        time += leftquanta2;
                        q3.push(temp);
                    }
                    leftquanta2 = quanta2;
                    flag3 = true;
                } else {
                    temp.bt_left -= next - time;
                    leftquanta2 -= (next - time); // correct the calculation of leftquanta2
                    time = next;
                    q2.push(temp); // re-push the process
                    flag2 = false;
                }
            }

            // FCFS for the remaining processes
            while (!q3.empty() && flag3) {
                process temp = q3.front();
                q3.pop(); // move pop outside the conditional

                if (next >= time + temp.bt_left) {
                    p[temp.no].ft = time + temp.bt_left;
                    time += temp.bt_left;
                    temp.bt_left = 0;
                    left_process--;
                } else {
                    temp.bt_left -= next - time;
                    time += next - time;
                    q3.push(temp); // re-push the process
                    flag3 = false;
                }
            }
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

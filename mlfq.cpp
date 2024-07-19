
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

    void getOrder(int n, float quanta1,float quanta2, vector<process>& p) {
      
        vector<process> p1;

        queue<process> q1, q2, q3;

        float leftquanta2 = 5; // it will keep the track record of quanta left for the preempted process in q2

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

                temp.st = time;

                if (temp.bt_left <= quanta1) {
                    temp.ft = time + temp.bt_left;
                    time += temp.bt_left;
                    left_process--;
                    temp.bt_left = 0;
                    p1.push_back(temp);
                } else {
                    temp.bt_left -= quanta1;
                    time += quanta1;
                    q2.push(temp);

                }
                cout<<"P"<<temp.no<<" ";

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
                    
                    time=p[enter].at;
                    // cout<<enter<<" enter " << p[enter].at<<endl;
                    q1.push(p[enter]);
                    enter++;

                    if (enter < n) {
                    next = p[enter].at;

                   flag2 = false;
                   flag3 = false;
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
                        temp.ft = time + temp.bt_left;
                        time += temp.bt_left;
                        temp.bt_left = 0;
                        left_process--;
                        p1.push_back(temp);
                    } else {
                        temp.bt_left -= leftquanta2;
                        time += leftquanta2;
                        q3.push(temp);
                    }
                    leftquanta2 = quanta2;
                    flag3 = true;
                } else {
                    temp.bt_left -= next - time;
                    time += next - time;
                    leftquanta2 -= (next - time); // correct the calculation of leftquanta2
                    q2.push(temp); // re-push the process
                    flag2 = false;
                }
                cout<<"P"<<temp.no<<" ";
            }

            // FCFS for the remaining processes
            while (!q3.empty() && flag3) {
                process temp = q3.front();
                q3.pop(); // move pop outside the conditional

                if (next >= time + temp.bt_left) {
                    temp.ft = time + temp.bt_left;
                    time += temp.bt_left;
                    temp.bt_left = 0;
                    left_process--;
                    p1.push_back(temp);
                } else {
                    temp.bt_left -= next - time;
                    time += next - time;
                    q3.push(temp); // re-push the process
                    flag3 = false;
                }
                cout<<"P"<<temp.no<<" ";
            }
        }

        // Calculate the waiting time and turnaround time
        float total_wt = 0;
        float total_tat = 0;

        for (auto &it : p1) {

            it.tat = it.ft - it.at;
            it.wt = it.tat - it.bt;
            total_wt += it.wt;
            total_tat += it.tat;
        }
        
        cout << endl<< "Process no\tArrival Time\tBurst Time\tStarting time\tFinishing time\tWaiting time\tTurnaround Time" << endl;
        for (int i = 0; i < n; i++) {
            cout << p1[i].no << "\t\t" << p1[i].at << "\t\t" << p1[i].bt << "\t\t" << p1[i].st << "\t\t" << p1[i].ft << "\t\t" << p1[i].wt << "\t\t" << p1[i].tat << endl;
        }

        cout << "Average waiting time: " << total_wt / n << endl;
        cout << "Average Turnaround time: " << total_tat / n << endl;
    }
};

int main(int argc,char *argv[]) {

      if (argc < 5 || (argc - 1) % 2 != 0) {
        cerr << "Error: Invalid number of arguments provided." << endl;
        return 1;
    }
    cout<<"This the argument count "<<argc<<endl;

    int n = (argc - 3) / 2; // Number of processes
    cout<<"Number of the process: "<<n<<endl;
    float quanta1 = stof(argv[1]); // quanta 1
    float quanta2 = stof(argv[2]); // quanta 2

    cout<<"Quanta: "<<quanta1 <<" "<<quanta2<<" "<<endl;
  
    vector<process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].at = stof(argv[3 +  2*i]);
        p[i].bt = stof(argv[4 +  2*i]);
        p[i].no = i + 1;
        p[i].bt_left = p[i].bt;
    }

    Solution sol;
    sol.getOrder(n,quanta1,quanta2,p);
    return 0;
}

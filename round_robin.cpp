#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct process {
    float at, bt;
    int st[1000]; // this will keep the track record for the given
    // process and its starting time
    // here we are taking size = 1000 because assuming that this is going to
    // be done 1000 rounds
    int wt, ft, tat, no;
};

class Solution {
public:
    void getOrder(int n, int slice, vector<process>& p) {
        vector<float> arr(n);
        vector<float> burst(n);

        for (int i = 0; i < n; i++) {
            arr[i] = p[i].at;
        }

        for (int i = 0; i < n; i++) {
            burst[i] = p[i].bt;
        }

        int left_process = n;

        vector<vector<int>> start(n, vector<int>(1000, -1)); // this will keep track record of which process and in which turn what is its starting time

        bool flag;
        float time = 0;
        float mini; // smallest arrival time of the available process --

        int next = 0;

        while (left_process) {
            flag = false; // for further
            mini = INT_MAX;

            // go and choose the process which has the least starting time
            // and still has some burst time left
            for (int i = 0; i < n; i++) {
                // adding 0.1 because we have added 0.1 to the process coming at this time
                if (arr[i] <= time + 0.1 && mini > arr[i] && burst[i] > 0) {
                    next = i;
                    mini = arr[i];
                    flag = true; // will be helpful when no process is there at that time
                }
            }

            if (!flag) { // no process is there
                time++;
                cout << "Idle" << " ";
                continue; // just surpass
            }

            cout << p[next].no << " ";

            int j = 0;
            while (start[next][j] != -1) {
                j++;
            }

            if (start[next][j] == -1) {
                start[next][j] = time;
                p[next].st[j] = time; // you can update it at last
            }

            if (burst[next] >= slice) {
                time += slice;
                burst[next] -= slice;
            } else {
                time += burst[next];
                burst[next] = 0;
            }

            if (burst[next] > 0) {
                arr[next] = time + 0.1;
            }

            if (burst[next] == 0) {
                left_process--;
                p[next].ft = time;
                p[next].wt = p[next].ft - p[next].at - p[next].bt;
                p[next].tat = p[next].ft - p[next].at;
            }
        }

        cout << endl;
        cout << "Process no" << "\t" << "Arrival Time" << "\t" << "Burst Time" << "\t" << "Starting time" << "\t" << "Finishing time" << "\t" << "Waiting time" << "\t" << "Turnaround Time" << "\t" << endl;

        for (int i = 0; i < n; i++) {
            cout << p[i].no << "\t\t" << p[i].at << "\t\t" << p[i].bt << "\t\t" << p[i].st[0] << "\t\t" << p[i].ft << "\t\t" << p[i].wt << "\t\t" << p[i].tat << "\t\t" << endl;
        }

        float waiting_time = 0;
        float turnaround_time = 0;

        for (int i = 0; i < n; i++) {
            waiting_time += p[i].wt;
            turnaround_time += p[i].tat;
        }

        cout << "Average waiting time " << waiting_time / n << endl;
        cout << "Average Turnaround time " << turnaround_time / n << endl;

        return;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4 || (argc - 2) % 2 != 0) {
        cerr << "Error: Invalid number of arguments provided." << endl;
        return 1;
    }

    int n = (argc - 2) / 2; // Number of processes
    int slice = stoi(argv[1]); // Time slice

    cout<<"Time slice " << slice<<endl;

    vector<process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].at = stof(argv[2 +  2*i]);
        p[i].bt = stof(argv[3 +  2*i]);
        p[i].no = i + 1;
    }

    Solution sol;
    sol.getOrder(n, slice, p);

    return 0;
}




#include <bits/stdc++.h>
using namespace std;

// Custom comparator for the priority queue
struct Compare {
    bool operator()(const pair<pair<int, int>, int>& a, const pair<pair<int, int>, int>& b){
        return a.first.second > b.first.second; // Compare based on burst time
    }
};

// Function to simulate the task scheduling
void getOrder(vector<pair<pair<int , int> , int >> &tasks) {
    // Priority queue to process tasks based on burst time
    priority_queue<pair<pair<int, int>, int>, vector<pair<pair<int, int>, int>>, Compare> pq;
    
    // Sort tasks based on arrival time
    sort(tasks.begin(), tasks.end());
    
    int index = 0;
    int time = 0;

    // Main loop to process tasks
    while(!pq.empty() || index < tasks.size()){

        // If the queue is empty, move the current time to the next task's arrival time
        if(pq.empty() && time < tasks[index].first.first){
            time = tasks[index].first.first;
        }

        // Enqueue all tasks that are ready to be processed
        while(index < tasks.size() && tasks[index].first.first <= time){
            pq.push({{tasks[index].first.first, tasks[index].first.second}, tasks[index].second});
            index++;
        }

        // Process the task with the shortest burst time
        if(!pq.empty()){
            auto currentTask = pq.top();
            pq.pop();
            cout << "Processing task " << currentTask.second + 1 <<endl;

            int timenext;
            if(index < tasks.size()) {
                timenext = tasks[index].first.first;
            } 
            else{
                timenext = INT_MAX;
            }

            int tame = min(timenext , time + currentTask.first.second);
            int gap = time + currentTask.first.second - tame;
            time = tame;
            currentTask.first.second = gap;

            // cout << " to " << time << endl;

            // If the current task is not yet finished, put it back into the queue
            if(currentTask.first.second > 0) {
                pq.push(currentTask);
            }
        }
    }   
}

int main(){
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<pair<pair<int , int> , int>> tasks;
    
    for(int i = 0; i < n; i++){
        int at, bt; 
        cout << "Enter arrival time of " << i + 1 << " : ";
        cin >> at;
        cout << "Enter burst time of " << i + 1 << " : ";
        cin >> bt;
        tasks.push_back({{at , bt}, i});
    } 
    getOrder(tasks);
    return 0;
}
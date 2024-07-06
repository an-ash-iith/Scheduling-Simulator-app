// C++ program to Calculate Waiting
// Time for given Processes
#include <bits/stdc++.h>
using namespace std;

typedef pair<int,pair<int,int>> pi;

// Function to Calculate start time end time waiting time and average waiting time
void SJFschedule(int at[] , int bt[] , int N)
{
    // Declare the array for waiting time
	int st[N];
    st[0] = at[0];
    int et[N];
    et[0] = st[0] + bt[0]-1;
	// Declare the array for waiting time
	int wt[N];
    wt[0] = 0;

    for(int i = 1 ; i < N ; i++){
        st[i] = max(at[i] , st[i-1] + bt[i-1]);
        et[i] = st[i] + bt[i] -1;
        wt[i] = st[i] - at[i];
    }

    // Print waiting time process 1
	    cout << "PN\t\tAT\t\tBT\t\tst\t\tet\t\twt\n";
    for(int i = 0 ; i < N ; i++){
	    cout <<i+1<<"\t\t"<<at[i]<<"\t\t"<<bt[i]<<"\t\t"<<st[i]<<"\t\t"<<et[i]<<"\t\t"<<wt[i]<<endl;
    }
 
    
	// Declare variable to calculate
	// average
	float average;
	float sum = 0;

	// Loop to calculate sum of all
	// waiting time
	for (int i = 0; i < N; i++) {
		sum = sum + wt[i];
	}

	// by dividing it by no. of process
	average = sum / N;

	// Print Average Waiting Time
	cout << "\nAverage waiting time = "
		<< average;
    
    for(int j = 0 ; j < st[0] ; j++){
        cout<<"ideal"<<endl;
    }
    for(int i = 0 ; i < (et[0] - st[0] + 1) ; i++){
        cout<<"P1"<<endl;
    }
    for(int i = 0 ; i < N-1 ; i++){
        int k = st[i+1] - et[i] +1; 
        for(int j = 0 ; j < k ; j++){
            cout<<"ideal"<<endl;
        }
        int times = et[i] - st[i] + 1;
        for(int j = 0 ;j < times ; j++){
            cout<<"P"<<i+1<<endl;
        }
    }

}

// Driver code
int main()
{
	// Number of process
	int n;
    cout<<"TOtal number of process to be processed: ";
    cin>>n;
    
//lets make the priority queue -- as user is not going to give input on the basis of sorted order

  vector<pi> inp;

  int a ,b;

    for(int i = 0 ; i < n ; i++){
        cout<<"Arrival & End time for P"<<i+1<<" : ";
        cin>>a >>b;

        inp.push_back({a,{b,i}});
    }

    sort(inp.begin(),inp.end());


for (int i = 0; i < n; i++)
{
   cout<<inp[i].first<<" "<<inp[i].second.first<<" "<<inp[i].second.second<<endl;
}

 
	// Function call to find waiting time
	// SJFschedule(at, bt, n);
	return 0;
}
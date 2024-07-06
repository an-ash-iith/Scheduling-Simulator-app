// C++ program to Calculate Waiting
// Time for given Processes
#include <bits/stdc++.h>
using namespace std;

// typedef pair<int,pair<int,int>> pi;

// Function to Calculate start time end time waiting time and average waiting time
void SJFschedule(vector<pair<int,pair<int,int>>> inp , int N)
{
    //Declare the array for waiting time
	int st[N];
    st[0] = inp[0].first;
    int et[N];
    et[0] = st[0] + inp[0].second.first-1;
	// Declare the array for waiting time
	int wt[N];
    wt[0] = 0;

    for(int i = 1 ; i < N ; i++){
        st[i] = max(inp[i].first , st[i-1] + inp[i-1].second.first);
        et[i] = st[i] + inp[i].second.first -1;
        wt[i] = st[i] - inp[i].first;
    }

    // Print waiting time process 1
	    cout << "PN\t\tAT\t\tBT\t\tst\t\tet\t\twt\n";
    for(int i = 0 ; i < N ; i++){
	    cout <<inp[i].second.second<<"\t\t"<<inp[i].first<<"\t\t"<<inp[i].second.first<<"\t\t"<<st[i]<<"\t\t"<<et[i]<<"\t\t"<<wt[i]<<endl;
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
	cout << "\nAverage waiting time = "<< average<<endl;
    
    for(int j = 0 ; j < st[0] ; j++){
        cout<<"idle"<<" ";
    }
    for(int i = 0 ; i < (et[0] - st[0] + 1) ; i++){
        cout<<"P"<<inp[0].second.second<<" ";
    }
    for(int i = 0 ; i < N-1 ; i++){
        int k = st[i+1] - et[i] -1; 
        for(int j = 0 ; j < k ; j++){
            cout<<"idle"<<" ";
        }
        int times = et[i+1] - st[i+1] + 1;
        for(int j = 0 ;j < times ; j++){
            cout<<"P"<<inp[i+1].second.second<<" ";
        }
    }

}

// Driver code
int main(int argc,char* argv[])
{

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
    

    
    //lets make the priority queue -- as user is not going to give input on the basis of sorted order

    vector<pair<int,pair<int,int>>> inp;

    int a ,b;
     
    

    for(int i = 1 ; i < argc ; i+=2){
       
        inp.push_back({stoi(argv[i]),{stoi(argv[i+1]),(i+1)/2}});
    }

    sort(inp.begin(),inp.end());

    for (int i = 0; i < n; i++){
       cout<<inp[i].first<<" "<<inp[i].second.first<<" "<<inp[i].second.second<<endl;
    }
 
	// Function call to find waiting time
	SJFschedule(inp,  n);
	return 0;
}
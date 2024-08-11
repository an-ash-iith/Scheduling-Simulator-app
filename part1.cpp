#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ll long long
using namespace std;
const int mod2 = 1e9 + 7;

typedef struct // process control block  --
{
   int pid, priority;
   double at, bt, tat, wt, left_bt, ft;
   // at- arrival time   bt-burst time  tat - turnaround time  wt = waiting time  left_bt = left burst time  ft = finish time

} pcb;

class Bitmap
{

   vector<bool> bitmap;

public:
   Bitmap(int size) : bitmap(size, false) {}

   void set(int index)
   {
      if (index < bitmap.size())
      {
         bitmap[index] = true;
      }
   }

   void clear(int index)
   {
      if (index < bitmap.size())
      {
         bitmap[index] = false;
      }
   }

   bool get(int index) const
   {
      if (index < bitmap.size())
      {
         return bitmap[index];
      }
      return false;
   }

   int size() const
   {
      return bitmap.size();
   }

   int next_set_bit(int start) const
   {
      for (int i = start; i < bitmap.size(); ++i)
      {
         if (bitmap[i])
         {
            return i;
         }
      }
      return bitmap.size(); // Returning the size if no set bit is found
   }
};

// global variables ---
atomic<double> curr_time(0);
int curr_queue_no = 0;
Bitmap bm(40); // if 1 then that queue is not empty else that queue is empty ---
int turn = 0;  // initially first queue will be active --

vector<vector<queue<pcb>>> que(40, vector<queue<pcb>>(2));
;                                                   //
pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER; // Mutex for the queue

// turn will have value of 0 or 1  -- turn =0 will denote first one is active and other one is expired --
//  the above two queue should be shared between two process as updation
// lt_scheduler parallely keep pushing the arrived proceess ---
// as both thread have write access it may seem that what main thread going to write will be actually written
// well before by this thread  -- first thread is popping from the from and second thread is pusing from the back , so no race condition --
// but suppose when second thread is pushing first element and main thread is trying to get it -- then it will in truble at that time  ---
// it should also be noted that time is the share variable so this also need to the race condiotn

// this will be the reference time for the lt_scheduler funtion to push the process or not ---
void *lt_scheduler(void *args)
{
   vector<pcb> *process = (vector<pcb> *)args;

   int n = process->size();
   int process_count = 0;

   while (true && process_count < n)
   {
      double arr_t = (*process)[process_count].at;
      while (curr_time >= arr_t)
      {
         int priority = (*process)[process_count].priority;

         // kept lock to avoid this --

         // if you have already surpass the priority of incoming process then push to the current expired queue as
         // it will go there and run next time else pust into this active one only ---

         if (curr_queue_no > priority)
         {
            que[!turn][priority].push((*process)[process_count]);
         }
         else
         {
            pthread_mutex_lock(&q_lock);
            que[turn][priority].push((*process)[process_count]);
            pthread_mutex_unlock(&q_lock);
         }

         bm.set(priority); // setting that priority bit ---

         process_count++;
      }
   }

   return NULL;
}

// compare to sort according to arrival time --

static bool compare(pcb &p1, pcb &p2)
{
   if (p1.at == p2.at)
   {
      return p1.priority < p2.priority;
   }
   else
   {
      return p1.at < p2.at;
   }
}

void set_timeslice(vector<double> &time_quanta)
{
   for (int priority = 0; priority < 40; priority++)
   {
      if (priority < 20)
      {
         time_quanta[priority] = (40 - priority) * 20; // millisecond
      }
      else
      {
         time_quanta[priority] = (40 - priority) * 5; // millisecond
      }
   }
}
int main()
{

   // as there is going to have 40 priority ---- 100 to 139

   cout<<"Enter the number of process "<<endl;

   int n; // number of process
   cin>>n;

   // create one thread to distribute present process into these queues --
 
   vector<pcb> process(n);
   vector<pcb> completed;
   
   cout<<"Enter the pocess detail - pid - arrival time - burst time priority (100 -139)  "<<endl;

   for (int i = 0; i < n; i++)
   {
      cin >> process[i].pid >> process[i].at >> process[i].bt >> process[i].priority;

      if(process[i].priority <100 || process[i].priority > 139) 
      { 
         cerr<<"Normal process priority out of range "<<endl;
         return -1;
      }

      process[i].priority-=100;  //adjust the priority for the furthur calculation -- 
      
      process[i].left_bt = process[i].bt;
   }

   sort(process.begin(), process.end(), compare); // sort on the basis of the arrival time ----

   pthread_t tid;

   // by other thread should reflect here also  ----

   // lets decide the dynamich time slice for each of the priority --

   vector<double> time_quanta(40);

   set_timeslice(time_quanta);

   pthread_create(&tid, NULL, lt_scheduler, &process);

   int executed = 0;
   int turn = 0;
   int index = 0;

   while (executed < n)
   {
      int index = bm.next_set_bit(index);

      if (index == 40) // already you executed all the queue --
      {
         turn = !turn;
      }
      else
      {

         // execute all the process of that queue --

         que[curr_queue_no][turn];

         while (!que[curr_queue_no][turn].empty())
         {
            pthread_mutex_lock(&q_lock);

            pcb curr_process = que[curr_queue_no][turn].front();
            que[curr_queue_no][turn].pop();

            pthread_mutex_unlock(&q_lock);

            curr_time = curr_time + min(curr_process.left_bt, time_quanta[curr_queue_no]);

            if (curr_process.left_bt <= time_quanta[curr_queue_no])
            {
               curr_process.left_bt = 0;
               curr_process.ft = curr_time;
               completed.push_back(curr_process);
               executed++;
            }
            else
            {
               curr_process.left_bt -= time_quanta[curr_queue_no];
               que[curr_queue_no][!turn].push(curr_process);
            }

            if (que[curr_queue_no][turn].empty())
            {
               sleep(0.1); // wait so that this queue should be updated  --
            }
         }
      }
   }

   pthread_join(tid, NULL);

   double total_wt = 0;
   double total_tat = 0;

   for (int i = 0; i < n; i++)
   {
      completed[i].tat = completed[i].ft - completed[i].at;
      completed[i].wt = completed[i].tat - completed[i].bt;

      total_wt += completed[i].wt;
      total_tat += completed[i].tat;
   }

   cout << "Process no\tArrival Time\tBurst Time\tStarting time\tFinishing time\tWaiting time\tTurnaround Time" << endl;
   for (int i = 0; i < n; i++)
   {
      cout << completed[i].pid << "\t\t" << completed[i].at << "\t\t" << completed[i].bt << "\t\t" << completed[i].ft << "\t\t" << completed[i].wt << "\t\t" << completed[i].tat << endl;
   }

   cout << "Average waiting time: " << total_wt / n << endl;
   cout << "Average Turnaround time: " << total_tat / n << endl;

   return 0;
}
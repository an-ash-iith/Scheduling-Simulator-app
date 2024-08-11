#include <bits/stdc++.h>
#include <pthread.h>
#include <asm/tlb.h>
#include <asm/unistd.h>

#define ll long long
using namespace std;
const int mod2 = 1e9+7; 

#define MAX_USER_PRIO	100
#define MAX_PRIO		(MAX_USER_PRIO + 40)
#define HZ 100 // HZ: This represents the number of clock ticks per second (or jiffies per second)
// If HZ = 100, one jiffy represents 10 milliseconds (ms).
// If HZ = 1000, one jiffy represents 1 millisecond (ms).
/*
why min time slice is needed?
if time slice is much smaller then context switch time will be around the time slice leading to 
unproductive waste of the cpu cycle 
*/
#define MIN_TIMESLICE		max(5 * HZ / 1000, 1) //this is the minimum time slice i will be gining to a procss
//in linux scheduler 2.6 , HZ is fixed at 100 -- but can be decide by the user if you want to bring some variation 
#define DEF_TIMESLICE		(100 * HZ / 1000)  //defines the default time slice (in jiffies) allocated to a process.

//relation between nice value and priority --
//high priority means less nice -- 
//140 value of priority have 20 as nice value -- 120 have 0 as nice vaue and 100 have -20 

#define NICE_TO_PRIO(nice)	(MAX_PRIO + (nice)- 20)
#define PRIO_TO_NICE(prio)	((prio) - MAX_PRIO+ 20)
#define USER_PRIO(p)		((p)-MAX_USER_PRIO)  // noting but actual priority - 100 -- 


/*
 * Some helpers for converting nanosecond timing to jiffy resolution
  here 1 jiffy is 10^-2 sec or 10 ms  --- here jiffy denots clocks -- jiffy per second means clocks per second 
 */
#define NS_TO_JIFFIES(TIME)	((TIME) / (1000000000 / HZ))
#define JIFFIES_TO_NS(TIME)	((TIME) * (1000000000 / HZ))

/*
 * task_timeslice() scales user-nice values [ -20 ... 0 ... 19 ]
 * to time slice values: [800ms ... 100ms ... 5ms]
 *
 * The higher a thread's priority, the bigger timeslices
 * it gets during one round of execution. But even the lowest
 * priority thread gets MIN_TIMESLICE worth of execution time.
 */

//  '\' means macro definition continue on the second line  -- 
#define SCALE_PRIO(x, prio) \
	max(x * (MAX_PRIO - prio) / (MAX_USER_PRIO / 2), MIN_TIMESLICE)

static unsigned int static_prio_timeslice(int static_prio)
{
	if (static_prio < NICE_TO_PRIO(0))  //here will have value of 120
		return SCALE_PRIO(DEF_TIMESLICE * 4, static_prio);  //low priority value will get more time 
	else
		return SCALE_PRIO(DEF_TIMESLICE, static_prio);
}


static inline unsigned int task_timeslice(struct task_struct *p)
{
	return static_prio_timeslice(p->static_prio);
}



/*
 * These are the runqueue data structures:
 */

struct prio_array {
	unsigned int nr_active;
	DECLARE_BITMAP(bitmap, MAX_PRIO+1); /* include 1 bit for delimiter */
	struct list_head queue[MAX_PRIO];
};


/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
struct rq {
	pthread_spinlock_t lock;   
   //note linux have its own lock written -- but i am using posix thread spinlock_t 
	/*
	 * nr_running and cpu_load should be in the same cacheline because
	 * remote CPUs use both these fields when doing load calculation.
	 */
	unsigned long nr_running;
	unsigned long raw_weighted_load;
#ifdef CONFIG_SMP
	unsigned long cpu_load[3];
	unsigned char idle_at_tick;
#ifdef CONFIG_NO_HZ
	unsigned char in_nohz_recently;
#endif
#endif
	unsigned long long nr_switches;

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */

	unsigned long nr_uninterruptible;
	unsigned long expired_timestamp;
	/* Cached timestamp set by update_cpu_clock() */
	unsigned long long most_recent_timestamp;
	struct task_struct *curr, *idle;
	unsigned long next_balance;
	struct mm_struct *prev_mm;
	struct prio_array *active, *expired, arrays[2];
	int best_expired_prio;
	atomic_ulong nr_iowait;   // number of tasks (processes or threads) currently waiting for I/O (Input/Output) operations to complete

};


/*
 * Adding/removing a task to/from a priority array:
 */
static void dequeue_task(struct task_struct *p, struct prio_array *array)
{
	array->nr_active--; //reducing the process which are active
	list_del(&p->run_list);
	if (list_empty(array->queue + p->prio))
		__clear_bit(p->prio, array->bitmap);
}

static void enqueue_task(struct task_struct *p, struct prio_array *array)
{
	sched_info_queued(p);
	list_add_tail(&p->run_list, array->queue + p->prio);
	__set_bit(p->prio, array->bitmap);
	array->nr_active++;
	p->array = array;
}

/*
 * Put task to the end of the run list without the overhead of dequeue
 * followed by enqueue.
 */
static void requeue_task(struct task_struct *p, struct prio_array *array)
{
	list_move_tail(&p->run_list, array->queue + p->prio);
}

static inline void
enqueue_task_head(struct task_struct *p, struct prio_array *array)
{
	list_add(&p->run_list, array->queue + p->prio);
	__set_bit(p->prio, array->bitmap);
	array->nr_active++;
	p->array = array;
}

/*
 * __normal_prio - return the priority that is based on the static
 * priority but is modified by bonuses/penalties.
 *
 * We scale the actual sleep average [0 .... MAX_SLEEP_AVG]
 * into the -5 ... 0 ... +5 bonus/penalty range.
 *
 * We use 25% of the full 0...39 priority range so that:
 *
 * 1) nice +19 interactive tasks do not preempt nice 0 CPU hogs.
 * 2) nice -20 CPU hogs do not get preempted by nice 0 tasks.
 *
 * Both properties are important to certain workloads.
 */

static inline int __normal_prio(struct task_struct *p)
{
	int bonus, prio;

	bonus = CURRENT_BONUS(p) - MAX_BONUS / 2;

	prio = p->static_prio - bonus;
	if (prio < MAX_RT_PRIO)
		prio = MAX_RT_PRIO;
	if (prio > MAX_PRIO-1)
		prio = MAX_PRIO-1;
	return prio;
}

/*
 * To aid in avoiding the subversion of "niceness" due to uneven distribution
 * of tasks with abnormal "nice" values across CPUs the contribution that
 * each task makes to its run queue's load is weighted according to its
 * scheduling class and "nice" value.  For SCHED_NORMAL tasks this is just a
 * scaled version of the new time slice allocation that they receive on time
 * slice expiry etc.
 */


int main()
{

return 0;
}
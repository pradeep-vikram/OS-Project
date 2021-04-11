// A C++ program that simulates the CPU scheduler

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include<limits>

using namespace std;

// Global variables to hold the arrival,burst time,priority
// and average waiting time and turnaround time

int p_count,quant,round_count = 0;
int arr_time[20];
int burst_time[20];
int priority[20];
float avg_wt[20];
int avg_wt_count;
float avg_tt[20];

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// A function to calculate waiting time in FCFS Algorithm
int * FCFS()
{
    static int wt1[20];

    int wt_1[5];
    if(arr_time[0]!= 0)
    {
        wt1[0]=arr_time[0];
        wt_1[0]=arr_time[0];
    }
    else
    {
        wt1[0] = 0;
        wt_1[0] = 0;
    }
    // calculating waiting time
    for (int  i = 1; i < 5 ; i++ )
    {
            wt_1[i] = burst_time[i-1] + wt_1[i-1];
            wt1[i] =  burst_time[i-1] + wt_1[i-1] - arr_time[i];
    }

    int total_wt=0;
    // Calculate total waiting time
    for (int  i=0; i<p_count; i++)
    {
        total_wt = total_wt + wt1[i];
    }

    avg_wt[avg_wt_count]=(float)total_wt / (float)p_count;

    return(wt1);
}

// A function to calculate waiting time in SJF Algorithm
int * SJF()
{
    avg_wt_count++;
    int mat[10][6];
    static int wt2[20];
    for(int i=0; i<p_count; i++)
	{
		mat[i][0] = i+1;
		mat[i][1] = arr_time[i];
		mat[i][2] = burst_time[i];
	}

	//Arrange By arrival time
    for(int i=0; i<p_count; i++)
	{
		for(int j=0; j<p_count-i-1; j++)
		{
			if(mat[j][1] > mat[j+1][1])
			{
				for(int k=0; k<5; k++)
				{
					swap(mat[j][k], mat[j+1][k]);
				}
			}
		}
	}

	//Calculating completion time
    int temp, val;
	mat[0][3] = mat[0][1] + mat[0][2];
	mat[0][5] = mat[0][3] - mat[0][1];
	mat[0][4] = mat[0][5] - mat[0][2];

	for(int i=1; i<p_count; i++)
	{
		temp = mat[i-1][3];
		int low = mat[i][2];
		for(int j=i; j<p_count; j++)
		{
			if(temp >= mat[j][1] && low >= mat[j][2])
			{
				low = mat[j][2];
				val = j;
			}
		}
		mat[val][3] = temp + mat[val][2];
		mat[val][5] = mat[val][3] - mat[val][1];
		mat[val][4] = mat[val][5] - mat[val][2];
		for(int k=0; k<6; k++)
		{
			swap(mat[val][k], mat[i][k]);
		}
	}

	//Re-ording the matrix by process number
    for(int i=0;i<p_count;i++)
	{
	    for(int j=i+1;j<p_count;j++)
        {
            if(mat[i][0] > mat[j][0])
            {
                for(int k=0; k<6; k++)
				{
					swap(mat[i][k], mat[j][k]);
				}

            }
        }
	}

	for(int i=0;i<p_count;i++)
    {
        wt2[i] = mat[i][4];
    }
    int total_wt = 0;
    for(int i=0;i<p_count;i++)
    {
        total_wt += wt2[i];
    }
    avg_wt[avg_wt_count] = float(total_wt) / float(p_count);

    return(wt2);
}

// A function to calculate waiting time in SRTN Algorithm
int * SRTN()
{
    avg_wt_count++;
    static int wt3[20];
    struct Process
    {
        int pid; // Process ID
        int bt; // Burst Time
        int art; // Arrival Time
    };
    Process proc[] = { { 0, 0, 0 }, { 0, 0, 0 },
					{ 0, 0, 0 }, { 0, 0, 0 },
					 { 0, 0, 0 }, { 0, 0, 0 },
					  { 0, 0, 0 }, { 0, 0, 0 },
					   { 0, 0, 0 }, { 0, 0, 0 },};
    for(int i=0;i<p_count;i++)
    {
        proc[i].pid = i+1;
        proc[i].bt = burst_time[i];
        proc[i].art = arr_time[i];
    }

    int rt[p_count];

	// Copy the burst time into rt[]
	for (int i = 0; i < p_count; i++)
		rt[i] = proc[i].bt;

	int complete = 0, t = 0, minm = INT_MAX;
	int shortest = 0, finish_time;
	bool check = false;

	// Process until all processes gets
	// completed
	while (complete != p_count) {

		// Find process with minimum
		// remaining time among the
		// processes that arrives till the
		// current time`
		for (int j = 0; j < p_count; j++) {
			if ((proc[j].art <= t) &&
			(rt[j] < minm) && rt[j] > 0) {
				minm = rt[j];
				shortest = j;
				check = true;
			}
		}

		if (check == false) {
			t++;
			continue;
		}

		// Reduce remaining time by one
		rt[shortest]--;

		// Update minimum
		minm = rt[shortest];
		if (minm == 0)
			minm = INT_MAX;

		// If a process gets completely
		// executed
		if (rt[shortest] == 0) {

			// Increment complete
			complete++;
			check = false;

			// Find finish time of current
			// process
			finish_time = t + 1;

			// Calculate waiting time
			wt3[shortest] = finish_time -
						proc[shortest].bt -
						proc[shortest].art;

			if (wt3[shortest] < 0)
				wt3[shortest] = 0;
		}
		// Increment time
		t++;
	}

	//Calculating Total waiting time
	int total_wt = 0;
	for(int i=0;i<p_count;i++)
    {
        total_wt += wt3[i];
    }
    avg_wt[avg_wt_count] = float(total_wt) / float(p_count);
    return(wt3);
}

// A function to calculate waiting time in PREEMPTIVE PRIORITY Algorithm
int * PRIORITY()
{
    avg_wt_count++;
    static int wt4[20];
    class Process
    {
        public:
            //string processName;
            int arrivalTime;
            int burstTime;
            int priority;
            int remainingTime;
            int responseTime;
            int completionTime;
            int waitingTime;
            int turnAroundTime;
            void initialize()
            {
                remainingTime = burstTime;
            }
    };
    Process processes[p_count];
    for(int i=0;i<p_count;i++)
    {
        processes[i].arrivalTime = arr_time[i];
        processes[i].burstTime = burst_time[i];
        processes[i].priority = priority[i];
        processes[i].initialize();
    }
    for(int i=0;i<p_count-1;i++){
        for(int j=i+1;j<p_count;j++){
            if(processes[j].arrivalTime < processes[i].arrivalTime){
                Process temp = processes[j];
                processes[j] = processes[i];
                processes[i] = temp;
            }
        }
    }

    int currentTime = 0;

    while(true){

    	int currentHighestPriorityIndex = -1;
    	int currentHighestPriority = numeric_limits<int>::max();

    	bool isAllCompleted = true;

    	for(int i=0;i<p_count;i++){
    		if(processes[i].remainingTime > 0){
    			isAllCompleted = false;
                if(processes[i].arrivalTime <= currentTime){
                    if(processes[i].priority < currentHighestPriority){
                        currentHighestPriority = processes[i].priority;
                        currentHighestPriorityIndex = i;
                    }
                }

    		}
    	}

    	if(isAllCompleted){
    		break;
    	}

		if(processes[currentHighestPriorityIndex].remainingTime == processes[currentHighestPriorityIndex].burstTime){
			processes[currentHighestPriorityIndex].responseTime = currentTime;
		}

    	processes[currentHighestPriorityIndex].remainingTime--;
        currentTime++;

		if(processes[currentHighestPriorityIndex].remainingTime == 0){
			processes[currentHighestPriorityIndex].completionTime = currentTime;
		}
    }

    int total_wt = 0;

    for(int n=0;n<p_count;n++)
    {
        processes[n].turnAroundTime = processes[n].completionTime - processes[n].arrivalTime;
        processes[n].waitingTime = processes[n].turnAroundTime - processes[n].burstTime;
        wt4[n] = processes[n].waitingTime;
        total_wt += processes[n].waitingTime;
    }

    avg_wt[avg_wt_count]=(float) total_wt/p_count;
    return(wt4);

}

// A function to calculate waiting time in ROUND ROBIN Algorithm
int * RR()
{
    avg_wt_count++;
    static int wt5[20];
    int i, NOP = p_count, sum=0,count=0, y, wt=0, at[10], bt[10], temp[10];
    y = NOP;
    for(i=0; i<NOP; i++)
    {
        at[i] = arr_time[i];
        bt[i] = burst_time[i];
        temp[i] = bt[i];
    }

    int mat[NOP][3];
    for(sum=0, i = 0; y!=0; )
    {
        if(temp[i] <= quant && temp[i] > 0) // define the conditions
        {
            sum = sum + temp[i];
            temp[i] = 0;
            count=1;
        }
        else if(temp[i] > 0)
        {
            temp[i] = temp[i] - quant;
            sum = sum + quant;
        }
        if(temp[i]==0 && count==1)
        {
            y--; //decrement the process no.
            mat[i][0] = i+1;
            mat[i][1] = sum-at[i]-bt[i];
            mat[i][2] = sum-at[i];
            wt = wt+sum-at[i]-bt[i];
            count =0;
        }
        if(i==NOP-1)
        {
            i=0;
        }
        else if(at[i+1]<=sum)
        {
            i++;
        }
        else
        {
            i=0;
        }
    }
    //represents the average waiting time
    for(int i=0;i<NOP;i++)
	{
	    for(int j=i+1;j<NOP;j++)
        {
            if(mat[i][0] > mat[j][0])
            {
                for(int k=0; k<3; k++)
				{
					swap(mat[i][k], mat[j][k]);
				}

            }
        }
	}
	for(int i=0;i<p_count;i++)
    {
        wt5[i] = mat[i][1];
    }
    avg_wt[avg_wt_count] = float(wt)/float(p_count);
	return(wt5);

}

// Function that prints the waiting time of each process corresponding to time taken in each scheduling algorithm
void print_wt(int *wt1,int *wt2,int *wt3,int *wt4,int *wt5)
{
    cout<<"\n\tRound : "<<round_count<<"\t"<<p_count<<" Processes\tQuantum : "<<quant;
    cout<<"\n\t_____________________________________________";
    cout<<"\n\n\t\tWaiting |\n ";
    cout<<"\t\tTime | FCFS |  SJF  | SRTN | PRIORITY  |  RR \n";
    cout<<"\t\t -----+------+-------+------+----------+--------\n";
    for(int i=0;i<p_count;i++)
    {
        cout<<"\t\t P"<<i+1<<"    |  "<< *(wt1 + i) <<"   |  "<<*(wt2 + i)<<"   | "<<*(wt3 + i)<<"    |   "<<*(wt4 + i)<<"     |   "<<*(wt5 + i )<<"\n";
    }
    cout<<"\n\t\t-----+------+-----+------+----------+-----------\n";
    cout<<"\t\tAVERAGE| "<<avg_wt[0];
    for(int i=1;i<5;i++)
    {
        cout<<"  |  "<<avg_wt[i];
    }
}

// Function that prints the turnaround time of each process corresponding to time taken in each scheduling algorithm
// This function adds waiting time and burst time
void print_tat(int *wt1,int *wt2,int *wt3,int *wt4,int *wt5)
{
    int tat1[p_count],tat2[p_count],tat3[p_count],tat4[p_count],tat5[p_count];
    int tt1=0,tt2=0,tt3=0,tt4=0,tt5=0;
    for(int i=0;i<p_count;i++)
    {
        tat1[i] = *(wt1+i) + burst_time[i];
        tt1+=tat1[i];
        tat2[i] = *(wt2+i) + burst_time[i];
        tt2+=tat2[i];
        tat3[i] = *(wt3+i) + burst_time[i];
        tt3+=tat3[i];
        tat4[i] = *(wt4+i) + burst_time[i];
        tt4+=tat4[i];
        tat5[i] = *(wt5+i) + burst_time[i];
        tt5+=tat5[i];
    }
    avg_tt[0] = float(tt1)/float(p_count);
    avg_tt[1] = float(tt2)/float(p_count);
    avg_tt[2] = float(tt3)/float(p_count);
    avg_tt[3] = float(tt4)/float(p_count);
    avg_tt[4] = float(tt5)/float(p_count);
    cout<<"\n\n\t\tTurnaround | \n\n";
    cout<<"\t\tTime | FCFS |  SJF  | SRTN | PRIORITY  |  RR \n";
    cout<<"\t\t -----+------+-------+------+----------+----------\n";
    for(int i=0;i<p_count;i++)
    {
        cout<<"\t\t P"<<i+1<<"    |  "<< tat1[i] <<"   |  "<<tat2[i]<<"   | "<<tat3[i]<<"    |   "<<tat4[i]<<"     |   "<<tat5[i]<<"\n";
    }
    cout<<"\n\t\t-----+------+-----+------+----------+------------\n";
    cout<<"\t\tAVERAGE| "<<avg_tt[0];
    for(int i=1;i<5;i++)
    {
        cout<<"  |  "<<avg_tt[i];
    }
    cout<<"\n";

}

//Main function
int main () {

   // open a file in write mode.
   ifstream infile;
   infile.open("process.txt",ios::in);
   int data1;

   while(!infile.eof())
   {
        round_count++;
        avg_wt_count=0;
        // Reading from the file
        infile >> data1;
        if(infile.eof())
            {
                break;
            }
        p_count=data1;

        infile >> data1;
        if(infile.eof())
            {
                break;
            }
        quant = data1;

        // Initializing global variables to zero
        for(int i=0;i<20;i++)
        {
            arr_time[i]=0;
            burst_time[i]=0;
            priority[i]=0;
            avg_wt[i]=0;
            avg_tt[i]=0;
        }
        avg_wt_count=0;

        int data,flag=0;
        for(int i=0;i<p_count;i++)
        {
            infile >> data;
            if(infile.eof())
            {
                break;
                flag =1;
            }
            arr_time[i] = data;
            infile >> data;
            if(infile.eof())
            {
                break;
                flag =1;
            }
            burst_time[i] = data;
            infile >> data;
            if(infile.eof())
            {
                break;
                flag=1;
            }
            if(flag==1)
            {
                break;
            }
            priority[i] = data;
        }

        // returned array of waiting time is stored in variables declared below
        int *wt1,*wt2,*wt3,*wt4,*wt5;
        wt1 = FCFS();
        wt2 = SJF();
        wt3 = SRTN();
        wt4 = PRIORITY();
        wt5 = RR();
        print_wt(wt1,wt2,wt3,wt4,wt5);
        print_tat(wt1,wt2,wt3,wt4,wt5);

    }


        // close the opened file.
        infile.close();

        return 0;
}

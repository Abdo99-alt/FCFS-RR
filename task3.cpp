// C++ program for implementation of FCFS && Round Robin
// scheduling algorithms with I/O burst included
#include<iostream>
#include <queue>
#include <fstream>
#include <iomanip>

using namespace std;
fstream fout;
typedef pair<int, int> pd;

struct myComp {
    constexpr bool operator()(
        pair<int, int> const& a,
        pair<int, int> const& b)
        const noexcept
    {
        return a.second > b.second;
    }
};

priority_queue<pd, vector<pd>, myComp> Blocked_queue;

priority_queue<int, vector<int>, greater<int> > Ready ;
queue<int> Ready_queue ;
queue<int> Running_queue ;

int n ;
using namespace std;

//functions to show the processes in queues
void showq(queue<int> gq)
{
    queue<int> g = gq;
    while (!g.empty()) {
        fout  << " " << g.front();
        g.pop();
    }
}

void showq2(priority_queue<pd, vector<pd>, myComp> gq)
{
    priority_queue<pd, vector<pd>, myComp> g = gq;
    while (!g.empty()) {
        fout << " "  << g.top().first;
        g.pop();
    }
}

void scheduling(int processes[],int quantumTime,int scheduling_algorithm, int n, int bt[], int at[],int Iot[]){

    int CPU_times[n] , bt_used[n] ;

    for (int i = 0 ; i < n ; i++){
        CPU_times[i]=0;
        bt_used[i]=0;
    }

    int time_cycle=0 , arrived = 0;
    int CPU_start_time[n],IO_start_time[n];
    int  Finishing_time=0 ;
    float CPU_used=0 ;
    int quantum=0;
    int Turnaround_time[n] ;

    while(time_cycle >= 0){
            if(scheduling_algorithm==1) quantum++;
            //ready
            for (int i = 0 ; i < n ; i++){
                if(at[processes[i]]==time_cycle){
                    arrived++;
                  if(bt[processes[i]]==0)
                      Blocked_queue.push(make_pair(processes[i],at[processes[i]]+Iot[processes[i]]));
                  else
                      Ready.push(processes[i]) ;
                }
            }
            //Quantum Ended and IO

            if(!Running_queue.empty()){
                if(quantum==quantumTime&&bt_used[Running_queue.front()]<bt[Running_queue.front()]&&scheduling_algorithm==1){
                        Ready.push(Running_queue.front()) ;
                        Running_queue.pop();
                }
                else if(bt_used[Running_queue.front()]==bt[Running_queue.front()]){
                        bt_used[Running_queue.front()]=0;
                        CPU_times[Running_queue.front()]++;
                    if(CPU_times[Running_queue.front()]>1){
                         Turnaround_time[Running_queue.front()]=time_cycle-at[Running_queue.front()];
                         Running_queue.pop();
                   }
                    else
                    {
                        CPU_times[Running_queue.front()]++;
                        IO_start_time[Running_queue.front()]=time_cycle;
                        Blocked_queue.push(make_pair(Running_queue.front(),
                        IO_start_time[Running_queue.front()]+Iot[Running_queue.front()]));
                        Running_queue.pop();
                    }
                }

            }

            //Blocked
            while(!Blocked_queue.empty() &&
            ((bt[Blocked_queue.top().first]==0&&at[Blocked_queue.top().first]
            +Iot[Blocked_queue.top().first]==time_cycle) || (bt[Blocked_queue.top().first]!=0&&
            IO_start_time[Blocked_queue.top().first]+Iot[Blocked_queue.top().first]==time_cycle))){

                    if(bt[Blocked_queue.top().first]==0&&at[Blocked_queue.top().first]+Iot[Blocked_queue.top().first]==time_cycle){
                        Turnaround_time[Blocked_queue.top().first]=Iot[Blocked_queue.top().first];
                        Blocked_queue.pop();
                    }
                    else{
                       Ready.push(Blocked_queue.top().first) ;
                       Blocked_queue.pop();
                    }

            }
            while(!Ready.empty()){
                Ready_queue.push(Ready.top());
                Ready.pop();
            }
            //CPU
            if(Running_queue.empty()&&(!Ready_queue.empty())){
                Running_queue.push(Ready_queue.front());
                Ready_queue.pop();
                quantum=0;
            }
            if(!Running_queue.empty()) bt_used[Running_queue.front()]++;

            if(!Ready_queue.empty()||!Running_queue.empty()||!Blocked_queue.empty()||time_cycle<=at[0]){
            fout <<time_cycle << " -->";
            if(!Ready_queue.empty()){

             showq(Ready_queue);
             fout <<" : Ready" ;
            }
             if(!Running_queue.empty()){
                 CPU_used++;
               showq(Running_queue);
                fout <<" : Running";
             }
             if(!Blocked_queue.empty()){
                  showq2(Blocked_queue);
                  fout <<" : Blocked"  ;
             }
              fout <<"\n"  ;
            }
            if (Ready_queue.empty()&&Running_queue.empty()&&Blocked_queue.empty()&&arrived == n)
            break;
            time_cycle++ ,Finishing_time++;;
    }
    Finishing_time--;
    float  CPU_Utilization=CPU_used/(Finishing_time+1) ;
    fout <<"\nFinishing time:"<< Finishing_time <<"\n";
    fout <<"CPU Utilization:"<< setprecision(2) << CPU_Utilization<<"\n" ;
     for (int i = 0 ; i < n ; i++)
 fout <<"Turnaround time of Process " <<i<<":"<<Turnaround_time[i]<<"\n";

}

void bubbleSort(int arrival_time[],int processes[],int CPU_time[],int IO_time[],int size) {

  for (int step = 0; step < size; ++step) {
    for (int i = 0; i < size - step; ++i) {
      if (arrival_time[i] > arrival_time[i + 1]) {
        //IDs
        int temp1 = processes[i];
        processes[i]=processes[i+1];
        processes[i+1]=temp1 ;
        //Arrival times
        temp1 = arrival_time[i];
        arrival_time[i]=arrival_time[i+1];
        arrival_time[i+1]=temp1;
        //CPU times
        temp1 = CPU_time[i];
        CPU_time[i]=CPU_time[i+1];
        CPU_time[i+1]=temp1;
        //IO times
        temp1 = IO_time[i];
        IO_time[i]=IO_time[i+1];
        IO_time[i+1]=temp1;
      }
    }
  }
}

// Driver code
int main()
{
    std ::string testfile;
    std ::string outfile;
    fstream fp;
    //get the test case filename from user
    cout << "Enter file name : ";
    getline(cin , testfile); 
    while(1){
        fp.open(testfile , ios::in);
        if(fp.fail()){
            cout << "Invalid\n";
            cout << "Enter file name : ";
            fp.clear();
            getline(cin , testfile);
        }
        else break;
    }
    std ::string unused;
    int lines = 0;
    //count the lines of test file,hence the number of processes
    while(std :: getline(fp , unused))
        ++lines;

    int p_table[lines][4];
    int processes[lines] , CPU_time[lines] , IO_time[lines] , arrival_time[lines];
    //seek the beginning of file
    fp.clear();
    fp.seekg(0);
    //loop to store the contents of file in 2D array
    for(int i = 0 ; i < lines ; i++){
        fp.get();
        for(int j = 0 ; j < 4 ; j++)
            fp >> p_table[i][j];
        std::getline(fp , unused);
    }
    for(int i = 0 ; i < lines ; i++){
        for(int j = 0 ; j < 4 ; j++){
            processes[i] = p_table[i][0];
            CPU_time[i] = p_table[i][1];
            IO_time[i] = p_table[i][2];
            arrival_time[i] = p_table[i][3];
        }
    }
    int quantumTime ,scheduling_algorithm ;
    cout << "Enter '0' for FCFS '1' for RR : " ;
    cin >>scheduling_algorithm ;
    if(scheduling_algorithm == 0)
        outfile = "outputFCFS.txt";
    else outfile = "outputRR.txt";
    //opening a test file for storing output in it
    fout.open(outfile , ios::out);
    if(scheduling_algorithm==1){
        cout << "Enter The quantumTime : " ;
        cin >>quantumTime ;
    }
    //number of processes
    n = sizeof processes / sizeof processes[0];
    bubbleSort(arrival_time,processes,CPU_time,IO_time,n-1);
    scheduling(processes,quantumTime,scheduling_algorithm,n, CPU_time, arrival_time , IO_time);
    fout.close();
    fp.close();
 return 0;
}
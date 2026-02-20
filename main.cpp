#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <queue>
using namespace std;

struct Process {
    int arrivalTime = 0;
    int burstTime = 0;
    int prioLevel = 0;

    int remainingTime = 0;
    bool finished = false;

    int waitingTime = 0;
    int turnaroundTime = 0;
    int responseTime = 0;
    int completionTime = 0;
};

void printGanttChart(int timeElapsed, int timeCont, int prevIdx, Process& process) {
    cout << timeElapsed - timeCont << " " << prevIdx + 1 << " " << timeCont;
    if(process.finished) cout << "X";
    cout << endl;
}

void printStats(vector<Process> &process, int timeElapsed) {
    int processSize = process.size();

    // total elapsed time
    cout << "Total time elapsed: " << timeElapsed << "ns" << endl;

    // total cpu burst time
    int totalCPUBurstTime = 0;
    for(int i = 0; i < processSize; i++) {
        totalCPUBurstTime += process[i].burstTime;
    }
    cout << "Total CPU burst time: " << totalCPUBurstTime << "ns" << endl;
    
    // total cpu utilization
    float cpuUtil = totalCPUBurstTime / timeElapsed * 100;
    cout << "CPU Utilization: " << cpuUtil << "%" << endl;

    // throughput
    float throughput = (float)processSize / timeElapsed;
    cout << "Throughput: " << throughput << " processes/ns" << endl;
    
    // waiting time
    float totalWaitingTime = 0;
    cout << "Waiting times:" << endl;
    for(int i = 0; i < processSize; i++) {
        cout << "  Process " << i + 1 << ": " << process[i].waitingTime << "ns" << endl;
        totalWaitingTime += process[i].waitingTime;
    }
    cout << "Average waiting time: " << totalWaitingTime / processSize << "ns" << endl;

    // turnaround time
    float totalTurnaroundTime = 0;
    cout << "Turnaround times:" << endl;
    for(int i = 0; i < processSize; i++) {
        cout << "  Process " << i + 1 << ": " << process[i].turnaroundTime << "ns" << endl;
        totalTurnaroundTime += process[i].turnaroundTime;
    }
    cout << "Average turnaround time: " << totalTurnaroundTime / processSize << "ns" << endl;

    // response time
    float totalResponseTime = 0;
    cout << "Response times:" << endl;
    for(int i = 0; i < processSize; i++) {
        cout << "  Process " << i + 1 << ": " << process[i].responseTime << "ns" << endl;
        totalResponseTime += process[i].responseTime;
    }
    cout << "Average response time: " << totalResponseTime / processSize << "ns" << endl;
}

// TODO:
void FCFS(vector<Process> &process, int processAmount) {
    int timeElapsed = 0;
    int timeCont = 0;
    int completed = 0;

    // FCFS simulation while loop

    int curIdx = -1;
    bool processing = false;
    queue<int> readyQ;

    while(completed != processAmount){

        // push process into ready queue
        for(int i = 0; i < processAmount; i++){
            Process& p = process[i];
            if(p.arrivalTime == timeElapsed){
                readyQ.push(i);
            }
        }

        // put process into CPU if possible
        if(!processing && readyQ.size() != 0){
            curIdx = readyQ.front();
            readyQ.pop();
            processing = true;
        }

        // if CPU is currently running a process:
        if(processing){
            process[curIdx].remainingTime--;
            if(process[curIdx].remainingTime == 0) {
                process[curIdx].finished = true;
                process[curIdx].completionTime = timeElapsed + 1; // completed it at THIS loop so +1
                process[curIdx].turnaroundTime = process[curIdx].completionTime - process[curIdx].arrivalTime;
                process[curIdx].waitingTime = process[curIdx].turnaroundTime - process[curIdx].burstTime;
                processing = false;
                completed++;
            }
        }
        
        // increment internal timer
        timeElapsed++;
        timeCont++;
    }

    printStats(process, timeElapsed);
}

// TODO:
void SJF(vector<Process> &process, int processAmount) {

}

/* Limitations:
    - cannot work with inputs of 0 0 0 for processes (will lead to infinite loop).
*/
void SRTF(vector<Process> &process, int processAmount) {
    int timeElapsed = 0;
    int timeCont = 0; // continous time for CPU
    int completed = 0; // completed processes

    // saver variables
    int prevIdx = -1;
    while(completed != processAmount) {
        int shortestIdx = -1;
        int min = INT_MAX;
        // find the arrived, unfinished process with the smallest remaining time
        for(int i = 0; i < processAmount; i++) {
            Process& cur = process[i];
            if(cur.arrivalTime <= timeElapsed && min > cur.remainingTime && !cur.finished) {
                min = cur.remainingTime;
                shortestIdx = i;
            }
        }
        // cpu is idle, no process in ready queue
        if(shortestIdx == -1) {
            timeElapsed++;
            continue;
        }
        // if the process is different, print previous
        if(shortestIdx != prevIdx) {
            if(prevIdx != -1) { // only print the prev if there ACTUALLY is a prev
                printGanttChart(timeElapsed, timeCont, prevIdx, process[prevIdx]);
            }
            timeCont = 0; // reset continuous run time for the new process
        }
        // if the process has never been touched, then register that it has been touched
        if(process[shortestIdx].burstTime == process[shortestIdx].remainingTime) {
            process[shortestIdx].responseTime = timeElapsed - process[shortestIdx].arrivalTime;
        }
        // increment waiting processes if they are idle and have arrived
        for(int i = 0; i < processAmount; i++) {
            Process& cur = process[i];
            if(i != shortestIdx && cur.arrivalTime <= timeElapsed && !cur.finished) {
                cur.waitingTime++;
            }
        }
        // decrement remainting for process
        process[shortestIdx].remainingTime--;
        // if the process is complete
        if(process[shortestIdx].remainingTime == 0) {
            process[shortestIdx].finished = true;
            process[shortestIdx].completionTime = timeElapsed + 1; // completed it at THIS loop so +1
            process[shortestIdx].turnaroundTime = process[shortestIdx].completionTime - process[shortestIdx].arrivalTime;
            completed++;
        }
        // increment timers
        timeElapsed++;
        timeCont++;
        // remember the currently running process
        prevIdx = shortestIdx; 
    }
    if(prevIdx != -1) { // again makes sure we missed something, since it doesn't print the last completed process
        printGanttChart(timeElapsed, timeCont, prevIdx, process[prevIdx]);
    }
    printStats(process, timeElapsed);
}

// TODO:
void P(vector<Process> &process, int processAmount) {

}

// TODO:
void RR(vector<Process> &process, int processAmount, int timeQuantum) {

}

int main() {
    int testCases;
    cout << "Enter amount of test cases: " << endl;
    cin >> testCases; // gets test cases
    
    for(int i = 0; i < testCases; i++) {
        cout << "Test Case # " << i + 1 << endl;

        string algorithm = "";
        int processAmount = 0;
        int timeQuantum = 0;

        cout << "Input number of processes and type of algorithm: " << endl;
        cin >> processAmount >> algorithm;

        // if its a round robin, it has to have a time quantum variable
        if(algorithm == "RR") {
            cin >> timeQuantum;
        }

        // create a vector of process objects
        vector<Process> process(processAmount);
        // input process data
        for(int j = 0; j < processAmount; j++) {
            cin >> process[j].arrivalTime >> process[j].burstTime >> process[j].prioLevel;
            process[j].remainingTime = process[j].burstTime;
        }

        cout << "OUTPUT" << endl;
        cout << i + 1 << " " << algorithm << endl;

        // checks which algorithm the user chose
        if(algorithm == "FCFS") {
            FCFS(process, processAmount);
        } else if(algorithm == "SJF") {
            SJF(process, processAmount);
        } else if(algorithm == "SRTF") {
            SRTF(process, processAmount);
        } else if(algorithm == "P") {
            P(process, processAmount);
        } else if(algorithm == "RR") {
            RR(process, processAmount, timeQuantum);
        } else {
            cout << "Algorithm does not exist." << endl;
        }
    }
    return 0;
}
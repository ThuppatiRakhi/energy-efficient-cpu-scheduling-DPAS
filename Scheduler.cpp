#include <iostream>
using namespace std;
struct Process {
    int id;
    int at;   // arrival time
    int bt;   // burst time
    int ct;   // completion time
    int tat;  // turnaround time
    int wt;   // waiting time
};
// energy levels
double LOW = 1.2;
double MED = 2.1;
double HIGH = 3.0;
double energy(double power, int time) {
    return power * (time / 1000.0);
}
void calculate(Process p[], int n) {
    for(int i=0;i<n;i++){
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}
void print(Process p[], int n) {
    cout<<"PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i=0;i<n;i++){
        cout<<p[i].id<<"\t"<<p[i].at<<"\t"<<p[i].bt<<"\t"<<p[i].ct<<"\t"<<p[i].tat<<"\t"<<p[i].wt<<"\n";
    }
}
double FCFS(Process p[], int n) {
    int time = 0;
    double totalE = 0;
    for(int i=0;i<n;i++){
        if(time < p[i].at) time = p[i].at;
        time += p[i].bt;
        p[i].ct = time;
        totalE += energy(MED, p[i].bt);
    }
    calculate(p, n);
    cout << "\n--- FCFS ---\n";
    print(p, n);
    return totalE;
}
double SJF(Process p[], int n) {
    Process temp[10];
    for(int i=0;i<n;i++) temp[i] = p[i];
    int done[10] = {0};
    int time = 0, completed = 0;
    double totalE = 0;
    while(completed < n) {
        int idx = -1, minBT = 9999;
        for(int i=0;i<n;i++){
            if(!done[i] && temp[i].at <= time && temp[i].bt < minBT){
                minBT = temp[i].bt;
                idx = i;          }
        }
        if(idx == -1) { time++; continue; }
        time += temp[idx].bt;
        temp[idx].ct = time;
        done[idx] = 1;
        completed++;
        totalE += energy(MED, temp[idx].bt);
    }
    calculate(temp, n);
    cout << "\n--- SJF ---\n";
    print(temp, n);
    return totalE;
}
double RR(Process p[], int n, int q) {
    Process temp[10];
    for(int i=0;i<n;i++) temp[i] = p[i];
    int rem[10];
    for(int i=0;i<n;i++) rem[i] = temp[i].bt;
    int time = 0, completed = 0;
    double totalE = 0;
    while(completed < n) {
        for(int i=0;i<n;i++){
            if(rem[i] > 0 && temp[i].at <= time){
                int run = (rem[i] > q ? q : rem[i]);
                rem[i] -= run;
                time += run;
                totalE += energy(MED, run);
                if(rem[i] == 0){
                    temp[i].ct = time;
                    completed++;
                }
            }
        }
        time++;
    }
    calculate(temp, n);
    cout << "\n--- ROUND ROBIN ---\n";
    print(temp, n);
    return totalE;
}
double DPAS(Process p[], int n) {
    Process temp[10];
    for(int i=0;i<n;i++) temp[i] = p[i];
    int done[10] = {0};
    int time = 0, completed = 0;
    double totalE = 0;
    while(completed < n) {
        int idx = -1, minBT = 9999;
        for(int i=0;i<n;i++){
            if(!done[i] && temp[i].at <= time && temp[i].bt < minBT){
                minBT = temp[i].bt;
                idx = i;
            }
        }
        if(idx == -1) { time++; continue; }
        double power;
        if(temp[idx].bt <= 3) power = LOW;
        else if(temp[idx].bt <= 10) power = MED;
        else power = HIGH;
        time += temp[idx].bt;
        temp[idx].ct = time;
        done[idx] = 1;
        completed++;
    totalE += energy(power, temp[idx].bt);    }
    calculate(temp, n);
    cout << "\n--- DPAS (Energy Efficient Algo) ---\n";
    print(temp, n);
    return totalE;
}
int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    Process p[10];
    for(int i=0;i<n;i++){
        p[i].id = i+1;
        cout<<"Enter AT and BT for P"<<i+1<<": ";
        cin >> p[i].at >> p[i].bt;
    }
    int q;
    cout<<"Enter RR Quantum: ";
    cin >> q;
    double e1 = FCFS(p, n);
    double e2 = SJF(p, n);
    double e3 = RR(p, n, q);
    double e4 = DPAS(p, n);
    cout << "\n=== ENERGY COMPARISON (J) ===\n";
    cout << "FCFS: " << e1 << "\n";
    cout << "SJF: " << e2 << "\n";
    cout << "RR:  " << e3 << "\n";
    cout << "DPAS:" << e4 << "\n";
    return 0;
}

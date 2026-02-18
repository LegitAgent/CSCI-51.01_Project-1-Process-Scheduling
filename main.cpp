#include <iostream>
#include <string>
using namespace std;

int main() {
    int testCases;
    cout << "Enter amount of test cases: " << endl;
    cin >> testCases;
    
    for(int i = 0; i < testCases; i++) {
        cout << "Test Case # " << i + 1 << endl;

        string algorithm = "";
        int processesAmount = 0;
        int time = 0;

        cout << "Input number of processes and type of algorithm: " << endl;
        cin >> processesAmount >> algorithm;

        if(algorithm == "RR") {
            cin >> time;
        }

        int process[processesAmount][3];
        for(int j = 0; j < processesAmount; j++) {
            cin >> process[j][0] >> process[j][1] >> process[j][2];
        }
    }
    return 0;
}
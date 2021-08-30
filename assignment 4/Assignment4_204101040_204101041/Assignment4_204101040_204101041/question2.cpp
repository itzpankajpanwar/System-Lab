#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

int vehicleID = 0;
bool usingTunnel = false;    // variable used to indicate whether there is any car inside the tunnel
int last_used = -1;          // variable used to indicate the direction of vehicle which passed the tunnel recently
mutex useTunnel;             // mutex used to ensure atomicity
condition_variable c;        // conditional variable used to allow or reject a vehicle to enter the tunnel
vector<int> waiting(2);      // vector to count the number of vehicles waiting on each side

void Arrive(int id, int dir){
    unique_lock<mutex> ul(useTunnel);
    string s = dir ? "North" : "South";
    cout << "Vehicle " << id << " arrived at " << s << " end." << endl; 

    // a vehicle will wait outside the tunnel if there is already a vehicle present inside the tunnel
    // or the direction of vehicle previously passed through the tunnel has the same direction as the
    // incoming vehicle and there is some other vehicle already waiting to enter the tunnel at the opposite end
    while(usingTunnel || (last_used==dir && waiting[1-dir])){
        waiting[dir]++;      // increase the waiters on 'dir' end
        c.wait(ul);          // waiting until the above conditiom is true
        waiting[dir]--;      // got green signal to enter the tunnel hence reuding the waiting count
    }

    last_used = dir;         // storing the direction in which the vehicle moved
    usingTunnel = true;      // tunnel is busy
    cout << "Vehicle " << id << " entered at the tunnel from " << s << " end." << endl; 
}

void Depart(int id, int dir){
    unique_lock<mutex> ul(useTunnel);
    string s = dir ? "South" : "North";
    cout << "Vehicle " << id << " departed from " << s << " end." << endl;
    usingTunnel = false;   // tunnel is empty
    c.notify_all();        // notifying all the other vehicles that the tunnel is empty
}

// controller function to handle arrival and departure of the vehicles through the tunnel
void controller(int id, int dir){
    Arrive(id, dir);
    Depart(id, dir);
}

int main(){
    int n_vehicles = 0;
    cout << "Enter the total number of vehicles: ";
    cin >> n_vehicles;

    // creating thread for every incoming vehicle
    vector<thread> threads(n_vehicles);
    for(int i=0; i<n_vehicles; ++i){
        int dir = rand() % 2;
        threads[i] = thread(controller, ++vehicleID, dir);
    }

    for(int i=0; i<n_vehicles; ++i) threads[i].join();

    return 0;
}
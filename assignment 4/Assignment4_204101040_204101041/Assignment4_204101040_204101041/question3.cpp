#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class Hospital{
  private:
    mutex m;
    int n_chairs, n_patients;
    int n_vaccinated = 0;
    int n_returned = 0;
    queue<int> waiting_queue;
    condition_variable_any doctor_cv;

  public:
    Hospital(int n_chairs, int n_patients){
        this->n_chairs = n_chairs;
        this->n_patients = n_patients;
    }

    // function to handle doctors functionality
    void Doctor(){
        m.lock();
        // hospital is open till the specified number of patients get arrived
        while(n_vaccinated + n_returned < n_patients){
            // doctor does his own work if no patient is waiting
            while(waiting_queue.size() == 0){
                cout << "Doctor is doing his work..." << endl;
                doctor_cv.wait(m);  // doctor waits until a patient arrives/is waiting
            }

            // doctor vaccinates the patient
            cout << "Patient " << waiting_queue.front() << " got vaccinated." << endl; 
            waiting_queue.pop();  // chooses the next patient waiting in the queue
            n_vaccinated++;
            this_thread::sleep_for(chrono::milliseconds(1)); // assumes that it takes 1 ms for vaccianation
        }
        m.unlock();
    }

    void Patient(int id){
        m.lock();
        cout << "Patient " << id  << " arrived at the clinic." << endl;
        // patient leaves the clinic if there are no chairs for waiting
        if(waiting_queue.size() == n_chairs){
            n_returned++;
            cout << "Patient " << id << " returned without getting vaccinated." << endl;
        }

        // patient waits in queue is some other patient is being vaccinated
        else{
            waiting_queue.push(id);
            doctor_cv.notify_all();
        }
        m.unlock();
    }
};

int patientID = 0;
int main(){
    int n_chairs;
    cout << "Enter the number of chairs: ";
    cin >> n_chairs;

    int n_patients;
    cout << "Enter the number of patients: ";
    cin >> n_patients; 

    // creating a hospital monitor object
    Hospital hospital(n_chairs, n_patients);
    // running the doctor thread
    thread doctorThread(&Hospital::Doctor, &hospital);

    // creating separate thread for every incoming patient
    vector<thread> patients(n_patients); 
    for(int i=0; i<n_patients; ++i){
        patients[i] = thread(&Hospital::Patient, &hospital, ++patientID);
    }

    for(int i=0; i<n_patients; ++i) patients[i].join();
    doctorThread.join();

    return 0;
}
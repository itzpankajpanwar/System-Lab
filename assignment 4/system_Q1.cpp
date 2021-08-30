#include<bits/stdc++.h>

using namespace std;

#define ld long double 

vector<ld> priority(9);
vector<ld> arrival_time(9);
vector<ld> burst_time(9);
vector<ld> completion_time(9);
vector<ld> waiting_time(9);
vector<ld> TAT(9); 


void get_priority(string s)    // function to extract priority from input roll no 
{   int i=0;
    for(auto &x:s)
        priority[i++]= (x-'0');
    return;
}

void get_arrival_time(string s)   // function to extract arrival time  from input roll no
{   int i=8;
    for(auto &x:s)
        arrival_time[i--]= (x-'0');
    return;
}

void get_burst_time(string s)  // function to randomly generate burst time ( range (2-7) milliseconds ) for each process 
{   int i=0;

    for(auto &x:s)
        {   int r=0;
            r = 2 + rand() % 5 ;  
            burst_time[i++]= r;
        }
    return;
}

struct trip
{   
    int pno;
    ld start,end;
};

vector<trip> gantt_chart;

void print_gannt_chart()
{   
    cout<<"\n\n             FINAL GANTT CHART\n";
    cout<<"\n format is : Process_No(Start_Time , End_Time ) ";
    cout<<"\n # indicate no process is executed \n\n";
    int z = 5*gantt_chart.size();
    while(z--)
        cout<<"- -";
    cout<<endl<<"|";
    if(gantt_chart[0].start!=0)
        cout<<" # |";
    int ct=0;
    for(auto &x : gantt_chart)
        {   
            if( ct>0 && gantt_chart[ct].start != gantt_chart[ct-1].end+1)
                cout<<" # | ";
            cout<<"  P"<<x.pno<<"("<<x.start<<","<<x.end<<") |";
            ct++;

        }
    cout<<"\n";
    z = 5*gantt_chart.size();
    while(z--)
        cout<<"- -";
    cout<<endl;
}

int main()
{   
    int p_pre =-1;
    int cec=0;
    ld avg_waiting_time =0 , avg_tat =0;
    srand(time(0));  
    string roll_no;
    while(roll_no.length() != 9 )
        {
            cout<<"\nEnter Roll NO : (9 digit ) ";
            cin>>roll_no;
            if(roll_no.length()!= 9)
                cout<<"Invalid roll NO : Please Enter Correct Length";
        }

    get_priority(roll_no);
    get_arrival_time(roll_no);
    get_burst_time(roll_no);

    vector<ld> at = arrival_time;
    vector<ld> bt = burst_time;
    
    ld ct=0;  // variable to hold current time isntance 
    vector<int> vcq;
    bool runner = true;
    cout<<"\n Higher value menas higher priority \n";
    while(runner)
    {   
        vcq.clear();
         
        for(int i=0;i<9;i++)     // getting all process which are available for exection at current time 
            if(at[i]<=ct)
                vcq.push_back(i);
        
        if(vcq.size()>0)
        {
            int max_priority=vcq[0];

            for(int i=1;i<vcq.size();i++)    // deciding which process will get executed based on priority and process index
                if( priority[vcq[i]] > priority[max_priority] )
                    max_priority = vcq[i];
        
            cout<<"\ntime : "<<ct<<"\t Process : "<<max_priority;

            if(gantt_chart.size()==0)
                gantt_chart.push_back({max_priority , ct ,ct});
            else
            {
                if( gantt_chart[gantt_chart.size()-1].pno == max_priority)
                    gantt_chart[gantt_chart.size()-1].end = ct;
                else
                    gantt_chart.push_back({max_priority , ct,ct});
            }


            bt[max_priority]--;   // executing process for one time unit 

            if(bt[max_priority]<=0)
            {   
                cout<<"\t process "<<max_priority<<" compeleted";
                completion_time[max_priority] = ct+1;
                waiting_time[max_priority] = ct+1 - arrival_time[max_priority] - burst_time[max_priority];
                TAT[max_priority] = ct+1 - arrival_time[max_priority] ;
                at[max_priority] = 1000000;
            }
        }
        else  // if no process is executed in current time unit 
            cout<<"\ntime : "<<ct<<"\t No process is executed  ";
        
        runner = false;
        for(int i=0;i<9;i++)  // checking if there is still any process left for execcution 
        {
            if(bt[i]>0)
                runner=true;  // if any process is left for execution then runner flag will be true 
        }

        ct++;  
        
    }

    for(int i=0;i<9;i++)   // calculating waiting time and turn around time 
    {
        avg_waiting_time+=waiting_time[i];
        avg_tat = TAT[i];
    }
    avg_waiting_time/=9;
    avg_tat/=9;

    cout<<"\n\n\nProcess No \t Arrival Time \t Priority\tBurst Time\tCompletion Time\t waiting Time \t Turn around time";
    cout<<"\n-------------------------------------------------------------------------------------------------------------------------";
    
    for(int i=0;i<9;i++)
    cout<<"\n\t"<<i<<"\t\t"<<arrival_time[i]<<"\t\t"<<priority[i]<<"\t\t"<<burst_time[i]<<"\t\t"<<completion_time[i]<<"\t\t"<<waiting_time[i]<<"\t\t"<<TAT[i];
   
    cout<<"\n-------------------------------------------------------------------------------------------------------------------------";
    cout<<"\n\n Average waiting time : \t"<<avg_waiting_time;
    cout<<"\n Average Turn around time : \t"<<avg_tat<<"\n\n ";

    print_gannt_chart();
    


    return 0;

}
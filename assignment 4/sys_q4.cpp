#include<bits/stdc++.h>

#include<thread>   

#include <unistd.h>


pthread_t philosopher[5];  //creating 5 philosphers 

pthread_mutex_t forks[5];  

pthread_mutex_t forks[5]; 

/*int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr) :
 Creates a mutex, referenced by mutex, with attributes specified by attr. If attr is NULL, 
 the default mutex attribute (NONRECURSIVE) is used.  */

using namespace std;

bool create_mutex_for_forks()
{
    for(int i=0;i<5;i++)
    {
        int res = pthread_mutex_init(&forks[i],NULL);
        if(res==-1)
            return false;
    }
    return true;
}

void *thread_function(void *n)
    {     
        long  philosopher_no = *(int *)n;
        cout<<" \nPhilosopher "<<philosopher_no<<" is thinking \n";


        //occupy forks if available 
        pthread_mutex_lock(&forks[philosopher_no]); 
        pthread_mutex_lock(&forks[(philosopher_no+1)%5]);   // as for last philosopher second fork no is 0

        cout<<"\nPhilosopher "<<philosopher_no<<" is eating \n";

        //sleep(3);
        //usleep(300);
           
        //release locks occupied by philospher 
        pthread_mutex_unlock(&forks[philosopher_no]);
        pthread_mutex_unlock(&forks[(philosopher_no+1)%5]);

        cout<<"\nPhilosopher "<<philosopher_no<<" Finished eating \n";
   
        return 0;
    }



int main(){

    void *message ;
    cout<<" DINIG PHILOSOPHERS PROBLEM IMPLEMENTATION ";
    cout<<"\n No Of philosophers : 5 ( already specified in question ) " ;

    if( !create_mutex_for_forks() )
    {
        cout<<"\n Mutex creation failed ";
        cout<<"\n Program Terminated ";
        exit(0);
    }
    else    
        cout<<"Mutex creation successfull";

    
    //for thred creation
    //https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_create.html

    for(int i=0;i<5;i++)
        {   
            //cout<<"\n"<<i<<"here \n";
            int res = pthread_create( &philosopher[i] , NULL , &thread_function , &i );
            if( res != 0)
                {
                    cout<<"\n Thread creation error";
                    cout<<"\n Program Terminated ";
                    exit(1);
                }
        }




    for(int i=0;i<5;i++)
    {
        int res = pthread_join( philosopher[i] ,&message );
        if(res!=0)
        {
            cout<<"\n Thread join failed \n";
            exit(1);
        }
    }

    for(int i=0;i<5;i++)
        {
            int res = pthread_mutex_destroy(&forks[i]);
            if(res!=0)
            {
                cout<<"\n Mutex Destroyed \n";
                exit(1);
            }
        } 
    return 0;
}
#include<bits/stdc++.h>
#define ld long double 
using namespace std;

struct inode {
  char name[8];        //file name
  int size;             // file size (in number of blocks)
  int blockPointers[8]; // direct block pointers
  int used;             // 0 => inode is free; 1 => in use
}

inode files[16];   
int current_files_in_system = 0;

void create(string name,int size)
  {
    if(name.size()>8)
      {
        cout<<"\n file name size is large , cannot create file with this name ";
        return;
      }
    if(size>8)
      {
        cout<<"\n file size is large , max file size should be (8kb) ";
        return;
      }
    for(int i = 0; i<current_files_in_system ;i++)
      if(files[i].name == name)
      {
        cout<<"ERROR: File with same Name already exists";
        return;
      }
    files[current_files_in_system].name=name;
    files[current_files_in_system].size=size;


}


int main()
{ 
  int choice  = 0;
  cout<<"1. Create a file \n";
  cout<<"2. Delete a file\n";
  cout<<"3. Read a file \n";
  cout<<"4. write into a file \n";
  cout<<"5. Display System Files\n";
  cout<<"6. exit\n"
  cout<<"Enter your choice : ";
  cin>>choice;
  cout<<"\n";
  while(ch>=1 && ch <=5)
  {
    switch(choice)
    {
      case 1:
          cout<<"Enter file Name:(file name size should be less than 8)";
          cin>>name;
          cout<<"Enter File Size:( in 1Kb)";
          cin>>size1;
          create_file(name,size);

    }
  }

  return 0;
}
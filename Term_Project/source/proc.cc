#include <iostream>
#include <cstring>
#include "U.h"
#include "P.h"
#include "mpi.h"

using namespace std;

int main(int argc, char** argv){
    int p;      // Total number of Processes
    int id;     // Rank or processor ID
    pid_t pid;
	// Initialize MPI ------------------------------
	int ierr = MPI_Init(&argc, &argv);	
	
	// Number of Processes -------------------------
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// Get (this) Process Rank ---------------------
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
    cout << "I am process " << id << " of " << p << endl;

    if (id == 0){
        try {
            U u;
            u.readfile("Hamlet.txt");

            P p(argv[1]);

            for (int i = 0 ; i < u.size() ; i++)
                p.count(u.codepoint(i));
            
            cout << p << endl;
            
        }catch(const string error){
            cout << error << endl;
        }
    }
    
    if (id == 1){
        try {
            U u;
            u.readfile("Hamlet.txt");

            P p(argv[2]);

            for (int i = 0 ; i < u.size() ; i++)
                p.count(u.codepoint(i));
            
            cout << p << endl;
            
        }catch(const string error){
            cout << error << endl;
        }
    }
    
    if (id == 2){
        try {
            U u;
            u.readfile("Hamlet.txt");

            P p(argv[3]);

            for (int i = 0 ; i < u.size() ; i++)
                p.count(u.codepoint(i));
            
            cout << p << endl;
            
        }catch(const string error){
            cout << error << endl;
        }
    }
		
}

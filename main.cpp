#include <iostream>
#include "binary.h"
#include <string>
#include <sstream>
#include <fstream>
#include <csignal>
#include <unistd.h>
bool THREAD_RUNNING = true;
int cin_int(std::string messages);

// contructor for pthread
struct binary_file
{
    int size;
    std::fstream *file; 
};

// Thread process to check all the thread running conditions
void* thread_running(void* threadarg)
{
    using namespace std;
    struct binary_file *binary_data;
    binary_data = reinterpret_cast<binary_file *>(threadarg);
    std::fstream& file = *(binary_data->file);
    while (binary::check_result(file, binary_data->size) == false){
        usleep(10);
    }
    THREAD_RUNNING = false;
    return nullptr;
}
// Thread to modify the files
void* thread_process(void* threadarg)
{
    using namespace std;

    pthread_mutex_lock(&binary::mutex);
    cout << "Thread " << pthread_self() << " starting ..." << endl;
    pthread_mutex_unlock(&binary::mutex);

    struct binary_file *binary_data;
    binary_data = reinterpret_cast<binary_file *>(threadarg);
    std::fstream& file = *(binary_data->file);

    while (THREAD_RUNNING)
        {
            int row = rand() % (binary_data->size);
            int column = rand() % (binary_data->size);

            pthread_mutex_lock(&binary::mutex);
            cout << "row " << row+1 << " column " << column+1 << endl;
            binary::check_set_neighbor(file, row, column, binary_data->size);
            binary::print_matrix(file,binary_data->size);
            cout << endl << endl;
            pthread_mutex_unlock(&binary::mutex);
            // print matrix
            usleep(100);
        }

    pthread_mutex_lock(&binary::mutex);
    cout << "Thread " << pthread_self() <<" ending ... " << endl;
    pthread_mutex_unlock(&binary::mutex);

    return nullptr;
}


int main(int argc, char **argv)
{
    using namespace std;
    cout << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "         Type 'exit' to quit console            " << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
         << endl;

    const int size = cin_int("Enter number of row and column  (n = row*column): ");
    const int num_thread = cin_int("Enter number of thread (M > 0): ");
    cout << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
         << endl;
    cout << "   Create a binary file data.bin with " << size << " row and " << size << " column"
         << endl;
    binary::file_create(size);
    fstream* myFile = new fstream("data.bin", ios::in | ios::out | ios::binary);
    binary::print_matrix(*myFile, size); 
    cout << endl;
    // make thread
    binary_file *thread_data = new binary_file();
    thread_data->size = size;
    thread_data->file = myFile;
    // type cast thread arg term
    void *cp = reinterpret_cast<void *>(thread_data);
    int thread;
    pthread_t threads[num_thread];
    pthread_t parent_thread;
    for (int i = 0; i < num_thread; i++)
    {
        thread = pthread_create(&threads[i], NULL, thread_process, cp);
        if (thread)
        {
            cout << "Unable to create thread, " << thread << endl;
            exit(-1);
        }
    }
    pthread_create(&parent_thread,NULL, thread_running, cp);
    for (int i = 0; i < num_thread; i++)
    {
        thread = pthread_join(threads[i], nullptr);
        if (thread)
        {
            cout << "Unable to join," << thread << endl;
            exit(-1);
        }
    }

    pthread_join(parent_thread, nullptr);

    cout << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
         << endl;
    cout << "   Final matrix:" << endl;
    binary::print_matrix(*myFile, size);
    cout << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl
         << endl;

    delete thread_data;
    myFile->close();
    delete myFile;
    exit(0);
    
}


// function to get int as input from user
int cin_int(std::string messages)
{
    using namespace std;

    string input = "";
    int myNumber = 0;
    while (true)
    {
        //cout << "Please enter a positive number (q to quit): ";
        cout << messages;
        getline(cin, input);
        if (input == "exit" || input == "quit")
            exit(1);
        // This code converts from string to number safely.
        stringstream myStream(input);
        if (myStream >> myNumber)
            if (myStream.rdbuf()->in_avail() == 0) //catches ALL non-numerics
                if (myNumber < 0)
                {
                    cout << ".... Please enter a positive number ....\n\n";
                    continue;
                }
                else
                {
                    return myNumber;
                }

        cout << ".... Invalid number, please try again ....\n\n";
    }
}

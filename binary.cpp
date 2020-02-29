#include <iostream>
#include "binary.h"
#include <fstream>
#include <stdlib.h> // for random
#include <pthread.h>

pthread_mutex_t binary::mutex = PTHREAD_MUTEX_INITIALIZER;

void binary::file_create(const int size)
{
    int n = size * size;
    std::ofstream myFile("data.bin",std::ios::out|std::ios::binary|std::ios::trunc);
    srand (time(NULL));
    for (int i = 0; i < n; i++){
        int value = rand()%2;
        const char value1 = value;
        myFile.write(&value1,1);
    }
    myFile.close();
} 
int binary::target_location(int row, int column, const int size){
    int location = row * size + column;
    return location;     
}

unsigned char binary::get_value(std::fstream& file,int row, int column, const int size){
    while(row < 0){
        row += size;
    }
    while(column < 0 ){
        column += size;
    }
    row = row % size;
    column = column % size;
    int location = target_location(column, row, size);
    file.clear(); 
    file.seekg(location,file.beg);
    char buffer;
    file.read(&buffer,1);
    return (unsigned char) buffer;

}

void binary::set_value(std::fstream& file, int target_location, unsigned char value){
    file.clear();
    file.seekp(target_location, file.beg);
    char buffer = value;
    file.write(&buffer,1);
}

bool binary::check_result(std::fstream& file, const int size){
    pthread_mutex_lock(&mutex);

    unsigned char reference = get_value(file,0,0,size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (get_value(file,i,j,size) != reference){
                pthread_mutex_unlock(&mutex);
                return false;
                }
            }
        }   
    
    pthread_mutex_unlock(&mutex);
    return true;
}

void binary::check_set_neighbor(std::fstream& file, int row, int column, const int size){
    int zero_count = 0;
    int one_count = 0;
    for (int i = row-1; i<row+1; i++){
        for (int j = column-1; j < column+1; j++){
            if ((i == row && j == column) /*|| (i < 0) || (j < 0)*/){
                continue;
            }
            else if (get_value(file, i, j, size)==0){
                zero_count++;
            }
            else{
                one_count++;
            }
        }
    }
    int target_location_1 = target_location(row,column,size);
    if (zero_count < one_count){
        //for (int i = row-1; i<row+1; i++){
            //for (int j = column-1; j < column+1; j++){
                //int set_location = target_location(i,j,size);
                set_value(file,target_location_1,1);
            }
        
    else {
        //for (int i = row-1; i<row+1; i++){
            //for (int j = column-1; j < column+1; j++){
                //int set_location = target_location(i,j,size);
                set_value(file,target_location_1,0);
            }
}

void binary::print_matrix (std::fstream& file, const int size){
    for (int i = 0; i < size; i++){
        std::cout << "      ";
        for (int j = 0; j < size; j++){
            std::cout << "  " << (int)get_value(file, i, j, size) << " ";
        }
        std::cout << std::endl;
    }
}



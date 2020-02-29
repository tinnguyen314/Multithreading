#ifndef BINARY_H
#define BINARY_H
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
class binary
{
    public:
        static void file_create(const int size);
        static int target_location(int row, int column, const int size);
        static char get_value(std::fstream& file,int row, int column, const int size);
        static void set_value(std::fstream& file, int target_location, unsigned char value);
        static bool check_result(std::fstream& file, const int size); //check to see if all array is 0 or 1
        static void check_set_neighbor(std::fstream& file, int row, int column, const int size);
};
#endif
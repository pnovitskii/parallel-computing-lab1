#include <iostream>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <vector>
#include <chrono>

using namespace std;

const int SIZE = 500;
const int THREADS = 256;

void compute(vector<vector<int>>& matrix, vector<int> tasks) {
    for (int k = 0; k < tasks.size(); k++) {
        int min = matrix[0][tasks[k]];
        int cord = 0;
        for (int i = 0; i < SIZE; i++) {
            if (matrix[i][tasks[k]] < min) {
                min = matrix[i][tasks[k]];
                cord = i;
            }
        }
        swap(matrix[cord][tasks[k]], matrix[SIZE - 1 - tasks[k]][tasks[k]]);
    }
}
void doWork(int amount_of_threads) {
    //CREATE MATRIX
    vector<vector<int>> matrix(SIZE, vector<int>(SIZE));
    for (int i = 0; i < SIZE; i++) 
        for (int j = 0; j < SIZE; j++) 
            matrix[i][j] = rand() % 200 - 100;
        
    //ROUND ROBIN
    const int n = SIZE;
    const int k = amount_of_threads;

    const int tasks_per_thread = n / k;
    int remainder = n % k;

    vector<thread> threads;

    int start = 0;
    for (int i = 0; i < k; ++i) {
        if (start > n) break;
        vector<int> tasks;
        if (remainder > 0) {
            tasks.push_back(start);
            --remainder;
            ++start;
        }

        for (int j = 0; j < tasks_per_thread; ++j) {
            tasks.push_back(start);
            ++start;
        }
        
        threads.emplace_back(compute, ref(matrix), tasks);
    }

    for (auto& thread : threads) 
        thread.join();
}
int main()
{
    srand(time(NULL));
    
    for (int i = 2; i <= THREADS; i*=2) {
        //cout << "TASKS: " << SIZE << endl;
        cout << "THREADS: " << setw(3) << i << " ";// << endl << endl;
        auto start = std::chrono::high_resolution_clock::now();
        doWork(i);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        for (int j = 0; j < duration.count() / 150; j++) {
            cout << "#";
        }
        std::cout << " " << duration.count() / 1000 << "." << duration.count() % 1000 << " seconds\n";// " milliseconds" << std::endl;//Time taken by code: 
    }
    
    return 0;
}

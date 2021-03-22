#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;
using namespace std;

std::vector <std::thread> th_vec;
typedef double(*pointFunc)(double);

inline double f(double x)
{
    return  4 / sqrt(4 - (pow(x, 2)));
}

void left_rect(double lower_bound, double upper_bound, int n)  //the decision of the left rectangle integral
{
    double g_sum = 0.0;
    double h = (upper_bound - lower_bound) / n;
    double* y = new double[n];
    double x;
    int i;

#pragma loop(ivdep)
#pragma loop(hint_parallel(4))
    //#pragma loop(no_vector)
    for (i = 0; i < n; i++)
    {
        x = lower_bound + h * i;
        y[i] = 4 / sqrt(4 - (pow(x, 2)));
    }

    for (i = 0; i < n; i++)
    {
        g_sum += y[i];
    }

    double answ1 = g_sum * h;
    cout << "Left integral: " << answ1 << endl;
    cout << "Count of intervals: " << n << endl;
    cout << "Accuracy: " << 100 - (abs(2 * M_PI / 3 - answ1) / 2 * M_PI / 3 * 100) << "%" << endl;

}

void right_rect(double lower_bound, double upper_bound, int n)   //the decision of the left rectangle integral
{
    double g_sum = 0.0;
    double h = (upper_bound - lower_bound) / n;
    double* y = new double[n];
    double x;
    int i;

#pragma loop(ivdep)
#pragma loop(hint_parallel(4))
    //#pragma loop(no_vector)
    for (i = 1; i <= n; i++)
    {
        x = lower_bound + h * i;
        y[i - 1] = 4 / sqrt(4 - (pow(x, 2)));
    }

    for (i = 0; i < n; i++)
    {
        g_sum += y[i];
    }

    double answ2 = g_sum * h;

    cout << "Right integral: " << answ2 << endl;
    cout << "Count of intervals: " << n << endl;
    cout << "Accuracy: " << 100 - (abs(2 * M_PI / 3 - answ2) / 2 * M_PI / 3 * 100) << "%  " << endl;

}

int main() {
    double lower_bound = 0, upper_bound = 1;
    int n;
    int intervals[] = { 100, 1000, 10000, 100000, 1000000 };   //count of interval

    for (int i = 0; i < sizeof(intervals) / sizeof(int); i++) {
        n = intervals[i];
        cout << fixed;
        cout << setprecision(10);

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        thread thread1(left_rect, lower_bound, upper_bound, n);
        thread thread2(right_rect, lower_bound, upper_bound, n);

        // left_rect(lower_bound, upper_bound, n);
        // right_rect(lower_bound, upper_bound, n);

        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> duration = (t2 - t1);
        cout << "Duration is: " << duration.count() << " sec" << endl << endl;
        thread1.join();   // splitting
        thread2.join();   // into the threads
    }

    system("pause");
}
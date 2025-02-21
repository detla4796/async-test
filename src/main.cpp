#include <iostream>
#include <future>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;
using namespace filesystem;

void gen_vector(int sz, function<void(vector<int>)>callback)
{
    vector<int> vec;
    srand(time(0));
    for (int i = 0; i < sz; i++)
    {
        vec.push_back(rand());
    }
    callback(vec);
}

void summa(const vector<int>& vec, function<void(int)>callback)
{
    int sm = 0;
    for (auto i : vec)
    {
        sm += i;
    }
    callback(sm);
}

void proizved(const vector<int>& vec, function<void(int)>callback)
{
    int sm = 0;
    for (auto i : vec)
    {
        sm *= i;
    }
    callback(sm);
}

int main()
{
    auto task = async(launch::async, [](function<void(vector<int>)>callback) 
    {
        gen_vector(100, callback);
    },
    [](vector<int> vec)
    {
        for (int sn : vec)
        {
            cout << sn << endl;
        }
        auto sum = async(launch::async, [vec](function<void(int)>callback)
        {
            summa(vec, callback);
        },
        [] (int sum)
        {
            cout << endl << "Summa: ";
            cout << sum;
        });
        auto przv = async(launch::async, [vec](function<void(int)>callback)
        {
            proizved(vec, callback);
        },
        [] (int proiz)
        {
            cout << endl << "Proizvedenie: ";
            cout << proiz;
        });
        sum.wait();
        przv.wait(); 
    });
    task.wait();
}
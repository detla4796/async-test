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

void proc_vector(const vector<int>& vec, function<void(vector<int>)> callback)
{
    vector<int> proc(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
    {
        proc[i] = vec[i] * 2;
    }
    callback(proc);
}

int main()
{
    auto task = async(launch::async, [](function<void(vector<int>)>callback) 
    {
        gen_vector(10, callback);
    },
    [](vector<int> vec)
    {
        cout << "First mass: " << endl;
        for (int num : vec)
        {
            cout << num << " ";
        }
        cout << endl;

        auto proc_task = async(launch::async, [vec](function<void(vector<int>)>callback)
        {
            proc_vector(vec, callback);
        },
        [](vector<int> proc)
        {
            cout << "Changed mass: " << endl;
            for (int num : proc)
            {
                cout << num << " ";
            }
            cout << endl;
        });
        proc_task.wait();
    });
    task.wait();
}
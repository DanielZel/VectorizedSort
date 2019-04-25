#include <iostream> 
#include <chrono>

template <class T, class Compare>
bool is_sorted(T arr, Compare compare, int cols = 1)
{
    for (auto i = 0; i < arr.size() - 1 - cols ; ++i)
    {
        if (!compare(arr[i], arr[(i + cols)]))
            return false;
    }
    return true;
}

template <class T>
void randomize(T &arr, int lbound, int ubound)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(lbound, ubound);

    for (auto &el : arr)
    {
        el = dis(gen);
    }
}

template <typename F>
unsigned int measure_ms(F f)
{
    using namespace std;
    auto t1 = chrono::high_resolution_clock::now();
    f();
    auto t2 = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
}

template <typename T>
void merge_two_cols(T *data_in_0, T *data_in_1, unsigned step, T* data_out, size_t n_in)
{
    size_t last=0;
    T *p0 = data_in_0;
    T *p1 = data_in_1;
    T *p0_end = data_in_0 + n_in;
    T *p1_end = data_in_1 + n_in;

    T v0 = *p0;
    T v1 = *p1;
    do
    {
        if (v0 > v1)
        {
            data_out[last++] = v0;
            p0+=step;
            v0 = *p0;
        }
        else
        {
            data_out[last++] = v1;
            p1+=step;
            v1 = *p1;
        }
    }while ((p0 < p0_end) && (p1 < p1_end));
}
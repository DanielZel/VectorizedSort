#include <arm_neon.h>
#include <array>
#include <random>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>
#include <functional>
#include <boost/align/aligned_allocator.hpp>
#include <utils.hpp>


using namespace std;
using dtype = int32_t;

const size_t MIN_TEST_VEC_SIZE = 32;
const size_t MAX_TEST_VEC_SIZE = 65536;
const dtype  MAX_ELEMENT_VALUE = 1000;


void bitonic_sort(int32_t *data_in, int32_t *data_out, int32_t n)
{
    static vector<int32_t> merged_0_1(MAX_TEST_VEC_SIZE); 
    static vector<int32_t> merged_2_3(MAX_TEST_VEC_SIZE);

    constexpr int vector_width_in_bytes =  16; //quadword=128bit
    constexpr int vector_width =  vector_width_in_bytes / sizeof(data_in[0]);
    int n_div_vec_width= n / vector_width;
    int log2n = static_cast<int>(log2(n_div_vec_width));
    assert(exp2(log2n) == n_div_vec_width);

    for (auto i = 0; i < log2n ; ++i)
    {
        for (auto j = i ; j >= 0; --j)
        {
            int arrow_len = 1<<j;
            for (auto k = 0; k < n_div_vec_width/2; k++)
            {
                bool is_up =  k & (1<<i);

                uint32_t  mask = (1 << j) - 1;
                size_t upper_idx = ((k & ~mask) << 1) | (k & mask);
                size_t lower_idx = upper_idx + arrow_len;
                int32x4_t v_upper = vld1q_s32(&data_in[upper_idx * vector_width]); 
                int32x4_t v_lower = vld1q_s32(&data_in[(upper_idx + arrow_len)*vector_width]); 
                int32x4_t v_min = vminq_s32(v_upper, v_lower);
                int32x4_t v_max = vmaxq_s32(v_upper, v_lower);
                auto max_idx = (upper_idx + static_cast<int>(is_up) * arrow_len) * vector_width;
                auto min_idx = (upper_idx + static_cast<int>(!is_up) * arrow_len) * vector_width;
                vst1q_s32(&data_in[max_idx], v_max);
                vst1q_s32(&data_in[min_idx], v_min);
            }
        }
    }

    int32_t *to_merge[4] = {data_in, data_in+1, data_in+2, data_in+3};

    merge_two_cols(to_merge[0], to_merge[1], 4, merged_0_1.data(), n >> 2);
    merge_two_cols(to_merge[2], to_merge[3], 4, merged_2_3.data(), n >> 2);

    to_merge[0] = merged_0_1.data();
    to_merge[1] = merged_2_3.data();
    merge_two_cols(to_merge[0], to_merge[1], 1, data_out, n>>1 );
}

int main()
{
    for (auto vec_size = MIN_TEST_VEC_SIZE; vec_size <= MAX_TEST_VEC_SIZE; vec_size *= 2)
    {
        using aligned_vector = vector<dtype,boost::alignment::aligned_allocator<dtype, 128>>;
        aligned_vector vec(vec_size);
        randomize(vec, -MAX_ELEMENT_VALUE, MAX_ELEMENT_VALUE);
        aligned_vector vec_cpy{vec};
        aligned_vector bitonic_out(vec_size);
        int time_ref = measure_ms([&vec](){ std::sort(vec.begin(), vec.end(), greater<dtype>());});
        int time_bitonic = measure_ms([&vec_cpy, &bitonic_out ,vec_size](){ bitonic_sort(vec_cpy.data(), bitonic_out.data(), vec_size);});
        cout << "acceleration for " << vec_size << " elements = " << (float)time_ref/time_bitonic << endl;
        assert(is_sorted(vec, greater_equal<dtype>()));
        assert(is_sorted(vec_cpy, greater_equal<dtype>(), 4));
        //for (auto &el : bitonic_result) cout << el << " ";
        assert(is_sorted(bitonic_out, greater_equal<dtype>()));
    }
    return 0;
}
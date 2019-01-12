#pragma once

#include <algorithm>
#include <numeric>

namespace htwk {

template <class Container, class OutputIterator>
OutputIterator copy(const Container& in, OutputIterator out) {
    return std::copy(std::begin(in), std::end(in), out);
}

template <class Container, class OutputIterator, class UnaryPredicate>
void transform(Container& in, OutputIterator out, UnaryPredicate pred) {
    std::transform(std::begin(in), std::end(in), out, pred);
}

// Equivalent to
// for (size_t i = 0; i < in1.size(); i++)
//     in1[i] += in2[i];
template <class Container1, class Container2>
void pointwise_pluseq(Container1& in1, const Container2& in2) {
    std::transform(std::begin(in1), std::end(in1), std::begin(in2), std::begin(in1),
                   std::plus<typename Container1::value_type>());
}

// Equivalent to
// for (size_t i = 0; i < in1.size(); i++)
//     in1[i] -= in2[i];
template <class Container1, class Container2>
void pointwise_minuseq(Container1& in1, const Container2& in2) {
    std::transform(std::begin(in1), std::end(in1), std::begin(in2), std::begin(in1),
                   std::minus<typename Container1::value_type>());
}

template <class Container>
typename Container::value_type sum(const Container& in) {
    return std::accumulate(std::begin(in), std::end(in), 0);
}

}  // namespace htwk

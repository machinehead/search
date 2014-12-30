#pragma once

namespace Nstd {

template<class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    while (first != last) {
        *result++ = *first++;
    }
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result)
{
    while (true) {
        if (first1==last1) return Nstd::copy(first2, last2, result);
        if (first2==last2) return Nstd::copy(first1, last1, result);
        *result++ = (*first2 < *first1) ? *first2++ : *first1++;
    }
}

}


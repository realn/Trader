#pragma once

#include <memory>

namespace eco {
  namespace helper {
    template<int... Is>
    struct seq {};

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

    template<int ... Is>
    struct gen_seq<0, Is...> { using type = seq<Is...>; };
  }
}
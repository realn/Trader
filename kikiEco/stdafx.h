#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <CBStr/StringEx.h>

using namespace std::literals;

namespace std {
  template<class _Cont, class _Pred>
  void erase_if(_Cont cont, _Pred pred) {
    auto it = std::begin(cont);
    while(it != std::end(cont)) {
      it = pred(*it) ? cont.erase(it) : it++;
    }
  }
}
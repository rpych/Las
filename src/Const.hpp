#pragma once
#include <string>

namespace las
{
  struct LAS_CPP
  {
    static inline const std::string LAS_BEGIN {"//^^las begin^^"};
    static inline const std::string LAS_END {"//^^las end^^"};
    static inline const std::string LAS_SINGLE {"//^^las^^"};
    static inline const std::string LAS_SUBST_BEGIN {"//^^las sub begin^^"};
    static inline const std::string LAS_SUBST_END {"//^^las sub end^^"};
  };
  
}
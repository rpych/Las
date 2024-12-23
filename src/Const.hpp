#pragma once
#include <string>

namespace las
{
  //std::type_identity
  struct LasLanguage
  {
    LasLanguage(std::string_view _LAS_BEGIN,
                 std::string_view _LAS_END,
                 std::string_view _LAS_SINGLE,
                 std::string_view _LAS_SUBST_BEGIN,
                 std::string_view _LAS_SUBST_END) : LAS_BEGIN(_LAS_BEGIN), LAS_END(_LAS_END), LAS_SINGLE(_LAS_SINGLE),
                                                    LAS_SUBST_BEGIN(_LAS_SUBST_BEGIN), LAS_SUBST_END(_LAS_SUBST_END) {};
    const std::string LAS_BEGIN{};
    const std::string LAS_END{};
    const std::string LAS_SINGLE{};
    const std::string LAS_SUBST_BEGIN{};
    const std::string LAS_SUBST_END{};
  };

  struct LasCpp : public LasLanguage
  {
    LasCpp() : LasLanguage(LAS_BEGIN, LAS_END, LAS_SINGLE, LAS_SUBST_BEGIN, LAS_SUBST_END) {}
    static inline const std::string LAS_BEGIN {"//^^las begin^^"};
    static inline const std::string LAS_END {"//^^las end^^"};
    static inline const std::string LAS_SINGLE {"//^^las^^"};
    static inline const std::string LAS_SUBST_BEGIN {"//^^las sub begin^^"};
    static inline const std::string LAS_SUBST_END {"//^^las sub end^^"};
  };
  
}
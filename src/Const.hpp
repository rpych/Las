#pragma once
#include <string>

namespace las
{

const inline std::string LasHiddenDirectory{".las_directory"};
constexpr inline uint16_t BackupMaxEntries{4}; 

  #define __GIT_VCS__ 1
  #ifdef  __GIT_VCS__
    #define GET_VCS las::commands::common::GitCmd
  #endif

  enum class Language
  {
    CPP,
    ELIXIR,
    PYTHON
  };

  struct LasLanguage
  {
    LasLanguage(std::string_view _COMMENT) : COMMENT(_COMMENT) {}
    
    bool containsLasIndOpen(std::string_view l) { return l.find(LAS_BEGIN) != std::string::npos; }
    bool containsLasIndClose(std::string_view l) { return l.find(LAS_END) != std::string::npos; }
    bool containsSingleLasInd(std::string_view l) { return l.find(LAS_SINGLE) != std::string::npos; }
    bool containsLasIndSubstOpen(std::string_view l) { return l.find(LAS_SUBST_BEGIN) != std::string::npos; }
    bool containsLasIndSubstClose(std::string_view l) { return l.find(LAS_SUBST_END) != std::string::npos; }

    const std::string COMMENT{};
    static inline const std::string LAS_BEGIN {"^^las begin^^"};
    static inline const std::string LAS_END {"^^las end^^"};
    static inline const std::string LAS_SINGLE {"^^las^^"};
    static inline const std::string LAS_SUBST_BEGIN {"^^las sub begin^^"};
    static inline const std::string LAS_SUBST_END {"^^las sub end^^"};
  };

  struct LasCpp : public LasLanguage
  {
    LasCpp() : LasLanguage(COMMENT) {}
    static inline const std::string COMMENT{"//"};
  };
  
}

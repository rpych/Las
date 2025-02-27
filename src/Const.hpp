#pragma once
#include <string>
#include <format>

namespace las
{

const inline std::string LasCmdOutputFile{"las_output.diff"};
const inline std::string LasStateBackupDir{".las_directory/backup"};
const inline std::string StagedAreaFilesBackup{".las_directory/areas/CmdStagedAreaBackup.diff"};
const inline std::string WorkAreaFilesBackup{".las_directory/areas/CmdWorkAreaBackup.diff"};
constexpr inline uint16_t BackupMaxEntries{4}; 

  #ifdef LAS_DEBUG
    #define logLasDebug(s, ...) \
      std::cout<<std::format(s __VA_OPT__(,) __VA_ARGS__)<<std::endl;
  #else
    #define logLasDebug(s, ...)
  #endif

  #define logLasError(s, ...) \
    std::cerr<<std::format(s __VA_OPT__(,) __VA_ARGS__)<<std::endl;


  #define __GIT_VCS__ 1
  #ifdef  __GIT_VCS__
    #define GET_VCS las::commands::common::GitCmd
  #endif

  enum class Language
  {
    CPP,
    ELIXIR,
    PYTHON,
    JAVA,
    DEFAULT
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

  struct LasElixir : public LasLanguage
  {
    LasElixir() : LasLanguage(COMMENT) {}
    static inline const std::string COMMENT{"#"};
  };

  struct LasPython : public LasLanguage
  {
    LasPython() : LasLanguage(COMMENT) {}
    static inline const std::string COMMENT{"#"};
  };

  struct LasJava : public LasLanguage
  {
    LasJava() : LasLanguage(COMMENT) {}
    static inline const std::string COMMENT{"//"};
  };

  struct LasDefault : public LasLanguage
  {
    LasDefault() : LasLanguage(COMMENT) {}
    static inline const std::string COMMENT{"#"};
  };
  
}

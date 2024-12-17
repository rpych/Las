#pragma once
#include <string_view>
#include <variant>
#include "../../../Const.hpp"
//#include "HunksParser.hpp"
#include "../Utils.hpp"

namespace las::commands::common
{

struct NormalArea{};
struct HunkArea{};
struct HunkSubstAreaPossible{};
struct HunkSubstArea{};
struct InvalidArea{};

struct NormalLineEvent{};
struct HunkBeginEvent{};
struct HunkEndEvent{};
struct HunkSingleEvent{};
struct HunkSubstBeginEvent{};
struct HunkSubstEndEvent{};

template<typename LANG>
class LasParserSmManager
{
  //using LasComment = HunksParser::LasComment;
  //using LasHunk = HunksParser::LasHunk;
  using LasParserState = std::variant<NormalArea, HunkArea, HunkSubstAreaPossible, HunkSubstArea, InvalidArea>;
  using LasHunkEvent   = std::variant<NormalLineEvent, HunkBeginEvent, HunkEndEvent, HunkSingleEvent, HunkSubstBeginEvent, HunkSubstEndEvent>;

public:
  LasParserSmManager() /*: lpSm(*this)*/ {}

  //template<typename SM_LANG>
  struct LasParserSm
  {
    LasParserSm(LasParserSmManager<LANG>& lpSmManager): sm(lpSmManager) {/*std::cout<<"CONSTRUCT:"<<sm.line;*/}
    LasParserSmManager<LANG>& sm;

    LasParserState operator()(NormalArea&, NormalLineEvent&)
    {
      std::cout<<"NormalArea and NormalLineEvent: "<<sm.line;//<<std::endl;
      return NormalArea{};
    }
    LasParserState operator()(NormalArea&, HunkBeginEvent&)
    {
      std::cout<<"parseForHunks::containsLasIndOpen with "<<sm.line;//<<std::endl;
      sm.commIndicators.push(LasComment{Comment::OPENING, sm.lineNum});
      return HunkArea{};
    }
    LasParserState operator()(NormalArea&, HunkSingleEvent&)
    {
      //std::cout<<"parseForHunks::containsSingleLasInd with "<<sm.line;//<<std::endl;
      auto const markEndIdx{sm.line.find(LANG::LAS_SINGLE) + LANG::LAS_SINGLE.length()};
      auto const linePart{sm.line.substr(markEndIdx)};
      auto startTrimIdx{linePart.find_first_not_of(" \t")};
      auto const leadContent{linePart.substr(startTrimIdx)};
      std::string substContent{leadContent.substr(0, (leadContent.find_last_not_of(" \n\t")+1))};
      std::cout<<"parseForHunks::containsSingleLasInd with "<<sm.line<<", markEndIdx:"<<markEndIdx<<", linePart:"<<linePart<<", startTrimIdx:"<<startTrimIdx<<", leadContent:"<<leadContent<<", substContent:"<<substContent<<", sizeSubstC:"<<substContent.length();
      sm.hunks.push_back(LasHunk{.substContent=substContent, .opComment=LasComment{Comment::OPENING, sm.lineNum}});
      if (substContent.size() > 0) { return NormalArea{}; }
      return HunkSubstAreaPossible{};
    }
    LasParserState operator()(HunkArea&, NormalLineEvent&)
    {
      std::cout<<"HunkArea and NormalLineEvent: "<<sm.line;//<<std::endl;
      return HunkArea{};
    }
    LasParserState operator()(HunkArea&, HunkEndEvent&)
    {
      std::cout<<"parseForHunks::containsLasIndClose with "<<sm.line;//<<std::endl;
      if (sm.commIndicators.empty())
      {
        std::cout<<"Any opening LAS comment does not exist in the file"<<std::endl;
        return InvalidArea{};
      }
      auto const& openingComment = sm.commIndicators.top();
      sm.commIndicators.pop();
      if (openingComment.comment != Comment::OPENING)
      {
        std::cout<<"Lacking LAS opening comment for LAS closing comment "<<std::endl;
        return InvalidArea{};
      }
      sm.hunks.push_back(LasHunk{.opComment=openingComment, .clComment=std::make_optional(LasComment{Comment::CLOSING, sm.lineNum})});
      return HunkSubstAreaPossible{};
    }
    LasParserState operator()(HunkSubstAreaPossible&, NormalLineEvent&)
    {
      std::cout<<"HunkSubstAreaPossible and NormalLineEvent: "<<sm.line<<std::endl;
      return NormalArea{};
    }
    LasParserState operator()(HunkSubstAreaPossible&, HunkSubstBeginEvent&)
    {
      std::cout<<"HunkSubstAreaPossible and HunkSubstBeginEvent: "<<sm.line;//<<std::endl;
      auto const markEndIdx{sm.line.find(LANG::LAS_SUBST_BEGIN) + LANG::LAS_SUBST_BEGIN.length()};
      auto const linePart{sm.line.substr(markEndIdx)};
      //auto const lastValidIdx{linePart.find_last_not_of(" \n\t")};
      //auto const lineEndIdx{linePart.size() > 0 ? (lastValidIdx != std::npos ? lastValidIdx+1 : 0) : 0};
      auto const lineEndIdx{linePart.find_last_not_of(" \n\t")};
      auto const substContent{linePart.substr(0, (lineEndIdx != std::string::npos ? lineEndIdx+1 : 0))};
      sm.substContent += substContent;
      return HunkSubstArea{};
    }
    LasParserState operator()(HunkSubstArea&, NormalLineEvent&)
    {
      std::cout<<"HunkSubstArea and NormalLineEvent: "<<sm.line;//<<std::endl;
      auto const lineEndIdx{sm.line.find_first_not_of("//")};
      auto const substContent{sm.line.substr((lineEndIdx != std::string::npos ? lineEndIdx : 0))};
      sm.substContent += substContent;
      return HunkSubstArea{};
    }
    LasParserState operator()(HunkSubstArea&, HunkSubstEndEvent&)
    {
      auto const markStartIdx{sm.line.find(LANG::LAS_SUBST_END)};
      auto const linePart{sm.line.substr(0, markStartIdx)};
      sm.substContent += linePart;
      auto& lastHunk = sm.hunks.back();
      lastHunk.substContent = sm.substContent;
      if (lastHunk.clComment) { lastHunk.clComment->line = sm.lineNum; }
      else { lastHunk.clComment = LasComment{Comment::CLOSING, sm.lineNum}; }
      std::cout<<"HunkSubstArea and HunkSubstEndEvent: "<<sm.line<<", opLineNum:"<<lastHunk.opComment.line<<", endlineNum:"<<lastHunk.clComment->line;//<<std::endl;
      sm.substContent.clear();
      return NormalArea{};
    }
    LasParserState operator()(auto&, auto&)
    {
      std::cout<<"InvalidArea: "<<sm.line<<std::endl;
      return InvalidArea{};
    }
  };

  void assignEventForLine(std::string_view line)
  {
    if (containsSingleLasInd(line)) { currentEvent = HunkSingleEvent{}; }
    else if (containsLasIndOpen(line)) { currentEvent = HunkBeginEvent{}; }
    else if (containsLasIndClose(line)) { currentEvent = HunkEndEvent{}; }
    else if(containsLasIndSubstOpen(line)) { currentEvent = HunkSubstBeginEvent{}; }
    else if(containsLasIndSubstClose(line)) { currentEvent = HunkSubstEndEvent{}; }
    else { currentEvent = NormalLineEvent{}; }
  }

  bool processLine(std::string_view _line, uint64_t _lineNum)
  {
    line = _line;
    lineNum = _lineNum;
    assignEventForLine(line);
    currentState = std::visit(LasParserSm{*this}, currentState, currentEvent);//
    //std::cout<<"PYCH::currentState: "<<currentState.index()<<", currentEvent: "<<currentEvent.index()<<",:"<<line<<", hunksSize:"<<hunks.size()<<std::endl;
    if (std::holds_alternative<InvalidArea>(currentState))
    {
      std::cout<<"ERROR when parsing LAS HUNK:: lineNum:"<<lineNum<<", line: "<<line<<std::endl;
      return false;
    }
    return true;
  }

  std::vector<LasHunk> hunks{};
private:
  LasParserState currentState{NormalArea{}};
  LasHunkEvent currentEvent{};
  std::stack<LasComment> commIndicators{};
  std::string substContent{};
  std::string_view line{};
  uint64_t lineNum{};
  //LasParserSm lpSm;//
};




}
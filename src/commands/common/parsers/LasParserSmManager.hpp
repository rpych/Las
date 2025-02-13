#pragma once
#include <string_view>
#include <variant>
#include "../../../Const.hpp"
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

class LasParserSmManager
{
  using LasParserState = std::variant<NormalArea, HunkArea, HunkSubstAreaPossible, HunkSubstArea, InvalidArea>;
  using LasHunkEvent   = std::variant<NormalLineEvent, HunkBeginEvent, HunkEndEvent, HunkSingleEvent, HunkSubstBeginEvent, HunkSubstEndEvent>;

public:
  LasParserSmManager(std::shared_ptr<LasLanguage> lang) : lang(lang) {}

  struct LasParserSm
  {
    LasParserSm(LasParserSmManager& lpSmManager): sm(lpSmManager) {}
    LasParserSmManager& sm;

    LasParserState operator()(NormalArea&, NormalLineEvent&)
    {
      logLasDebug("NormalArea and NormalLineEvent: {}", sm.line);
      return NormalArea{};
    }
    LasParserState operator()(NormalArea&, HunkBeginEvent&)
    {
      logLasDebug("NormalArea and HunkBeginEvent: {}", sm.line);
      sm.commIndicators.push(LasComment{Comment::OPENING, sm.lineNum});
      return HunkArea{};
    }
    LasParserState operator()(NormalArea&, HunkSingleEvent&)
    {
      logLasDebug("NormalArea and HunkSingleEvent: {}", sm.line);
      std::string substContent {std::move(getSubstContentFromRightSide(sm.line, sm.lang->LAS_SINGLE))};
      sm.hunks.push_back(LasHunk{.substContent=substContent, .opComment=LasComment{Comment::OPENING, sm.lineNum},
                                                             .clComment=LasComment{Comment::CLOSING, sm.lineNum}});
      if (substContent.size() > 0) { return NormalArea{}; }
      return HunkSubstAreaPossible{};
    }
    LasParserState operator()(HunkArea&, NormalLineEvent&)
    {
      logLasDebug("HunkArea and NormalLineEvent: {}", sm.line);
      return HunkArea{};
    }
    LasParserState operator()(HunkArea&, HunkEndEvent&)
    {
      logLasDebug("HunkArea and HunkEndEvent: {}", sm.line);
      if (sm.commIndicators.empty())
      {
        logLasError("Any opening LAS comment does not exist in the file: {}", sm.line);
        return InvalidArea{};
      }
      auto const& openingComment = sm.commIndicators.top();
      sm.commIndicators.pop();
      if (openingComment.comment != Comment::OPENING)
      {
        logLasError("Lacking LAS opening comment for LAS closing comment: {}", sm.line);
        return InvalidArea{};
      }
      sm.hunks.push_back(LasHunk{.opComment=openingComment, .clComment=LasComment{Comment::CLOSING, sm.lineNum}});
      return HunkSubstAreaPossible{};
    }
    LasParserState operator()(HunkSubstAreaPossible&, NormalLineEvent&)
    {
      logLasDebug("HunkSubstAreaPossible and NormalLineEvent: {}", sm.line);
      return NormalArea{};
    }
    LasParserState operator()(HunkSubstAreaPossible&, HunkSubstBeginEvent&)
    {
      logLasDebug("HunkSubstAreaPossible and HunkSubstBeginEvent: {}", sm.line);
      std::string substContent {std::move(getSubstContentFromRightSide(sm.line, sm.lang->LAS_SUBST_BEGIN))};
      sm.substContent += substContent;
      return HunkSubstArea{};
    }
    LasParserState operator()(HunkSubstArea&, NormalLineEvent&)
    {
      logLasDebug("HunkSubstArea and NormalLineEvent: {}", sm.line);
      std::string substContent {std::move(getNormalLineSubstContent(sm.line, sm.lang->COMMENT))};
      sm.substContent += substContent;
      return HunkSubstArea{};
    }
    LasParserState operator()(HunkSubstArea&, HunkSubstEndEvent&)
    {
      logLasDebug("HunkSubstArea and HunkSubstEndEvent: {}", sm.line);
      std::string substContent {std::move(getSubstContentFromLeftSide(sm.line, sm.lang->LAS_SUBST_END, sm.lang->COMMENT))};
      sm.substContent += substContent;
      auto& lastHunk = sm.hunks.back();
      lastHunk.substContent = sm.substContent;
      lastHunk.clComment.line = sm.lineNum;
      sm.substContent.clear();
      return NormalArea{};
    }
    LasParserState operator()(auto&, auto&)
    {
      logLasDebug("InvalidArea: {}", sm.line);
      return InvalidArea{};
    }
  };

  void assignEventForLine(std::string_view line)
  {
    if (lang->containsSingleLasInd(line)) { currentEvent = HunkSingleEvent{}; }
    else if (lang->containsLasIndOpen(line)) { currentEvent = HunkBeginEvent{}; }
    else if (lang->containsLasIndClose(line)) { currentEvent = HunkEndEvent{}; }
    else if(lang->containsLasIndSubstOpen(line)) { currentEvent = HunkSubstBeginEvent{}; }
    else if(lang->containsLasIndSubstClose(line)) { currentEvent = HunkSubstEndEvent{}; }
    else { currentEvent = NormalLineEvent{}; }
  }

  bool processLine(std::string_view _line, uint64_t _lineNum)
  {
    line = _line;
    lineNum = _lineNum;
    assignEventForLine(line);
    currentState = std::visit(LasParserSm{*this}, currentState, currentEvent);
    if (std::holds_alternative<InvalidArea>(currentState))
    {
      logLasError("Error when parsing LAS hunk in line {}:{}", lineNum, line);
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
  std::shared_ptr<LasLanguage> lang{};
};




}

// Generated from Scene.g4 by ANTLR 4.13.1


#include "SceneVisitor.h"

#include "SceneParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SceneParserStaticData final {
  SceneParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SceneParserStaticData(const SceneParserStaticData&) = delete;
  SceneParserStaticData(SceneParserStaticData&&) = delete;
  SceneParserStaticData& operator=(const SceneParserStaticData&) = delete;
  SceneParserStaticData& operator=(SceneParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sceneParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
SceneParserStaticData *sceneParserStaticData = nullptr;

void sceneParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sceneParserStaticData != nullptr) {
    return;
  }
#else
  assert(sceneParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SceneParserStaticData>(
    std::vector<std::string>{
      "file", "main", "pathdef", "calcdef", "stat", "walk", "expr", "klamKon", 
      "number", "var"
    },
    std::vector<std::string>{
      "", "'begin'", "'end'", "'a'", "'^'", "'*'", "'/'", "'+'", "'-'", 
      "'|'", "'('", "')'", "'walk'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "Walk", "Num", "Float", 
      "ID", "IncID", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,17,115,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,1,0,1,0,5,0,23,8,0,10,0,12,0,26,9,0,1,0,1,0,1,0,
  	5,0,31,8,0,10,0,12,0,34,9,0,1,1,1,1,5,1,38,8,1,10,1,12,1,41,9,1,1,1,1,
  	1,1,2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,5,1,6,1,6,1,6,3,6,57,8,6,1,6,1,6,
  	1,6,3,6,62,8,6,1,6,1,6,3,6,66,8,6,1,6,1,6,1,6,3,6,71,8,6,1,6,1,6,3,6,
  	75,8,6,1,6,1,6,1,6,3,6,80,8,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,90,
  	8,6,1,6,1,6,1,6,1,6,1,6,1,6,5,6,98,8,6,10,6,12,6,101,9,6,1,7,1,7,1,7,
  	1,7,1,8,1,8,3,8,109,8,8,1,9,1,9,3,9,113,8,9,1,9,0,1,12,10,0,2,4,6,8,10,
  	12,14,16,18,0,0,125,0,24,1,0,0,0,2,35,1,0,0,0,4,44,1,0,0,0,6,46,1,0,0,
  	0,8,48,1,0,0,0,10,50,1,0,0,0,12,89,1,0,0,0,14,102,1,0,0,0,16,108,1,0,
  	0,0,18,112,1,0,0,0,20,23,3,4,2,0,21,23,3,6,3,0,22,20,1,0,0,0,22,21,1,
  	0,0,0,23,26,1,0,0,0,24,22,1,0,0,0,24,25,1,0,0,0,25,27,1,0,0,0,26,24,1,
  	0,0,0,27,32,3,2,1,0,28,31,3,4,2,0,29,31,3,6,3,0,30,28,1,0,0,0,30,29,1,
  	0,0,0,31,34,1,0,0,0,32,30,1,0,0,0,32,33,1,0,0,0,33,1,1,0,0,0,34,32,1,
  	0,0,0,35,39,5,1,0,0,36,38,3,8,4,0,37,36,1,0,0,0,38,41,1,0,0,0,39,37,1,
  	0,0,0,39,40,1,0,0,0,40,42,1,0,0,0,41,39,1,0,0,0,42,43,5,2,0,0,43,3,1,
  	0,0,0,44,45,5,3,0,0,45,5,1,0,0,0,46,47,5,3,0,0,47,7,1,0,0,0,48,49,3,10,
  	5,0,49,9,1,0,0,0,50,51,5,12,0,0,51,52,3,12,6,0,52,11,1,0,0,0,53,56,6,
  	6,-1,0,54,57,3,14,7,0,55,57,3,16,8,0,56,54,1,0,0,0,56,55,1,0,0,0,57,58,
  	1,0,0,0,58,61,5,4,0,0,59,62,3,14,7,0,60,62,3,16,8,0,61,59,1,0,0,0,61,
  	60,1,0,0,0,62,90,1,0,0,0,63,66,3,14,7,0,64,66,3,16,8,0,65,63,1,0,0,0,
  	65,64,1,0,0,0,66,67,1,0,0,0,67,70,5,5,0,0,68,71,3,14,7,0,69,71,3,16,8,
  	0,70,68,1,0,0,0,70,69,1,0,0,0,71,90,1,0,0,0,72,75,3,14,7,0,73,75,3,16,
  	8,0,74,72,1,0,0,0,74,73,1,0,0,0,75,76,1,0,0,0,76,79,5,6,0,0,77,80,3,14,
  	7,0,78,80,3,16,8,0,79,77,1,0,0,0,79,78,1,0,0,0,80,90,1,0,0,0,81,82,5,
  	9,0,0,82,83,3,12,6,0,83,84,5,9,0,0,84,90,1,0,0,0,85,86,5,8,0,0,86,90,
  	3,16,8,0,87,90,3,16,8,0,88,90,3,18,9,0,89,53,1,0,0,0,89,65,1,0,0,0,89,
  	74,1,0,0,0,89,81,1,0,0,0,89,85,1,0,0,0,89,87,1,0,0,0,89,88,1,0,0,0,90,
  	99,1,0,0,0,91,92,10,6,0,0,92,93,5,7,0,0,93,98,3,12,6,7,94,95,10,5,0,0,
  	95,96,5,8,0,0,96,98,3,12,6,6,97,91,1,0,0,0,97,94,1,0,0,0,98,101,1,0,0,
  	0,99,97,1,0,0,0,99,100,1,0,0,0,100,13,1,0,0,0,101,99,1,0,0,0,102,103,
  	5,10,0,0,103,104,3,12,6,0,104,105,5,11,0,0,105,15,1,0,0,0,106,109,5,13,
  	0,0,107,109,5,14,0,0,108,106,1,0,0,0,108,107,1,0,0,0,109,17,1,0,0,0,110,
  	113,5,15,0,0,111,113,5,16,0,0,112,110,1,0,0,0,112,111,1,0,0,0,113,19,
  	1,0,0,0,16,22,24,30,32,39,56,61,65,70,74,79,89,97,99,108,112
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sceneParserStaticData = staticData.release();
}

}

SceneParser::SceneParser(TokenStream *input) : SceneParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SceneParser::SceneParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SceneParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *sceneParserStaticData->atn, sceneParserStaticData->decisionToDFA, sceneParserStaticData->sharedContextCache, options);
}

SceneParser::~SceneParser() {
  delete _interpreter;
}

const atn::ATN& SceneParser::getATN() const {
  return *sceneParserStaticData->atn;
}

std::string SceneParser::getGrammarFileName() const {
  return "Scene.g4";
}

const std::vector<std::string>& SceneParser::getRuleNames() const {
  return sceneParserStaticData->ruleNames;
}

const dfa::Vocabulary& SceneParser::getVocabulary() const {
  return sceneParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SceneParser::getSerializedATN() const {
  return sceneParserStaticData->serializedATN;
}


//----------------- FileContext ------------------------------------------------------------------

SceneParser::FileContext::FileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SceneParser::MainContext* SceneParser::FileContext::main() {
  return getRuleContext<SceneParser::MainContext>(0);
}

std::vector<SceneParser::PathdefContext *> SceneParser::FileContext::pathdef() {
  return getRuleContexts<SceneParser::PathdefContext>();
}

SceneParser::PathdefContext* SceneParser::FileContext::pathdef(size_t i) {
  return getRuleContext<SceneParser::PathdefContext>(i);
}

std::vector<SceneParser::CalcdefContext *> SceneParser::FileContext::calcdef() {
  return getRuleContexts<SceneParser::CalcdefContext>();
}

SceneParser::CalcdefContext* SceneParser::FileContext::calcdef(size_t i) {
  return getRuleContext<SceneParser::CalcdefContext>(i);
}


size_t SceneParser::FileContext::getRuleIndex() const {
  return SceneParser::RuleFile;
}


std::any SceneParser::FileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitFile(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::FileContext* SceneParser::file() {
  FileContext *_localctx = _tracker.createInstance<FileContext>(_ctx, getState());
  enterRule(_localctx, 0, SceneParser::RuleFile);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(24);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SceneParser::T__2) {
      setState(22);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(20);
        pathdef();
        break;
      }

      case 2: {
        setState(21);
        calcdef();
        break;
      }

      default:
        break;
      }
      setState(26);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(27);
    main();
    setState(32);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SceneParser::T__2) {
      setState(30);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
      case 1: {
        setState(28);
        pathdef();
        break;
      }

      case 2: {
        setState(29);
        calcdef();
        break;
      }

      default:
        break;
      }
      setState(34);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MainContext ------------------------------------------------------------------

SceneParser::MainContext::MainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<SceneParser::StatContext *> SceneParser::MainContext::stat() {
  return getRuleContexts<SceneParser::StatContext>();
}

SceneParser::StatContext* SceneParser::MainContext::stat(size_t i) {
  return getRuleContext<SceneParser::StatContext>(i);
}


size_t SceneParser::MainContext::getRuleIndex() const {
  return SceneParser::RuleMain;
}


std::any SceneParser::MainContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitMain(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::MainContext* SceneParser::main() {
  MainContext *_localctx = _tracker.createInstance<MainContext>(_ctx, getState());
  enterRule(_localctx, 2, SceneParser::RuleMain);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(35);
    match(SceneParser::T__0);
    setState(39);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SceneParser::Walk) {
      setState(36);
      stat();
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(42);
    match(SceneParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PathdefContext ------------------------------------------------------------------

SceneParser::PathdefContext::PathdefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SceneParser::PathdefContext::getRuleIndex() const {
  return SceneParser::RulePathdef;
}


std::any SceneParser::PathdefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitPathdef(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::PathdefContext* SceneParser::pathdef() {
  PathdefContext *_localctx = _tracker.createInstance<PathdefContext>(_ctx, getState());
  enterRule(_localctx, 4, SceneParser::RulePathdef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(44);
    match(SceneParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CalcdefContext ------------------------------------------------------------------

SceneParser::CalcdefContext::CalcdefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SceneParser::CalcdefContext::getRuleIndex() const {
  return SceneParser::RuleCalcdef;
}


std::any SceneParser::CalcdefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitCalcdef(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::CalcdefContext* SceneParser::calcdef() {
  CalcdefContext *_localctx = _tracker.createInstance<CalcdefContext>(_ctx, getState());
  enterRule(_localctx, 6, SceneParser::RuleCalcdef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(46);
    match(SceneParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatContext ------------------------------------------------------------------

SceneParser::StatContext::StatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SceneParser::WalkContext* SceneParser::StatContext::walk() {
  return getRuleContext<SceneParser::WalkContext>(0);
}


size_t SceneParser::StatContext::getRuleIndex() const {
  return SceneParser::RuleStat;
}


std::any SceneParser::StatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitStat(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::StatContext* SceneParser::stat() {
  StatContext *_localctx = _tracker.createInstance<StatContext>(_ctx, getState());
  enterRule(_localctx, 8, SceneParser::RuleStat);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(48);
    walk();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WalkContext ------------------------------------------------------------------

SceneParser::WalkContext::WalkContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SceneParser::WalkContext::Walk() {
  return getToken(SceneParser::Walk, 0);
}

SceneParser::ExprContext* SceneParser::WalkContext::expr() {
  return getRuleContext<SceneParser::ExprContext>(0);
}


size_t SceneParser::WalkContext::getRuleIndex() const {
  return SceneParser::RuleWalk;
}


std::any SceneParser::WalkContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitWalk(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::WalkContext* SceneParser::walk() {
  WalkContext *_localctx = _tracker.createInstance<WalkContext>(_ctx, getState());
  enterRule(_localctx, 10, SceneParser::RuleWalk);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    match(SceneParser::Walk);
    setState(51);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

SceneParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SceneParser::ExprContext::getRuleIndex() const {
  return SceneParser::RuleExpr;
}

void SceneParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AddContext ------------------------------------------------------------------

std::vector<SceneParser::ExprContext *> SceneParser::AddContext::expr() {
  return getRuleContexts<SceneParser::ExprContext>();
}

SceneParser::ExprContext* SceneParser::AddContext::expr(size_t i) {
  return getRuleContext<SceneParser::ExprContext>(i);
}

SceneParser::AddContext::AddContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::AddContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitAdd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ABSContext ------------------------------------------------------------------

SceneParser::ExprContext* SceneParser::ABSContext::expr() {
  return getRuleContext<SceneParser::ExprContext>(0);
}

SceneParser::ABSContext::ABSContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::ABSContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitABS(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultContext ------------------------------------------------------------------

std::vector<SceneParser::KlamKonContext *> SceneParser::MultContext::klamKon() {
  return getRuleContexts<SceneParser::KlamKonContext>();
}

SceneParser::KlamKonContext* SceneParser::MultContext::klamKon(size_t i) {
  return getRuleContext<SceneParser::KlamKonContext>(i);
}

std::vector<SceneParser::NumberContext *> SceneParser::MultContext::number() {
  return getRuleContexts<SceneParser::NumberContext>();
}

SceneParser::NumberContext* SceneParser::MultContext::number(size_t i) {
  return getRuleContext<SceneParser::NumberContext>(i);
}

SceneParser::MultContext::MultContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::MultContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitMult(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DifeContext ------------------------------------------------------------------

std::vector<SceneParser::KlamKonContext *> SceneParser::DifeContext::klamKon() {
  return getRuleContexts<SceneParser::KlamKonContext>();
}

SceneParser::KlamKonContext* SceneParser::DifeContext::klamKon(size_t i) {
  return getRuleContext<SceneParser::KlamKonContext>(i);
}

std::vector<SceneParser::NumberContext *> SceneParser::DifeContext::number() {
  return getRuleContexts<SceneParser::NumberContext>();
}

SceneParser::NumberContext* SceneParser::DifeContext::number(size_t i) {
  return getRuleContext<SceneParser::NumberContext>(i);
}

SceneParser::DifeContext::DifeContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::DifeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitDife(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NumExprContext ------------------------------------------------------------------

SceneParser::NumberContext* SceneParser::NumExprContext::number() {
  return getRuleContext<SceneParser::NumberContext>(0);
}

SceneParser::NumExprContext::NumExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::NumExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitNumExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VarExprContext ------------------------------------------------------------------

SceneParser::VarContext* SceneParser::VarExprContext::var() {
  return getRuleContext<SceneParser::VarContext>(0);
}

SceneParser::VarExprContext::VarExprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::VarExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitVarExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DimContext ------------------------------------------------------------------

std::vector<SceneParser::ExprContext *> SceneParser::DimContext::expr() {
  return getRuleContexts<SceneParser::ExprContext>();
}

SceneParser::ExprContext* SceneParser::DimContext::expr(size_t i) {
  return getRuleContext<SceneParser::ExprContext>(i);
}

SceneParser::DimContext::DimContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::DimContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitDim(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NegateContext ------------------------------------------------------------------

SceneParser::NumberContext* SceneParser::NegateContext::number() {
  return getRuleContext<SceneParser::NumberContext>(0);
}

SceneParser::NegateContext::NegateContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::NegateContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitNegate(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpContext ------------------------------------------------------------------

std::vector<SceneParser::KlamKonContext *> SceneParser::ExpContext::klamKon() {
  return getRuleContexts<SceneParser::KlamKonContext>();
}

SceneParser::KlamKonContext* SceneParser::ExpContext::klamKon(size_t i) {
  return getRuleContext<SceneParser::KlamKonContext>(i);
}

std::vector<SceneParser::NumberContext *> SceneParser::ExpContext::number() {
  return getRuleContexts<SceneParser::NumberContext>();
}

SceneParser::NumberContext* SceneParser::ExpContext::number(size_t i) {
  return getRuleContext<SceneParser::NumberContext>(i);
}

SceneParser::ExpContext::ExpContext(ExprContext *ctx) { copyFrom(ctx); }


std::any SceneParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitExp(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::ExprContext* SceneParser::expr() {
   return expr(0);
}

SceneParser::ExprContext* SceneParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  SceneParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  SceneParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 12;
  enterRecursionRule(_localctx, 12, SceneParser::RuleExpr, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(89);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<ExpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(56);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(54);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(55);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(58);
      match(SceneParser::T__3);
      setState(61);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(59);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(60);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MultContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(65);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(63);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(64);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(67);
      match(SceneParser::T__4);
      setState(70);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(68);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(69);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<DifeContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(74);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(72);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(73);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(76);
      match(SceneParser::T__5);
      setState(79);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case SceneParser::T__9: {
          setState(77);
          klamKon();
          break;
        }

        case SceneParser::Num:
        case SceneParser::Float: {
          setState(78);
          number();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<ABSContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(81);
      match(SceneParser::T__8);
      setState(82);
      expr(0);
      setState(83);
      match(SceneParser::T__8);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<NegateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(85);
      match(SceneParser::T__7);
      setState(86);
      number();
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<NumExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(87);
      number();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<VarExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(88);
      var();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(99);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(97);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<AddContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(91);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(92);
          match(SceneParser::T__6);
          setState(93);
          expr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<DimContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(94);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(95);
          match(SceneParser::T__7);
          setState(96);
          expr(6);
          break;
        }

        default:
          break;
        } 
      }
      setState(101);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- KlamKonContext ------------------------------------------------------------------

SceneParser::KlamKonContext::KlamKonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SceneParser::ExprContext* SceneParser::KlamKonContext::expr() {
  return getRuleContext<SceneParser::ExprContext>(0);
}


size_t SceneParser::KlamKonContext::getRuleIndex() const {
  return SceneParser::RuleKlamKon;
}


std::any SceneParser::KlamKonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitKlamKon(this);
  else
    return visitor->visitChildren(this);
}

SceneParser::KlamKonContext* SceneParser::klamKon() {
  KlamKonContext *_localctx = _tracker.createInstance<KlamKonContext>(_ctx, getState());
  enterRule(_localctx, 14, SceneParser::RuleKlamKon);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(102);
    match(SceneParser::T__9);
    setState(103);
    expr(0);
    setState(104);
    match(SceneParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

SceneParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SceneParser::NumberContext::getRuleIndex() const {
  return SceneParser::RuleNumber;
}

void SceneParser::NumberContext::copyFrom(NumberContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- FloatContext ------------------------------------------------------------------

tree::TerminalNode* SceneParser::FloatContext::Float() {
  return getToken(SceneParser::Float, 0);
}

SceneParser::FloatContext::FloatContext(NumberContext *ctx) { copyFrom(ctx); }


std::any SceneParser::FloatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitFloat(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntContext ------------------------------------------------------------------

tree::TerminalNode* SceneParser::IntContext::Num() {
  return getToken(SceneParser::Num, 0);
}

SceneParser::IntContext::IntContext(NumberContext *ctx) { copyFrom(ctx); }


std::any SceneParser::IntContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitInt(this);
  else
    return visitor->visitChildren(this);
}
SceneParser::NumberContext* SceneParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 16, SceneParser::RuleNumber);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(108);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SceneParser::Num: {
        _localctx = _tracker.createInstance<SceneParser::IntContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(106);
        match(SceneParser::Num);
        break;
      }

      case SceneParser::Float: {
        _localctx = _tracker.createInstance<SceneParser::FloatContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(107);
        match(SceneParser::Float);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarContext ------------------------------------------------------------------

SceneParser::VarContext::VarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SceneParser::VarContext::getRuleIndex() const {
  return SceneParser::RuleVar;
}

void SceneParser::VarContext::copyFrom(VarContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- GlobalVariableContext ------------------------------------------------------------------

tree::TerminalNode* SceneParser::GlobalVariableContext::IncID() {
  return getToken(SceneParser::IncID, 0);
}

SceneParser::GlobalVariableContext::GlobalVariableContext(VarContext *ctx) { copyFrom(ctx); }


std::any SceneParser::GlobalVariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitGlobalVariable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VariableContext ------------------------------------------------------------------

tree::TerminalNode* SceneParser::VariableContext::ID() {
  return getToken(SceneParser::ID, 0);
}

SceneParser::VariableContext::VariableContext(VarContext *ctx) { copyFrom(ctx); }


std::any SceneParser::VariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SceneVisitor*>(visitor))
    return parserVisitor->visitVariable(this);
  else
    return visitor->visitChildren(this);
}
SceneParser::VarContext* SceneParser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 18, SceneParser::RuleVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(112);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case SceneParser::ID: {
        _localctx = _tracker.createInstance<SceneParser::VariableContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(110);
        match(SceneParser::ID);
        break;
      }

      case SceneParser::IncID: {
        _localctx = _tracker.createInstance<SceneParser::GlobalVariableContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(111);
        match(SceneParser::IncID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool SceneParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 6: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool SceneParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 6);
    case 1: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

void SceneParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sceneParserInitialize();
#else
  ::antlr4::internal::call_once(sceneParserOnceFlag, sceneParserInitialize);
#endif
}


// Generated from Scene.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  SceneParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, Walk = 12, Num = 13, Float = 14, 
    ID = 15, IncID = 16, WS = 17
  };

  enum {
    RuleFile = 0, RuleMain = 1, RulePathdef = 2, RuleCalcdef = 3, RuleStat = 4, 
    RuleWalk = 5, RuleExpr = 6, RuleKlamKon = 7, RuleNumber = 8, RuleVar = 9
  };

  explicit SceneParser(antlr4::TokenStream *input);

  SceneParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~SceneParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class FileContext;
  class MainContext;
  class PathdefContext;
  class CalcdefContext;
  class StatContext;
  class WalkContext;
  class ExprContext;
  class KlamKonContext;
  class NumberContext;
  class VarContext; 

  class  FileContext : public antlr4::ParserRuleContext {
  public:
    FileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MainContext *main();
    std::vector<PathdefContext *> pathdef();
    PathdefContext* pathdef(size_t i);
    std::vector<CalcdefContext *> calcdef();
    CalcdefContext* calcdef(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FileContext* file();

  class  MainContext : public antlr4::ParserRuleContext {
  public:
    MainContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatContext *> stat();
    StatContext* stat(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MainContext* main();

  class  PathdefContext : public antlr4::ParserRuleContext {
  public:
    PathdefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PathdefContext* pathdef();

  class  CalcdefContext : public antlr4::ParserRuleContext {
  public:
    CalcdefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CalcdefContext* calcdef();

  class  StatContext : public antlr4::ParserRuleContext {
  public:
    StatContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    WalkContext *walk();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatContext* stat();

  class  WalkContext : public antlr4::ParserRuleContext {
  public:
    WalkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Walk();
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WalkContext* walk();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() = default;
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AddContext : public ExprContext {
  public:
    AddContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ABSContext : public ExprContext {
  public:
    ABSContext(ExprContext *ctx);

    ExprContext *expr();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MultContext : public ExprContext {
  public:
    MultContext(ExprContext *ctx);

    std::vector<KlamKonContext *> klamKon();
    KlamKonContext* klamKon(size_t i);
    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DifeContext : public ExprContext {
  public:
    DifeContext(ExprContext *ctx);

    std::vector<KlamKonContext *> klamKon();
    KlamKonContext* klamKon(size_t i);
    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NumExprContext : public ExprContext {
  public:
    NumExprContext(ExprContext *ctx);

    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarExprContext : public ExprContext {
  public:
    VarExprContext(ExprContext *ctx);

    VarContext *var();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DimContext : public ExprContext {
  public:
    DimContext(ExprContext *ctx);

    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NegateContext : public ExprContext {
  public:
    NegateContext(ExprContext *ctx);

    NumberContext *number();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExpContext : public ExprContext {
  public:
    ExpContext(ExprContext *ctx);

    std::vector<KlamKonContext *> klamKon();
    KlamKonContext* klamKon(size_t i);
    std::vector<NumberContext *> number();
    NumberContext* number(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  KlamKonContext : public antlr4::ParserRuleContext {
  public:
    KlamKonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExprContext *expr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KlamKonContext* klamKon();

  class  NumberContext : public antlr4::ParserRuleContext {
  public:
    NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    NumberContext() = default;
    void copyFrom(NumberContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  FloatContext : public NumberContext {
  public:
    FloatContext(NumberContext *ctx);

    antlr4::tree::TerminalNode *Float();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntContext : public NumberContext {
  public:
    IntContext(NumberContext *ctx);

    antlr4::tree::TerminalNode *Num();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  NumberContext* number();

  class  VarContext : public antlr4::ParserRuleContext {
  public:
    VarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    VarContext() = default;
    void copyFrom(VarContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GlobalVariableContext : public VarContext {
  public:
    GlobalVariableContext(VarContext *ctx);

    antlr4::tree::TerminalNode *IncID();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VariableContext : public VarContext {
  public:
    VariableContext(VarContext *ctx);

    antlr4::tree::TerminalNode *ID();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  VarContext* var();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};


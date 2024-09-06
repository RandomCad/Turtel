
// Generated from Scene.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SceneVisitor.h"


/**
 * This class provides an empty implementation of SceneVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SceneBaseVisitor : public SceneVisitor {
public:

  virtual std::any visitFile(SceneParser::FileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMain(SceneParser::MainContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPathdef(SceneParser::PathdefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCalcdef(SceneParser::CalcdefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStat(SceneParser::StatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWalk(SceneParser::WalkContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdd(SceneParser::AddContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitABS(SceneParser::ABSContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMult(SceneParser::MultContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDife(SceneParser::DifeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNumExpr(SceneParser::NumExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarExpr(SceneParser::VarExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDim(SceneParser::DimContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNegate(SceneParser::NegateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExp(SceneParser::ExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitKlamKon(SceneParser::KlamKonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInt(SceneParser::IntContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloat(SceneParser::FloatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVariable(SceneParser::VariableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGlobalVariable(SceneParser::GlobalVariableContext *ctx) override {
    return visitChildren(ctx);
  }


};


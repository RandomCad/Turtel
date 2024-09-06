
// Generated from Scene.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"
#include "SceneParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SceneParser.
 */
class  SceneVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SceneParser.
   */
    virtual std::any visitFile(SceneParser::FileContext *context) = 0;

    virtual std::any visitMain(SceneParser::MainContext *context) = 0;

    virtual std::any visitPathdef(SceneParser::PathdefContext *context) = 0;

    virtual std::any visitCalcdef(SceneParser::CalcdefContext *context) = 0;

    virtual std::any visitStat(SceneParser::StatContext *context) = 0;

    virtual std::any visitWalk(SceneParser::WalkContext *context) = 0;

    virtual std::any visitAdd(SceneParser::AddContext *context) = 0;

    virtual std::any visitABS(SceneParser::ABSContext *context) = 0;

    virtual std::any visitMult(SceneParser::MultContext *context) = 0;

    virtual std::any visitDife(SceneParser::DifeContext *context) = 0;

    virtual std::any visitNumExpr(SceneParser::NumExprContext *context) = 0;

    virtual std::any visitVarExpr(SceneParser::VarExprContext *context) = 0;

    virtual std::any visitDim(SceneParser::DimContext *context) = 0;

    virtual std::any visitNegate(SceneParser::NegateContext *context) = 0;

    virtual std::any visitExp(SceneParser::ExpContext *context) = 0;

    virtual std::any visitKlamKon(SceneParser::KlamKonContext *context) = 0;

    virtual std::any visitInt(SceneParser::IntContext *context) = 0;

    virtual std::any visitFloat(SceneParser::FloatContext *context) = 0;

    virtual std::any visitVariable(SceneParser::VariableContext *context) = 0;

    virtual std::any visitGlobalVariable(SceneParser::GlobalVariableContext *context) = 0;


};


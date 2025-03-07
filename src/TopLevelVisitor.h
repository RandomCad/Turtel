#pragma once

#include "SceneBaseVisitor.h"
#include "SceneParser.h"
#include "src/VariableHeandler.h"
#include <ostream>
/**
 *This class is the Top Level visitor.
 *It should only be used to visit the stat and direckt childs.
 *It outputs direcktly to an ostream.
 */
class TopLevelVisitor : public SceneBaseVisitor{
    std::ostream &output; ///<The output of this class. Everything is writen to this.
    VariableHeandler &vars; ///<The variable conetext my be changed.

  public:
    TopLevelVisitor(std::ostream &a, VariableHeandler &b) : 
      output(a), 
      vars(b) 
    {};

    
  private:
    ///The visitor for a walk statment.
    std::any visitJumpFront(SceneParser::JumpFrontContext *ctx) override;
    std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
    std::any visitWalkFront(SceneParser::WalkFrontContext *ctx) override;
    std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    std::any visitSave(SceneParser::SaveContext *ctx) override;
    std::any visitWaklHome(SceneParser::WaklHomeContext *ctx) override;
    std::any visitJumpHome(SceneParser::JumpHomeContext *ctx) override;
    std::any visitTurnRight(SceneParser::TurnRightContext *ctx) override;
    std::any visitTurnLeft(SceneParser::TurnLeftContext *ctx) override;
    std::any visitDirection(SceneParser::DirectionContext *ctx) override;
    std::any visitClear(SceneParser::ClearContext *ctx) override;
    std::any visitStopError(SceneParser::StopErrorContext *ctx) override;
    std::any visitStopOK(SceneParser::StopOKContext *ctx) override;
    std::any visitFinError(SceneParser::FinErrorContext *ctx) override;
    std::any visitFinOK(SceneParser::FinOKContext *ctx) override;
    std::any visitMark(SceneParser::MarkContext *ctx) override;
    std::any visitWalkMark(SceneParser::WalkMarkContext *ctx) override;
    std::any visitJumpMark(SceneParser::JumpMarkContext *ctx) override;
    std::any visitColorCmd(SceneParser::ColorCmdContext *ctx) override;
    ///The visitor for the var commands
    std::any visitStoreVar(SceneParser::StoreVarContext *ctx) override;
    std::any visitSubVar(SceneParser::SubVarContext *ctx) override;
    std::any visitDivVar(SceneParser::DivVarContext *ctx) override;
    std::any visitAddVar(SceneParser::AddVarContext *ctx) override;
    std::any visitMultVar(SceneParser::MultVarContext *ctx) override;

    //The visitor for the calcExpre
    std::any visitNumExpr(SceneParser::NumExprContext *ctx) override;
    std::any visitExp(SceneParser::ExpContext *ctx) override;
    std::any visitAdd(SceneParser::AddContext *ctx) override;
    std::any visitDim(SceneParser::DimContext *ctx) override;
    std::any visitDife(SceneParser::DifeContext *ctx) override;
    std::any visitMult(SceneParser::MultContext *ctx) override;
    std::any visitABS(SceneParser::ABSContext *ctx) override;
    std::any visitNegate(SceneParser::NegateContext *ctx) override;
    std::any visitVarExpr(SceneParser::VarExprContext *ctx) override;

    ///conditions (bool)
    std::any visitLesEqThan(SceneParser::LesEqThanContext *ctx) override;
    std::any visitGreaterThan(SceneParser::GreaterThanContext *ctx) override;
    std::any visitLesThan(SceneParser::LesThanContext *ctx) override;
    std::any visitGreaterEqThan(SceneParser::GreaterEqThanContext *ctx) override;
    std::any visitClamCond(SceneParser::ClamCondContext *ctx) override;
    std::any visitNotCond(SceneParser::NotCondContext *ctx) override;
    std::any visitAndCond(SceneParser::AndCondContext *ctx) override;
    std::any visitOrCond(SceneParser::OrCondContext *ctx) override;
    //Number
    std::any visitInt(SceneParser::IntContext *ctx) override;
    std::any visitFloat(SceneParser::FloatContext *ctx) override;
    ///The visitor for variables
    std::any visitVariable(SceneParser::VariableContext *ctx) override;
    std::any visitGlobalVariable(SceneParser::GlobalVariableContext *ctx) override;
    /// Helper funk to get the result of a calc expr
    std::string UnwrapExpre(SceneParser::ExprContext *ctx);

    ///The folowing commented out functions must be implimented by a preprocessing step by rewriteing the AST
    //std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    //std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
};

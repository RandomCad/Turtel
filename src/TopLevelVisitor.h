#pragma once


#include "SceneBaseVisitor.h"
#include "SceneParser.h"
#include "FunctionHandler.h"
#include "build/_deps/googletest-src/googletest/include/gtest/gtest_prod.h"
#include "src/Function.h"
#include "src/Variable.h"
#include "src/VariableHeandler.h"
#include <ostream>
#include <unordered_map>
/**
 *This class is the Top Level visitor.
 *It should only be used to visit the stat and direckt childs.
 *It outputs direcktly to an ostream.
 */
class TopLevelVisitor : public SceneBaseVisitor{
    std::ostream &output; ///<The output of this class. Everything is writen to this.
    std::unordered_map<std::string, Variable> envVar;
    std::unordered_map<std::string, Variable> ctxVar;
    std::unordered_map<std::string, Function> funcs;
    static int infinitLoopFlag; ///<The flag that says if infinit loops are allowed
                                ///<0 is unknowen -> create error report
                                ///<1 infinit loops are allowed
                                ///<-1 infinit loops aren't allowed
  public:
    TopLevelVisitor(std::ostream &a);
    ///entry point for code generation
    std::any visitFile(SceneParser::FileContext *ctx) override;
    
  private:
    //The visotr for the function implementation
    std::any visitCalcdef(SceneParser::CalcdefContext *ctx) override;
    std::any visitPathdef(SceneParser::PathdefContext *ctx) override;
    std::any visitMain(SceneParser::MainContext *ctx) override;
    ///The visitor for a walk statment.
    std::any visitJumpFront(SceneParser::JumpFrontContext *ctx) override;
    std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
    std::any visitWalkFront(SceneParser::WalkFrontContext *ctx) override;
    std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    std::any visitSave(SceneParser::SaveContext *ctx) override;
    std::any visitWalkHome(SceneParser::WalkHomeContext *ctx) override;
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
    ///The visitor for conditional operations
    std::any visitIf(SceneParser::IfContext *ctx) override;
    ///std::any visitElse(SceneParser::ElseContext *ctx) override; is inlined in the if!
    std::any visitWhile(SceneParser::WhileContext *ctx) override;
    std::any visitDoUntil(SceneParser::DoUntilContext *ctx) override;
    std::any visitToFor(SceneParser::ToForContext *ctx) override;
    std::any visitSimpUpFor(SceneParser::SimpUpForContext *ctx) override;
    std::any visitSimpDownFor(SceneParser::SimpDownForContext *ctx) override;
    std::any visitStepUpFor(SceneParser::StepUpForContext *ctx) override;
    std::any visitStepDownFor(SceneParser::StepDownForContext *ctx) override;

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
    std::any visitFuncCall(SceneParser::FuncCallContext *ctx) override;
    ///\return a vector with all the variables
    std::any visitParamlist(SceneParser::ParamlistContext *ctx) override;
    ///conditions (bool)
    std::any visitLesEqThan(SceneParser::LesEqThanContext *ctx) override;
    std::any visitGreaterThan(SceneParser::GreaterThanContext *ctx) override;
    std::any visitLesThan(SceneParser::LesThanContext *ctx) override;
    std::any visitGreaterEqThan(SceneParser::GreaterEqThanContext *ctx) override;
    std::any visitEqual(SceneParser::EqualContext *ctx) override;
    std::any visitUnequal(SceneParser::UnequalContext *ctx) override;
    std::any visitClamCond(SceneParser::ClamCondContext *ctx) override;
    std::any visitNotCond(SceneParser::NotCondContext *ctx) override;
    std::any visitAndCond(SceneParser::AndCondContext *ctx) override;
    std::any visitOrCond(SceneParser::OrCondContext *ctx) override;
    //Number
    std::any visitInt(SceneParser::IntContext *ctx) override;
    std::any visitFloat(SceneParser::FloatContext *ctx) override;
    //The visitor for variables
    ///\return The string of the variable
    std::any visitVariable(SceneParser::VariableContext *ctx) override;
    ///\return The string of the variable
    std::any visitGlobalVariable(SceneParser::GlobalVariableContext *ctx) override;
    /// Helper funk to get the result of a calc expr
    std::string UnwrapExpre(SceneParser::ExprContext *ctx);

  private:
  
    FRIEND_TEST(TOP_LEVEL_VISITOR_TEST_SUITE, While);
    FRIEND_TEST(TOP_LEVEL_VISITOR_TEST_SUITE, DoUntil);

    ///The folowing commented out functions must be implimented by a preprocessing step by rewriteing the AST
    //std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    //std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
};

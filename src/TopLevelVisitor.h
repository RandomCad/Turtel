#pragma once

#include "SceneBaseVisitor.h"
#include "SceneParser.h"
#include "src/MathVisitor.h"
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

    ///other Visitors:
    MathVisitor mathVis;
  public:
    TopLevelVisitor(std::ostream &a, VariableHeandler &b) : 
      output(a), 
      vars(b), 
      mathVis(vars) 
    {};

    std::any visitVariable(SceneParser::VariableContext *ctx) override;
    std::any visitGlobalVariable(SceneParser::GlobalVariableContext *ctx) override;
    
  private:
    ///The visitor for a walk statment.
    std::any visitJumpFront(SceneParser::JumpFrontContext *ctx) override;
    std::any visitWalkFront(SceneParser::WalkFrontContext *ctx) override;
    std::any visitSave(SceneParser::SaveContext *ctx) override;
    std::any visitWalkHome(SceneParser::WalkHomeContext *ctx) override;
    std::any visitJumpHome(SceneParser::JumpHomeContext *ctx) override;
    std::any visitTurnRight(SceneParser::TurnRightContext *ctx) override;
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

    std::string UnwrapExpre(SceneParser::ExprContext *ctx);

  private:
  
    ///The folowing commented out functions must be implimented by a preprocessing step by rewriteing the AST
    //std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    //std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
};

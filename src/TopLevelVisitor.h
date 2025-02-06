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
    
    ///The visitor for a walk statment.
    std::any visitJumpFront(SceneParser::JumpFrontContext *ctx) override;
    std::any visitWalkFront(SceneParser::WalkFrontContext *ctx) override;
    std::any visitSave(SceneParser::SaveContext *ctx) override;
    std::any visitWaklHome(SceneParser::WaklHomeContext *ctx) override;
    std::any visitJumpHome(SceneParser::JumpHomeContext *ctx) override;
    std::any visitTurnRight(SceneParser::TurnRightContext *ctx) override;
    ///The folowing commented out functions must be implimented by a preprocessing step by rewriteing the AST
    //std::any visitWalkBack(SceneParser::WalkBackContext *ctx) override;
    //std::any visitJumpBack(SceneParser::JumpBackContext *ctx) override;
};

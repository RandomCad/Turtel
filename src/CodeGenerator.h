#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <vector>

#include "../libs/SceneParser.h"
#include "../libs/SceneBaseVisitor.h"
#include "../UnitTest/UnitTest.h"
#include "Variable.h"
#include "src/VariableHeandler.h"

#define POSITION_X_NAME POS_X
#define POSITION_Y_NAME POS_Y
#define RENDERER_NAME "__rnd_rnd"
#define ROTATION_NAME Rotation
#define CALC_POS_X(len) POSITION_X_NAME + len * cos(ROTATION_NAME)
#define CALC_POS_Y(len) POSITION_Y_NAME + len * sin(ROTATION_NAME)


size_t GetUniquNumber();

class CodeGenerator : public SceneBaseVisitor{
  private:
    std::ostream &output;
    SceneParser::FileContext *astBase;
    SceneParser::MainContext *astMain;
    std::vector<SceneParser::CalcdefContext *> astCalcdef;
    std::vector<SceneParser::PathdefContext *> astPathdef;
    VariableHeandler _variables;
    //Only used for Unittesting
    CodeGenerator(): output(std::cout) {}
    CodeGenerator(std::ostream &outStream);
  public:
    CodeGenerator(std::ostream &outStream, SceneParser::FileContext *AstBase);

    ///Main function of the classe
    void GenerateCode();
    
    //Number
    std::any visitInt(SceneParser::IntContext *ctx) override;
    std::any visitFloat(SceneParser::FloatContext *ctx) override;
    //Expr
    std::any visitNumExpr(SceneParser::NumExprContext *ctx) override;
    std::any visitExp(SceneParser::ExpContext *ctx) override;
    std::any visitAdd(SceneParser::AddContext *ctx) override;
    std::any visitDim(SceneParser::DimContext *ctx) override;
    std::any visitDife(SceneParser::DifeContext *ctx) override;
    std::any visitMult(SceneParser::MultContext *ctx) override;
    std::any visitABS(SceneParser::ABSContext *ctx) override;
    std::any visitNegate(SceneParser::NegateContext *ctx) override;
    std::any visitWalk(SceneParser::WalkContext *ctx) override;
  
  private:
    std::any visitMain(SceneParser::MainContext *ctx) override;

    template<typename T>
    void inline CalcPosX(const T len){
      output << "POSITION_X_NAME + " << len <<" * cos(ROTATION_NAME)";
    }
    template<typename T>
    void inline CalcPosY(const T len){
      output << "POSITION_Y_NAME + " << len <<" * sin(ROTATION_NAME)";
    }
    
    void AddIncludes();
    void AddFunctionDeclaration();
    void AddMain();
    void AddTurtelMain();
    void AddTurtelFunctions();

    void ProgrammBase();
    void EndeMain();

#ifdef UNIT_TEST
    friend bool TestNumberExec(TestError*&ret);
    friend bool TestExprExec(TestError *&ret);
    friend bool TestCodeGeneratorProgramBase(TestError *&);
    friend bool TestCodeGeneratorEndMain(TestError *&);
    friend bool TestCodeGeneratorDTOr(TestError *&);
    friend bool TestCodeGeneratorEmpty(TestError *&);
    friend bool TestCodeGeneratorCTor(TestError *&);
#endif
};

#endif

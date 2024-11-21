#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <vector>

#include "../libs/SceneParser.h"
#include "../libs/SceneBaseVisitor.h"
#include "src/MathVisitor.h"
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
    MathVisitor _mathVis;
    //Only used for Unittesting
    CodeGenerator(): output(std::cout), _mathVis(_variables) {}
    CodeGenerator(std::ostream &outStream);
  public:
    CodeGenerator(std::ostream &outStream, SceneParser::FileContext *AstBase);

    ///Main function of the classe
    void GenerateCode();
    
  private:
    std::any visitMain(SceneParser::MainContext *ctx) override;
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

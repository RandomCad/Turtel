
#include "SceneParser.h"
#include "src/CodeGenerator.h"
#include "src/InternalVarNames.h"
#include "src/Variable.h"
#include "src/FunctionHandler.h"
#include "src/Function.h"
#include "src/VariableVisitor.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief Retrieves variables from the parameter list context.
 * 
 * This function extracts variables from the given parameter list context and returns them as a vector of Variable objects.
 * 
 * @param ctx The parameter list context to extract variables from.
 * @return A vector of Variable objects.
 */
std::vector<Variable> getVars(SceneParser::ParamlistContext *ctx){
  if (!ctx){
    return {};
  }
  std::vector<Variable> ret;
  ret.reserve(ctx->var().size());

  std::transform(
    ctx->var().begin(), 
    ctx->var().end(), 
    std::back_inserter(ret),
    [](const SceneParser::VarContext *s) -> Variable { 
      return Variable(VarType::DOUBLE, "__usr_func_" + s->children[0]->getText());
    }
  );
  return ret;
}

/**
 * @brief Constructs a FunctionHandler object.
 * 
 * This constructor initializes the FunctionHandler with the main context, path definitions, and calculation definitions.
 * 
 * @param mainCtx The main context.
 * @param paths A vector of path definition contexts.
 * @param calcs A vector of calculation definition contexts.
 */
FunctionHandler::FunctionHandler(SceneParser::MainContext* mainCtx, 
                    std::vector<SceneParser::PathdefContext*> &paths,
                    std::vector<SceneParser::CalcdefContext*> &calcs){
  funcs.emplace(
      MAIN_FUNC, 
      Function(TURTEL_MAIN_FUNC_NAME, VarType::VOID, mainCtx)
      );
  for (auto i : paths) {
    if(!funcs .emplace( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::VOID,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
  for (auto i : calcs) {
    if(!funcs .emplace ( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::DOUBLE,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
}

/**
 * @brief Constructs a FunctionHandler object.
 * 
 * This constructor initializes the FunctionHandler with the main context, path definitions, and calculation definitions.
 * 
 * @param mainCtx The main context.
 * @param paths An rvalue reference to a vector of path definition contexts.
 * @param calcs An rvalue reference to a vector of calculation definition contexts.
 */
FunctionHandler::FunctionHandler(SceneParser::MainContext* mainCtx, 
                    std::vector<SceneParser::PathdefContext*> &&paths,
                    std::vector<SceneParser::CalcdefContext*> &&calcs){
  funcs.emplace(
      MAIN_FUNC, 
      Function(TURTEL_MAIN_FUNC_NAME, VarType::VOID, mainCtx)
      );
  for (auto i : paths) {
    if(!funcs .emplace( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::VOID,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
  for (auto i : calcs) {
    if(!funcs .emplace ( i->ID()->getText(), 
                Function( USR_FUNC_PREFIX + i->ID()->getText(),
                VarType::DOUBLE,
                i)
              )
              .second){
      throw "duplicate function names"; //TODO better error
    }
  }
}

/**
 * @brief Retrieves the function declaration for a given function name.
 * 
 * This function returns the function declaration as a string for the specified function name.
 * 
 * @param funcName The name of the function.
 * @return The function declaration as a string.
 */
std::string FunctionHandler::getFunctionDeclaration(std::string funcName){
  return funcs.at(funcName).getFunctionDeclaration();
}

/**
 * @brief Retrieves the function call for a given function name.
 * 
 * This function returns the function call as a string for the specified function name and input variables.
 * 
 * @param funcName The name of the function.
 * @param input A vector of input variables.
 * @return The function call as a string.
 */
std::string FunctionHandler::getFunctionCall(std::string funcName, std::vector<Variable> input){
#ifndef NDEBUG
std::cerr << __func__ << std::endl;
#endif  
  return funcs.at(funcName).getFunctionCall(input);
}

/**
 * @brief Implements the functions.
 * 
 * This function generates the implementation of the functions and writes them to the output stream.
 * 
 * @param out The output stream to write the function implementations to.
 * @param vis The CodeGenerator object to use for generating the code.
 */
void FunctionHandler::ImplementFunctions(std::ostream &out, CodeGenerator *vis){
  for (auto i : funcs) {
    std::cerr << &i << std::endl;
    i.second.Implement(out, vis);
    std::cerr << &i << "ende" << std::endl;
  }
  std::cerr << "exiting" << __func__ << std::endl;
}

/**
 * @brief Retrieves the function declarations.
 * 
 * This function generates the function declarations and writes them to the output stream.
 * 
 * @param out The output stream to write the function declarations to.
 */
void FunctionHandler::getFunctionDeclarations(std::ostream &out){
  for (auto i : funcs) {
    out << i.second.getFunctionDeclaration();
  }
}

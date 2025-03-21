#pragma once

#include "src/InternalVarNames.h"
#include "src/VariableHeandler.h"
#include <ostream>

/**
 * @brief Generates the code for presenting the rendered content.
 * 
 * This function sets the render target, clears the renderer, copies the texture to the renderer,
 * presents the renderer, and then sets the render target back to the texture.
 * 
 * @param var The VariableHeandler object containing variable names.
 * @param output The output stream to write the generated code to.
 */
void GenPresent(VariableHeandler &var, std::ostream &output);

/**
 * @brief Calculates the new X position based on length and rotation.
 * 
 * This function calculates the new X position using the given length and rotation variables.
 * 
 * @tparam T The type of the length parameter.
 * @param len The length to calculate the new position.
 * @param output The output stream to write the generated code to.
 * @param xName The name of the X position variable.
 * @param rotName The name of the rotation variable.
 */
template<typename T>
void CalcPosX(
    const T len,
    std::ostream &output,
    const char *xName, 
    const char *rotName){
  output << xName << " + " << len << " * cos(" << rotName << ')';
}

/**
 * @brief Calculates the new X position based on length and rotation.
 * 
 * This function calculates the new X position using the given length and rotation variables.
 * 
 * @tparam T The type of the length parameter.
 * @param len The length to calculate the new position.
 * @param output The output stream to write the generated code to.
 * @param vars The VariableHeandler object containing variable names.
 */
template<typename T>
void CalcPosX(
    const T len,
    std::ostream &output,
    VariableHeandler &vars){
  output  << vars.getVariableNameString(POS_X) 
          << " + " 
          << len 
          << " * "
          << "cos(" << vars.getVariableNameString(ROTATION) << ')';
}

/**
 * @brief Calculates the new Y position based on length and rotation.
 * 
 * This function calculates the new Y position using the given length and rotation variables.
 * 
 * @tparam T The type of the length parameter.
 * @param len The length to calculate the new position.
 * @param output The output stream to write the generated code to.
 * @param yName The name of the Y position variable.
 * @param rotName The name of the rotation variable.
 */
template<typename T>
void CalcPosY(
    const T len,
    std::ostream &output,
    const char *yName, 
    const char *rotName){
  output << yName << " + " << len << " * sin(" << rotName << ')';
}

/**
 * @brief Calculates the new Y position based on length and rotation.
 * 
 * This function calculates the new Y position using the given length and rotation variables.
 * 
 * @tparam T The type of the length parameter.
 * @param len The length to calculate the new position.
 * @param output The output stream to write the generated code to.
 * @param vars The VariableHeandler object containing variable names.
 */
template<typename T>
void CalcPosY(
    const T len,
    std::ostream &output,
    VariableHeandler &vars){
  output  << vars.getVariableNameString(POS_Y) 
          << " + " 
          << len 
          << " * "
          << "sin(" << vars.getVariableNameString(ROTATION) << ')';
}

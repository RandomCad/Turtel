#ifndef MY_VISITOR_FILE
#define MY_VISITOR_FILE

#include "libs/SceneBaseVisitor.h"

class MyVisitor : public SceneBaseVisitor{
  std::any visitInt(SceneParser::IntContext *ctx) override;
};

#endif

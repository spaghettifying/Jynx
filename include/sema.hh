#ifndef SEMA_H_
#define SEMA_H_

#include "ast.hh"
#include "context.hh"

class Sema {
 public:
  Sema(CompilerContext& ctx) : ctx(ctx) {}
  ProgramNode* analyze(ProgramNode&);

 private:
  CompilerContext& ctx;
};

#endif  // SEMA_H_

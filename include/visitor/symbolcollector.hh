#ifndef SYMBOLCOLLECTOR_H_
#define SYMBOLCOLLECTOR_H_

#include "ast.hh"
#include "context.hh"

class SymbolCollector {
 public:
  static constexpr const char* LOG_KIND = "SymbolCollector";

  SymbolCollector(CompilerContext& ctx) : ctx(ctx) {}

  void collect(ProgramNode& program) { collectProgram(program); }

 private:
  CompilerContext& ctx;

  void collectStatement(StmtNode& stmt);
  void collectExpression(ExprNode& expr);

  void collectProgram(ProgramNode& node);
  void collectBlock(BlockNode& node);
  void collectMethodDecl(MethodDeclNode& node);
  void collectIfStmt(IfStmtNode& node);
  void collectWhileStmt(WhileStmtNode& node);

  void collectBinaryExpr(BinaryExprNode& node);
  void collectAssignmentExpr(AssignmentExprNode& node);
  void collectMethodCall(MethodCallNode& node);
  void collectArgument(ArgumentNode& node);
  void collectVarDecl(VarDeclNode& node);

  void collectParamNode(ParamNode& node);
};

#endif  // SYMBOLCOLLECTOR_H_

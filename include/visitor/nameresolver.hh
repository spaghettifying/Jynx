#ifndef NAMERESOLVER_H_
#define NAMERESOLVER_H_

#include <string>
#include <vector>

#include "ast.hh"
#include "context.hh"

class NameResolver {
 public:
  constexpr static const char* LOG_KIND = "NameResolver";

  NameResolver(CompilerContext& ctx) : ctx(ctx) {}

  void resolve(ProgramNode& program) { resolveProgram(program); }

 private:
  CompilerContext& ctx;

  void resolveStatement(StmtNode& stmt);
  void resolveExpression(ExprNode& expr);

  void resolveProgram(ProgramNode& node);
  void resolveBlock(BlockNode& node);
  void resolveVarDecl(VarDeclNode& node);
  void resolveIfStmt(IfStmtNode& node);
  void resolveWhileStmt(WhileStmtNode& node);
  void resolveReturn(ReturnStmtNode& node);
  void resolveExprStmt(ExprStmtNode& node);
  void resolveMethodDecl(MethodDeclNode& node);

  void resolveBinaryExpr(BinaryExprNode& node);
  void resolveUnaryExpr(UnaryExprNode& node);
  // void resolveLiteralExpr(LiteralExprNode& node);
  void resolveIdentifierExpr(IdentifierExprNode& node);
  void resolveAssignmentExpr(AssignmentExprNode& node);
  void resolveMethodCall(MethodCallNode& node);
  // void resolveArgument(ArgumentNode& node);

  const std::vector<Symbol>* find_method_overloads(const std::string& owner,
                                                   const std::string& name);
};

#endif  // NAMERESOLVER_H_

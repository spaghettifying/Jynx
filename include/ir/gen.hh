#ifndef GEN_H_
#define GEN_H_

#include "ast.hh"
#include "context.hh"
#include "ir/basic_block.hh"
#include "ir/function.hh"
#include "ir/module.hh"

class IRGenerator {
 public:
  static constexpr const char* LOG_KIND = "IRGenerator";

  IRGenerator(CompilerContext& ctx) : ctx(ctx) {}

  ir::Module* generateIR(ProgramNode& program) {
    generateProgram(program);
    return current_module;
  }

 private:
  CompilerContext& ctx;
  ir::Module* current_module = nullptr;
  ir::BasicBlock* current_block = nullptr;

  void generateStatement(StmtNode& stmt);
  ir::Value* generateExpression(ExprNode& expr);

  void generateExprStmt(ExprStmtNode& node);

  // blocks
  void generateProgram(ProgramNode& node);
  void generateBlock(BlockNode& node);

  // methods
  void generateMethodDecl(MethodDeclNode& node);
  void generateMethodCall(MethodCallNode& node);
  void generateParam(ParamNode& node);
  void generateArgument(ArgumentNode& node);
  void generateReturnStmt(ReturnStmtNode& node);

  // control flow
  void generateIfStmt(IfStmtNode& node);
  void generateElseStmt(ElseStmtNode& node);
  void generateWhileStmt(WhileStmtNode& node);

  // expressions
  void generateBinaryExpr(BinaryExprNode& node);
  void generateUnaryExpr(UnaryExprNode& node);
  ir::Value* generateLiteralExpr(LiteralExprNode& node);

  // variables
  void generateAssignmentExpr(AssignmentExprNode& node);
  void generateVarDecl(VarDeclNode& node);
  void generateIdentifierExpr(IdentifierExprNode& node);

  // classes
  void generateClassMember(ClassMemberNode& node);
  void generateClass(ClassNode& node);
  void generateFieldDecl(FieldDeclNode& node);
  void generateConstructorDecl(ConstructorDeclNode& node);
};

#endif  // GEN_H_

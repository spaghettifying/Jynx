#include "ir/gen.hh"

#include "diagnostics.hh"

void IRGenerator::generateProgram(ProgramNode& node) {
  for (auto& child : node.children) generateStatement(*child);
}

void IRGenerator::generateStatement(StmtNode& stmt) {
  if (auto* node = dynamic_cast<BlockNode*>(&stmt))
    generateBlock(*node);
  else if (auto* node = dynamic_cast<MethodDeclNode*>(&stmt))
    generateMethodDecl(*node);
  else if (auto* node = dynamic_cast<MethodCallNode*>(&stmt))
    generateMethodCall(*node);
  else if (auto* node = dynamic_cast<ParamNode*>(&stmt))
    generateParam(*node);
  else if (auto* node = dynamic_cast<ArgumentNode*>(&stmt))
    generateArgument(*node);
  else if (auto* node = dynamic_cast<ReturnStmtNode*>(&stmt))
    generateReturnStmt(*node);
  else if (auto* node = dynamic_cast<IfStmtNode*>(&stmt))
    generateIfStmt(*node);
  else if (auto* node = dynamic_cast<ExprStmtNode*>(&stmt))
    generateExprStmt(*node);
  else
    Diagnostics::instance().report_error(LOG_KIND, "Unknown statement type",
                                         stmt.location);
}

ir::Value* IRGenerator::generateExpression(ExprNode& expr) {
  if (auto* node = dynamic_cast<BinaryExprNode*>(&expr))
    generateBinaryExpr(*node);
  else if (auto* node = dynamic_cast<UnaryExprNode*>(&expr))
    generateUnaryExpr(*node);
  else if (auto* node = dynamic_cast<LiteralExprNode*>(&expr))
    return generateLiteralExpr(*node);
  else if (auto* node = dynamic_cast<AssignmentExprNode*>(&expr))
    generateAssignmentExpr(*node);
  else if (auto* node = dynamic_cast<VarDeclNode*>(&expr))
    generateVarDecl(*node);
  else if (auto* node = dynamic_cast<IdentifierExprNode*>(&expr))
    generateIdentifierExpr(*node);
  else
    Diagnostics::instance().report_error(LOG_KIND, "Unknown expression type",
                                         expr.location);
}

void IRGenerator::generateExprStmt(ExprStmtNode& node) {
  generateExpr(*node.expr);
}

void IRGenerator::generateVarDecl(VarDeclNode& node) {
  if (!node.declared_type)
    Diagnostics::instance().report_error(
        LOG_KIND, "Variable declaration has no type.", node.location);
  // op = alloca, type = type, parent
  ir::Instruction* ret =
      new ir::Instruction(ir::Opcode::Alloca, node.declared_type, current_block,
                          node.identifier.to_string());

  ir::Value* type =
      new ir::Value(node.declared_type, node.declared_type->to_string());

  ret->addOperand(type);

  if (node.initializer) {
    ir::Value* init = generateExpression(*node.initializer);
    ret->addOperand(init);
  }

  current_block->appendInstruction(std::make_unique<ir::Instruction>(*ret));
}

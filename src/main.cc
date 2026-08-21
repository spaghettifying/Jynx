#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "diagnostics.hh"
#include "lexer.hh"
#include "log.hh"
#include "parser.hh"
#include "sema.hh"

//
#include "ir/module.hh"
#include "ir/printer.hh"

void print_usage(char** argv) {
  LOG_FATAL("USAGE: {} <path-to-file>\n", argv[0]);
  exit(1);
}

int main(const int argc, char** argv) {
  Diagnostics::instance().clear();
  if (argc != 2) print_usage(argv);

  std::string filepath = argv[1];

  std::ifstream file(filepath, std::ios::binary);
  if (!file) {
    LOG_FATAL("Could not open file: {}", filepath);
    exit(1);
  }

  CompilerContext ctx;

  Lexer lexer(file, ctx);

  Parser parser(lexer, ctx);

  std::unique_ptr<ProgramNode> ast(parser.parseProgram());
  if (ast != nullptr) {
    Log::print_ast_reflection(ast.get());
  } else {
    LOG_ERROR("Parser returned null - no AST generated");
    return 1;
  }

  if (Diagnostics::instance().has_errors()) {
    Diagnostics::instance().print_errors();
    return 1;
  }

  Sema sema(ctx);
  ProgramNode* sema_tree = sema.analyze(*ast);

  if (!sema_tree) {
    if (Diagnostics::instance().has_errors()) {
      Diagnostics::instance().print_errors();
      LOG_ERROR("Semantic analysis failed; skipping code generation");
      return 1;
    }
  }

  // delete

  using namespace ir;

  Module mod;
  Function* func = mod.create_function("max", ctx.get_int32_type());

  BasicBlock* entry = func->create_block("entry");
  BasicBlock* then_bb = func->create_block("then");
  BasicBlock* else_bb = func->create_block("else");
  BasicBlock* merge = func->create_block("merge");

  // entry:
  auto* a = new GetArgumentInstruction(ctx.get_int32_type(), 0);
  a->set_name("a");
  entry->append(std::unique_ptr<Instruction>(a));

  auto* b = new GetArgumentInstruction(ctx.get_int32_type(), 1);
  b->set_name("b");
  entry->append(std::unique_ptr<Instruction>(b));

  // For now we fake the comparison (you can add ICmp later)
  // Just assume we have a condition value for demonstration
  auto* cond =
      new Instruction(Opcode::Add, ctx.get_bool_type());  // placeholder
  cond->set_name("cond");
  entry->append(std::unique_ptr<Instruction>(cond));

  // br %cond, then, else
  auto* br = new Instruction(Opcode::Branch, ctx.get_void_type());
  br->set_args({cond});
  entry->append(std::unique_ptr<Instruction>(br));
  entry->set_successors({then_bb, else_bb});

  // then:
  // Upsilon(%a, ^result)
  auto* phi = new PhiInstruction(ctx.get_int32_type());
  phi->set_name("result");

  auto* upsilon_then = new UpsilonInstruction(a, phi);
  then_bb->append(std::unique_ptr<Instruction>(upsilon_then));

  auto* jmp_then = new Instruction(Opcode::Jump, ctx.get_void_type());
  then_bb->append(std::unique_ptr<Instruction>(jmp_then));
  then_bb->set_successors({merge});

  // else:
  auto* upsilon_else = new UpsilonInstruction(b, phi);
  else_bb->append(std::unique_ptr<Instruction>(upsilon_else));

  auto* jmp_else = new Instruction(Opcode::Jump, ctx.get_void_type());
  else_bb->append(std::unique_ptr<Instruction>(jmp_else));
  else_bb->set_successors({merge});

  // merge:
  merge->append(std::unique_ptr<Instruction>(phi));  // the Phi itself

  auto* ret = new Instruction(Opcode::Return, ctx.get_void_type());
  ret->set_args({phi});
  merge->append(std::unique_ptr<Instruction>(ret));

  IRPrinter::print(mod);

  // delete

  return 0;
}

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <system_error>

#include "../compiler/parsing/tokenizer.h"
#include "../compiler/parsing/parser.h"
#include "../compiler/parsing/node.h"
#include "../include/engine.h"


namespace olcl
{
  namespace
  {
    void dump_ast(olcl::node & node, std::uint32_t depth, bool escape_next = true)
    {
      using nt = olcl::node_type;
      using nvt = olcl::symbol_type;

      switch (node.type)
      {
        case nt::t_module:
          for (auto binding : *node.module.bindings)
          {
            dump_ast(*binding, depth);
            std::cout << "\n";
          }
          break;

        case nt::t_binding:
          std::cout << *node.binding.name << " : ";
          dump_ast(*node.binding.type, depth, false);
          std::cout << " = ";
          dump_ast(*node.binding.value, depth, false);
          std::cout << ";";
          break;

        case nt::t_type:
          std::cout << *node.typeinfo.name;
          break;

        case nt::t_value:
          switch (node.value.type)
          {
            case nvt::t_character:
              std::cout << '\'' << (char) node.value.character;
              break;
            case nvt::t_number:
              std::cout << *node.value.number;
              break;
            case nvt::t_string:
              std::cout << *node.value.string;
              break;
            case nvt::t_variable:
              std::cout << *node.value.variable;
              break;
            case nvt::t_function:
              if (escape_next) std::cout << '(';
              dump_ast(*node.value.function.parameters, depth);
              std::cout << " -> ";
              dump_ast(*node.value.function.return_type, depth);
              std::cout << ' ';

              if (node.value.function.code)
                dump_ast(*node.value.function.code, depth);

              if (escape_next) std::cout << ')';
              break;
          }
          break;

        case nt::t_binary_operation:
          if (escape_next) std::cout << '(';
          dump_ast(*node.binary_operation.left, depth);
          std::cout << ' ' << node.binary_operation.operation->representation << ' ';
          dump_ast(*node.binary_operation.right, depth);
          if (escape_next) std::cout << ')';
          break;

        case nt::t_unary_operation:
          std::cout << '(' << node.unary_operation.operation->representation;
          dump_ast(*node.unary_operation.expression, depth);
          std::cout << ')';
          break;

        case nt::t_function_call:
          dump_ast(*node.call.expression, depth);
          std::cout << '(';
          dump_ast(*node.call.arguments, depth, false);
          std::cout << ')';
          break;

        case nt::t_block:
          std::cout << "{\n";

          for (auto e : *node.block.code)
          {
            for (std::uint32_t i = 0; i < depth + 1; ++i)
              std::cout << "  ";
            dump_ast(*e, depth + 1u, false);
            std::cout << ";\n";
          }

          for (std::uint32_t i = 0; i < depth; ++i)
              std::cout << "  ";
          std::cout << '}';
          break;

        case nt::t_list:
          if (escape_next) std::cout << '(';
          for (auto e : *node.list.values)
          {
            dump_ast(*e, depth);
            std::cout << ", ";
          }
          if (escape_next) std::cout << ')';
          break;

        case nt::t_stmt_return:
          std::cout << "return ";
          dump_ast(*node.stmt_return.expression, depth, false);
          break;

        case nt::t_stmt_if:
          std::cout << "if ";
          dump_ast(*node.stmt_if.condition, depth);
          std::cout << " then ";
          dump_ast(*node.stmt_if.on_true, depth);

          if (node.stmt_if.on_false)
          {
            std::cout << " else ";
            dump_ast(*node.stmt_if.on_false, depth);
          }

          break;
      }
    }
  }

  script * engine::loadScript(std::string filename)
  {
    using isb_char_iter = std::istreambuf_iterator<char>;

    std::string data;
    try
    {
      std::ifstream source;
      source.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      source.open(filename);
      data = std::string(isb_char_iter(source), isb_char_iter());
    }
    catch (std::exception &e)
    {
      std::cout << "Error #LC001:\n"
                << "File " << filename << " can not be opened" << std::endl;
      
      return nullptr;
    }

    try
    {
      auto tokens = olcl::parsing::tokenize(data);
      auto ast = olcl::parsing::parse(tokens);

      dump_ast(*ast, 0);
      auto s = new script(ast);

      std::cout << std::flush;
      return s;
    }
    catch (std::runtime_error &e)
    {
      // fail
      return nullptr;
    }
  }
}

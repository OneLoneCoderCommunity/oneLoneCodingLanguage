#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "operators.h"


namespace olcl
{
  using namespace olcl::parsing;

  enum class node_type
  {
    t_module,

    t_binding,
    t_block,
    t_list,

    t_unary_operation,
    t_binary_operation,

    t_function_call,

    t_type,
    t_value,
  };

  enum class symbol_type
  {
    t_variable,
    
    t_string,
    t_character,
    t_number,

    t_function,
  };


  struct node;
  struct context;

  struct symbol
  {
    symbol_type type;
    union
    {
      // TODO: use special type
      std::string *variable;
      std::string *string;
      std::uint16_t character;
      // TODO: use special type
      std::string *number;

      struct
      {
        context * context;

        node * return_type;
        node * parameters;

        node * code;
      } function;
    };
  };

  struct context
  {
    context * parent;
    std::map<std::string, symbol *> table;
  };

  struct node
  {
    node_type type;

    union
    {
      struct
      {
        std::vector<node *> *bindings;
      } module;

      struct
      {
        std::string *name;
        node *type;
        node *value;
      } binding;

      struct
      {
        std::vector<node *> * code;
      } block;

      struct
      {
        std::vector<node *> * values;
      } list;

      struct
      {
        node *left, *right;
        operator_t const * operation;
      } binary_operation;

      struct
      {
        node * expression;
        operator_t const * operation;
      } unary_operation;

      struct
      {
        node * expression;
        node * arguments;
      } call;


      struct
      {
        std::string * name;

        // flags
        bool is_const;
        // ...
      } typeinfo;

      olcl::symbol value;
    };

    node() {}
  };

  namespace parsing
  {
    inline node * make_node_bop(node * left, node * right, operator_t const * operation)
    {
      node & container = *(new node);
      container.type = node_type::t_binary_operation;
      container.binary_operation.left = left;
      container.binary_operation.right = right;
      container.binary_operation.operation = operation;
      return &container;
    }
  }
}

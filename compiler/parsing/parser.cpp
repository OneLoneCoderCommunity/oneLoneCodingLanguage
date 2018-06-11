#include "../error.h"
#include "parser.h"
#include "node.h"


namespace olcl { namespace parsing {
  namespace
  {
    node * parse_value(parser::state &state);
    node * parse_unit(parser::state &state);
    operator_t const & parse_operator(parser::state &state);
    node * parse_type(parser::state &state);

    node * parse_expression(parser::state &state);
    node * parse_expression(node * left_operand, parser::state &state, int base_priority);

    node * parse_binding(parser::state &state);
    node *parse_module(parser::state &state);

    operator_t const & parse_operator(parser::state &state)
    {
      if (state.peek().type != token_type::t_operator)
        return operator_unknown;

      operator_t const * operation = &operator_unknown;
      std::string representation;
      state.save();

      while (state.peek().type == token_type::t_operator && representation.size() < 5)
      {
        representation += state.peek().value;
        state.move();

        for (std::size_t index = 0; index < sizeof operators_list / sizeof 0[operators_list]; ++index)
          if (operators_list[index].representation == representation)
            operation = &operators_list[index];
      }

      state.restore();
      for (std::size_t i = 0; i < operation->representation.size(); ++i)
          state.move();

      return *operation;
    }

    node * parse_value(parser::state &state)
    {
      auto &container = *(new node);

      switch (state.peek().type)
      {
        case token_type::t_identifier:
          container.type = node_type::t_value;
          container.value.type = symbol_type::t_variable;
          container.value.variable = new std::string(state.peek().value);

          state.move();
          return &container;

        case token_type::t_number:
          container.type = node_type::t_value;
          container.value.type = symbol_type::t_number;
          container.value.number = new std::string(state.peek().value);

          state.move();
          return &container;

        case token_type::t_string:
          container.type = node_type::t_value;
          container.value.type = symbol_type::t_string;
          container.value.string = new std::string(state.peek().value);

          state.move();
          return &container;

        case token_type::t_character:
          container.type = node_type::t_value;
          container.value.type = symbol_type::t_character;
          // TODO: Add escape sequence support
          // TODO: Add wide characters support
          container.value.character = state.peek().value[0];

          state.move();
          return &container;

        case token_type::t_operator:
        {
          switch (state.peek().sub_type)
          {
            case token_sub_type::t_brace_round_left:
            {
              state.move();
              if (state.peek().sub_type == token_sub_type::t_brace_round_right)
              {
                state.move();

                node & c = *(new node);
                c.type = node_type::t_list;
                c.list.values = new std::vector<node *>;

                return &c;
              }

              node * container = parse_unit(state);
              if (state.is_error_occurred())
                return nullptr;

              if (state.peek().sub_type != token_sub_type::t_brace_round_right)
              {
                state.error.code = error_t::p_closing_brace_expected;
                state.error.message = "value: matching closing brace ')' is not found";
                return nullptr;
              }

              state.move();
              return container;
            }

            case token_sub_type::t_brace_curly_left:
            {
              state.move();
              node & container = *(new node);
              container.type = node_type::t_block;
              container.block.code = new std::vector<node *>;

              while (state.peek().sub_type != token_sub_type::t_brace_curly_right)
              {
                container.block.code->push_back(parse_unit(state));
                
                if (state.is_error_occurred())
                  return nullptr;

                while (state.peek().sub_type == token_sub_type::t_semicolon)
                  state.move();
              }

              state.move(); // skip closing curly brace '}'
              return &container;
            }

            default: break;
          }

          state.error.code = error_t::p_value_expected;
          state.error.message = "Expected a value, but <" + get_sub_type_name(state.peek().sub_type) + "> was given";
          return nullptr;
        }

        default:
          state.error.code = error_t::p_value_expected;
          state.error.message = "Expected a value, but <"
            + get_sub_type_name(state.peek().sub_type) + "> was given";
          return nullptr;
      }
    }

    node * parse_complex_value(parser::state &state)
    {
      node * value = parse_value(state);
      if (state.is_error_occurred())
        return nullptr;

      while (state.peek().sub_type == token_sub_type::t_dot)
      {
        state.move();
        if (state.peek().sub_type != token_sub_type::t_identifier)
        {
          state.error.code = error_t::p_identifier_expected;
          state.error.message = "complex_value: <identifer> expected, but <"
            + get_sub_type_name(state.peek().sub_type) + ">(" + state.peek().value + ") was given";
          return nullptr;
        }

        auto rp = new node;
        rp->type = node_type::t_value;
        rp->value.type = symbol_type::t_variable;
        rp->value.variable = new std::string(state.peek().value);
        value = make_node_bop(value, rp, &operator_member_access);
        state.move();
      }

      if (state.peek().sub_type == token_sub_type::t_brace_round_left)
      {
        state.move();

        node * args = nullptr;
        if (state.peek().sub_type == token_sub_type::t_brace_round_right)
        {
          args = new node;
          args->type = node_type::t_list;
          args->list.values = new std::vector<node *>;
        } else args = parse_unit(state);

        if (state.is_error_occurred())
          return nullptr;

        if (state.peek().sub_type != token_sub_type::t_brace_round_right)
        {
          state.error.code = error_t::p_closing_brace_expected;
          state.error.message = "complex_value: Closing round brace ')' expected after arguments list, but <"
            + get_sub_type_name(state.peek().sub_type) + "> was given";
          return nullptr;
        }

        state.move(); // skip brace round right ')'

        node & container = *(new node);
        container.type = node_type::t_function_call;
        container.call.expression = value;
        container.call.arguments = args;

        return &container;
      }

      auto op = &parse_operator(state);
      if (op == &operator_fun_rt_def)
      {
        auto ret_type = parse_unit(state);
        if (state.is_error_occurred())
          return nullptr;

        node & container = *(new node);
        container.type = node_type::t_value;
        container.value.type = symbol_type::t_function;

        container.value.function.context = nullptr;
        container.value.function.return_type = ret_type;
        container.value.function.parameters = value;

        if (ret_type->type == node_type::t_value && ret_type->value.type == symbol_type::t_function)
        {
          container.value.function.code = ret_type->value.function.code;
          ret_type->value.function.code = nullptr;
        }
        else if (state.peek().sub_type == token_sub_type::t_brace_curly_left)
        {
          container.value.function.code = parse_value(state);
          if (state.is_error_occurred())
            return nullptr;
        }

        return &container;
      }
      state.index -= op->representation.size();

      return value;
    }

    node * parse_simple_unit(parser::state &state)
    {
      auto value = parse_expression(state);

      if (state.is_error_occurred())
        return nullptr;

      auto op = &parse_operator(state);
      if (op == &operator_type_cast)
      {
        auto type = parse_type(state);
        if (state.is_error_occurred())
          return nullptr;

        return parse_expression(make_node_bop(value, type, op), state, 0);
      }

      state.index -= op->representation.size(); 

      return value;
    }

    node * parse_unit(parser::state &state)
    {
      auto value = parse_simple_unit(state);

      if (state.is_error_occurred())
        return nullptr;

      if (state.peek().sub_type == token_sub_type::t_comma)
      {
        node & container = *(new node);
        container.type = node_type::t_list;
        container.list.values = new std::vector<node *>;
        container.list.values->push_back(value);

        do
        {
          state.move();
          if (false
              || state.peek().type == token_type::t_identifier
              || state.peek().type == token_type::t_string
              || state.peek().type == token_type::t_character
              || state.peek().type == token_type::t_number)
          {
            container.list.values->push_back(parse_simple_unit(state));
            if (state.is_error_occurred())
              return nullptr;
          }
        } while (state.peek().sub_type == token_sub_type::t_comma);

        return &container;
      }

      return value;
    }

    node * parse_type(parser::state &state)
    {
      node & container = *(new node);
      container.type = node_type::t_type;

      container.typeinfo.is_const = false;

      if (state.peek().type != token_type::t_identifier)
      {
        state.error.code = error_t::p_identifier_expected;
        state.error.message = "type: expected <identifer>, but <"
          + get_sub_type_name(state.peek().sub_type) + "> was given";
        return nullptr;
      }

      container.typeinfo.name = new std::string(state.peek().value);
      state.move();

      return &container;
    }

    node * parse_expression(node * left_operand, parser::state &state, int base_priority)
    {
      operator_t const * operation = &parse_operator(state);
      // TODO: Implement the operators comparison 
      // TODO: Implement the operators comparison 
      // TODO: Implement the operators comparison 
      if (operation == &operator_unknown)
        return left_operand;

      if (operation->precedence == 0)
      {
        state.index -= operation->representation.size();
        return left_operand;
      }

      node * right_operand = parse_complex_value(state);
      if (state.is_error_occurred())
        return nullptr;

      // Get next operator
      operator_t const * next_operation = &parse_operator(state);

      while (next_operation->precedence > base_priority)
      {
          if (next_operation->precedence > operation->precedence)
          {
            // If the following operator has the higher priority
            // parse that subexpression first
            state.index -= next_operation->representation.size();
            right_operand = parse_expression(right_operand, state, operation->precedence);
            if (state.is_error_occurred())
              return nullptr;

            next_operation = &parse_operator(state);

            if (next_operation->precedence <= base_priority)
              break; // Stop going to prevent something bad to happen
          }

          left_operand = make_node_bop(left_operand, right_operand, operation);
          operation = next_operation;

          right_operand = parse_complex_value(state);
          if (state.is_error_occurred())
            return nullptr;

          next_operation = &parse_operator(state);
      }

      if (next_operation != &operator_unknown && next_operation->precedence <= base_priority && base_priority > 0)
        state.index -= next_operation->representation.size();

      return make_node_bop(left_operand, right_operand, operation);
    }

    node * parse_expression(parser::state &state)
    {
      node * left_operand = parse_complex_value(state);
      if (state.is_error_occurred())
        return nullptr;
      return parse_expression(left_operand, state, 0);
    }

    node * parse_binding(parser::state &state)
    {
      if (state.peek().type != token_type::t_identifier)
      {
        state.error.code = error_t::p_identifier_expected;
        state.error.message = "binding: <identifier> expected, but <"
          + get_sub_type_name(state.peek().sub_type) + "> was given";
        return nullptr;
      }

      auto name = new std::string(state.peek().value);
      state.move();

      node & auto_type = *(new node);
      auto_type.type = node_type::t_type;
      auto_type.typeinfo.name = new std::string("auto");
      auto_type.typeinfo.is_const = false;

      node &container = *(new node);
      container.type = node_type::t_binding;
      container.binding.name = name;
      container.binding.type = &auto_type;

      operator_t const * op = &parse_operator(state);
      if (op->representation == ":")
      {
        auto type = parse_type(state);
        if (state.is_error_occurred())
          return nullptr;

        container.binding.type = type;

        op = &parse_operator(state);
        delete &auto_type;
      }

      if (op->representation != "=")
      {
        state.error.code = error_t::p_equal_sign_expected;
        state.error.message = "Expected '=' before variable's value definition";
        return nullptr;
      }

      container.binding.value = parse_unit(state);

      return &container;
    }

    node * parse_statement(parser::state &state)
    {
      if (state.peek().type == token_type::t_identifier)
      {
        if (state.peek().value == "import")
        {
          state.move(); // skip 'import' identifer
          state.error.code = error_t::s_not_implemented;
          state.error.message = "import statement is not supported in the current compiler version";
          return nullptr;
        }
      }

      auto val = parse_binding(state);
      return val;
    }

    node *parse_module(parser::state &state)
    {
      auto &container = *(new node);
      container.type = node_type::t_module;
      container.module.bindings = new std::vector<node*>;

      while (!state.is_eof() && !state.is_error_occurred())
      {
        container.module.bindings->push_back(parse_statement(state));

        if (state.is_error_occurred())
          return nullptr;

        while (state.peek().sub_type == token_sub_type::t_semicolon)
          state.move();
      }

      return &container;
    }
  }

  node *parse(std::vector<token> &tokens)
  {
    parser::state state(tokens);
    auto module = parse_module(state);

    if (state.is_error_occurred())
      log_error(state);

    return module;
  }
}}

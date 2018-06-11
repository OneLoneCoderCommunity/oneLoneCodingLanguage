#include <utility>

#include "tokenizer.h"
#include "../error_handler.h"


namespace olcl { namespace parsing { namespace tokenizer {
  void state::move()
  {
    if (index >= source.size())
      return;

    ++column;
    if (source[index++] != '\n')
      return;

    ++line;
    column = 1;
  }

  parsing::token_sub_type get_character_type(char c)
  {
    using tst = token_sub_type;

    switch (c)
    {
      case '@': return tst::t_at_sign;

      case '(': return tst::t_brace_round_left;
      case ')': return tst::t_brace_round_right;
      case '[': return tst::t_brace_square_left;
      case ']': return tst::t_brace_square_right;
      case '<': return tst::t_brace_triangular_left;
      case '>': return tst::t_brace_triangular_right;
      case '{': return tst::t_brace_curly_left;
      case '}': return tst::t_brace_curly_right;

      case '+': return tst::t_plus;
      case '-': return tst::t_dash;
      case '~': return tst::t_tilde;
      case '.': return tst::t_dot;
      case ',': return tst::t_comma;

      case '*': return tst::t_star;
      case '/': return tst::t_slash;
      case '\\': return tst::t_backslash;
      case '%': return tst::t_percent;

      case '|': return tst::t_vertical_bar;
      case '&': return tst::t_ampersand;
      case '^': return tst::t_caret;

      case '=': return tst::t_equal;
      case '!': return tst::t_exclamation_mark;
      case '?': return tst::t_question_mark;

      case ':': return tst::t_colon;
      case ';': return tst::t_semicolon;

      default: return tst::t_unknown;
    }
  }

  parsing::token get_next_token(parsing::tokenizer::state &state)
  {
    while (std::isspace(state.peek()))
      state.move();

    if (state.is_eof())
    {
      return parsing::token{
        state.index, state.line, state.column,
        "##EOF##",
        token_type::t_eof, token_sub_type::t_eof
      };
    }

    token_sub_type sub_type = get_character_type(state.peek());
    if (sub_type != token_sub_type::t_unknown)
    {
      state.move();

      // skipping comments
      if (sub_type == token_sub_type::t_slash && state.peek() == '/')
      {
        do state.move();
        while (state.peek() != '\n');

        // TODO: remove the recursion
        return get_next_token(state);
      }

      return parsing::token{
        state.index - 1, state.line, state.column - 1,
        state.source.substr(state.index - 1, 1),
        token_type::t_operator,
        sub_type
      };
    }

    if (state.peek() == '"')
    {
      std::uint32_t length = 1;
      do
      {
        state.move(), ++length;

        if (state.peek() == '\\')
          state.move(), ++length;
        
        if (state.peek() == '\0')
        {
          state.error.code = error_t::t_unexpected_eof;
          state.error.message = "String was not closed";
          break;
        }
      } while (state.peek() != '"');
      state.move();

      return parsing::token{
        state.index - length, state.line, state.column - length,

        state.source.substr(state.index - length, length),

        token_type::t_string,
        token_sub_type::t_string
      };
    }

    if (state.peek() == '\'')
    {
      state.move();
      std::uint32_t length = 1;

      if (std::isspace(state.peek()) || std::iscntrl(state.peek()))
      {
        state.error.code = error_t::t_misleading_character;
        state.error.message =
          "Character definition is misleading\n"
          "Whitespace symbols should be written using escape sequences:\n"
          "  \\s - space\n"
          "  \\t - tabulation\n"
          "  \\n - new line\n"
          "  \\r - carriage return";
      }

      if (state.peek() == '\\')
      {
        state.move(), ++length;
        switch(state.peek())
        {
          case 'r': case 'n': case 't': case 's': break;
          default:
          state.error.code = error_t::t_special_character_not_exists;
          state.error.message = std::string("Special character identifier '")
            + state.peek() + "' does not exists";
        }
      }

      state.move();

      return parsing::token{
        state.index - length, state.line, state.column - length,

        state.source.substr(state.index - length, length),

        token_type::t_character, token_sub_type::t_character,
      };
    }

    if ('0' <= state.peek() && state.peek() <= '9')
    {
      std::uint32_t length = 0;
      bool is_decimal = false;
      do state.move(), ++length;
      while (false
             || std::isdigit(state.peek())
             || (state.peek() == '.' && !is_decimal && (is_decimal = true)));

      return parsing::token{
        state.index - length, state.line, state.column - length,

        state.source.substr(state.index - length, length),

        token_type::t_number,
        (is_decimal
         ? token_sub_type::t_decimal
         : token_sub_type::t_integer)
      };
    }

    if (false
        || std::isalpha(state.peek())
        || state.peek() == '_' || state.peek() == '$')
    {
      std::uint32_t length = 0;
      do state.move(), ++length;
      while (false
             || std::isalnum(state.peek())
             || state.peek() == '_' || state.peek() == '$');

      return parsing::token{
        state.index - length, state.line, state.column - length,
        state.source.substr(state.index - length, length),

        token_type::t_identifier, token_sub_type::t_identifier
      };
    }

    auto ret = parsing::token{
      state.index, state.line, state.column,
      state.source.substr(state.index, 1),

      token_type::t_unknown, token_sub_type::t_unknown
    };

    state.move();
    return ret;
  }
}}}

namespace olcl { namespace parsing {
  std::vector<token> tokenize(std::string &source)
  {
    std::vector<token> tokens;
    parsing::tokenizer::state state(source);

    while (state.index < source.size() && not state.is_error_occurred())
      tokens.push_back(tokenizer::get_next_token(state));

    if (state.is_error_occurred())
    {
      log_error(state);
    }

    return tokens;
  }
}}

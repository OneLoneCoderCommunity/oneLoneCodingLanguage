#pragma once

#include <cstddef>
#include <string>

namespace olcl { namespace parsing {
  enum class token_type
  {
    t_unknown,

    t_commentary,

    t_identifier,
    t_number,
    t_string,
    t_character,

    t_operator,

    t_eof,
  };

  enum class token_sub_type
  {
    t_unknown,

    t_inline_commentary, t_multiline_commentary,

    t_identifier,
    t_integer, t_decimal,
    t_string,
    t_character,

    t_at_sign,

    t_brace_round_left, t_brace_round_right,
    t_brace_square_left, t_brace_square_right,
    t_brace_triangular_left, t_brace_triangular_right,
    t_brace_curly_left, t_brace_curly_right,

    t_plus, t_dash, t_tilde,
    t_star, t_slash, t_backslash, t_percent,
    t_dot, t_comma,

    t_vertical_bar, t_ampersand, t_caret,

    t_equal, t_exclamation_mark, t_question_mark,
    t_colon, t_semicolon,

    t_eof,
  };

  struct token
  {
    std::uint32_t index, line, column;

    std::string value;
    token_type type;
    token_sub_type sub_type;

    friend std::ostream &operator<<(std::ostream &, token &);
  };

  std::string get_sub_type_name(token_sub_type type);

  std::ostream &operator<<(std::ostream &out, parsing::token &token);
}}

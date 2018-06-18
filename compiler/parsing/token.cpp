#include <iostream>

#include "token.h"


namespace olcl { namespace parsing {
  std::string get_sub_type_name(token_sub_type type)
  {
    using tst = token_sub_type;

    switch (type)
    {
      case tst::t_inline_commentary: return "comment/inline";
      case tst::t_multiline_commentary: return "comment/multiline";

      case tst::t_identifier: return "identifier";
      case tst::t_integer: return "number/integer";
      case tst::t_decimal: return "number/decimal";
      case tst::t_string: return "string";
      case tst::t_character: return "character";

      case tst::t_at_sign: return "op/at_sign";

      case tst::t_brace_round_left: return "op/brace_round_left";
      case tst::t_brace_round_right: return "op/brace_round_right";
      case tst::t_brace_square_left: return "op/brace_square_left";
      case tst::t_brace_square_right: return "op/brace_square_right";
      case tst::t_brace_triangular_left: return "op/brace_triangular_left";
      case tst::t_brace_triangular_right: return "op/brace_triangular_right";
      case tst::t_brace_curly_left: return "op/brace_curly_left";
      case tst::t_brace_curly_right: return "op/brace_curly_right";

      case tst::t_plus: return "op/plus";
      case tst::t_dash: return "op/dash";
      case tst::t_tilde: return "op/tilde";
      case tst::t_dot: return "op/dot";
      case tst::t_comma: return "op/comma";

      case tst::t_star: return "op/star";
      case tst::t_slash: return "op/slash";
      case tst::t_backslash: return "op/backslash";
      case tst::t_percent: return "op/percent";

      case tst::t_vertical_bar: return "op/vertical_bar";
      case tst::t_ampersand: return "op/ampersand";
      case tst::t_caret: return "op/caret";

      case tst::t_equal: return "op/equal_sign";
      case tst::t_exclamation_mark: return "op/exclamation_mark";
      case tst::t_question_mark: return "op/question_mark";

      case tst::t_colon: return "op/colon";
      case tst::t_semicolon: return "op/semicolon";

      case tst::t_eof: return "end_of_file";

      case tst::t_unknown: return "#???";
    }

    return "#fail";
  }

  std::ostream &operator<<(std::ostream &out, parsing::token &token)
  {
    out << token.line << '|' << token.column << ' '
        << get_sub_type_name(token.sub_type) << ' '
        << token.value;
    return out;
  }
}}

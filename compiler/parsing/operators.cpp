#include "operators.h"

#define ASSOC_LEFT false
#define ASSOC_RIGHT true

namespace olcl { namespace parsing {
  operator_t const operators_list[24]
  {
    operator_t("", 0, ASSOC_LEFT),       // 0 - unknown

    operator_t("::",   2, ASSOC_RIGHT),  // 1 - binding

    operator_t(".",    0, ASSOC_LEFT),   // 2 - member access
    operator_t("=",    1, ASSOC_RIGHT),  // 3 - assignment

    operator_t("^",  250, ASSOC_LEFT),   // 4 - power

    // TODO: Review operator's priority
    operator_t("%",  200, ASSOC_LEFT),   // 5 - modulo

    operator_t("*",  200, ASSOC_LEFT),   // 6 - multiplication
    operator_t("/",  200, ASSOC_LEFT),   // 7 - division

    operator_t("+",  100, ASSOC_LEFT),   // 8 - addition
    operator_t("-",  100, ASSOC_LEFT),   // 9 - subtraction


    operator_t(">=",  50, ASSOC_LEFT),   // 10 - greater or equal
    operator_t("<=",  50, ASSOC_LEFT),   // 11 - less or equal
    operator_t(">",   50, ASSOC_LEFT),   // 12 - greater
    operator_t("<",   50, ASSOC_LEFT),   // 13 - less

    operator_t("==",  40, ASSOC_LEFT),   // 14 - comparison
    operator_t("===", 40, ASSOC_LEFT),   // 15 - strict comparisong
    operator_t("!=",  40, ASSOC_LEFT),   // 16 - negated comparison
    operator_t("!==", 40, ASSOC_LEFT),   // 17 - negated strict comparison


    operator_t("&",  25, ASSOC_LEFT),    // 18 - bitwise and
    operator_t("|",  20, ASSOC_LEFT),    // 19 - bitwise or

    operator_t("&&", 15, ASSOC_LEFT),    // 20 - logical and
    operator_t("||", 10, ASSOC_LEFT),    // 21 - logical or

    operator_t(":",   0, ASSOC_LEFT),    // 22 - type cast
    operator_t("->",  0, ASSOC_LEFT),    // 23 - fun rt def
  };

  operator_t const & operator_unknown = operators_list[0];
  operator_t const & operator_member_access = operators_list[2];
  operator_t const & operator_type_cast = operators_list[22];
  operator_t const & operator_fun_rt_def = operators_list[23];
}}

#undef ASSOC_LEFT
#undef ASSOC_RIGHT

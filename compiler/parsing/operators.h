#pragma once

#include <string>

namespace olcl { namespace parsing {
  struct operator_t
  {
    int precedence;
    bool is_right_sided;
    std::string representation;

    operator_t(std::string representation_, int precedence_, bool is_right_sided_)
      : precedence(precedence_)
      , is_right_sided(is_right_sided_)
      , representation(representation_)
    { }
  };

  extern operator_t const operators_list[24];

  extern operator_t const & operator_unknown;
  extern operator_t const & operator_member_access;
  extern operator_t const & operator_type_cast;
  extern operator_t const & operator_fun_rt_def;
}}

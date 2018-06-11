#pragma once

namespace olcl { namespace parsing {
  enum class error_t
  {
    // Empty
    c_no_errors = 0,


    // Start up
    c_file_not_exists = 1,


    // Tokenization
    t_unexpected_eof = 10,

    t_invalid_identifier = 20,

    t_invalid_compiler_directive_name = 21,
    t_compiler_directive_name_not_stated = 22,

    t_special_character_not_exists = 30,
    t_misleading_character = 31,


    // Parsing
    p_identifier_expected = 100,
    p_method_type_expected = 101,
    p_method_name_expected = 102,
    p_opening_brace_expected = 103,

    p_duplicated_modifier = 110,
    p_reference_can_not_be_constant = 111,
    p_array_can_not_contain_references = 112,

    p_colon_before_type_specefication_expected = 120,
    p_double_colon_expected = 121,

    p_unknown_unit = 130,
    p_closing_brace_expected = 131,

    p_value_expected = 140,
    p_equal_sign_expected = 141,

    p_function_vertical_bar_after_type_expected = 150,
    p_function_vertical_bar_after_params_expected = 151,



    // Optimization
    // ...


    // Translation
    // ...


    // Compilation
    // ...


    // Special
    s_not_implemented = 1000,
  };
}}

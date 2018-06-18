#pragma once

#include <cstddef>
#include <cctype>
#include <vector>
#include <string>

#include "token.h"
#include "../error.h"


namespace olcl { namespace parsing {
  namespace tokenizer
  {
    struct state
    {
      std::uint32_t index, line, column;
      std::string &source;

      struct
      {
        error_t code;
        std::string message;
      } error;

    public:
      state(std::string & source_)
        : index(0), line(1), column(1)
        , source(source_)
        , error{error_t::c_no_errors, ""}
      { }

    public:
      inline char peek() const { return source[index]; }
      void move();

      inline bool is_eof() const { return index >= source.size(); }
      inline bool is_error_occurred() const { return error.code != error_t::c_no_errors; }
    };

    /// Converts character to it's corresponding type
    /// \param c target character
    /// \return character type or ::t_unknown
    parsing::token_sub_type get_character_type(char c);

    /// Gets a next token from the byte stream
    /// \param state current tokenization state
    /// \return parsed valuable token
    parsing::token get_next_token(parsing::tokenizer::state &state);
  }

  std::vector<token> tokenize(std::string &source);
}}

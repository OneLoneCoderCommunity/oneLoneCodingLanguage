#pragma once

#include <vector>
#include <string>

#include "../error_handler.h"
#include "operators.h"
#include "token.h"
#include "node.h"


namespace olcl { namespace parsing {
  namespace parser
  {
    struct state
    {
    public:
      std::vector<token> &tokens;
      std::size_t index;

      struct
      {
        error_t code;
        std::string message;
      } error;

    private:
      std::vector<std::size_t> saves;

    public:
      state(std::vector<token> &tokens_)
        : tokens(tokens_)
        , index(0)
        , error()
        , saves()
      { }


      inline bool is_eof() const { return index >= tokens.size() - 1; }
      inline bool is_error_occurred() const { return error.code != error_t::c_no_errors; }

      inline token peek() const { return tokens[index]; }

      inline void move()
      {
        if (index < tokens.size())
          ++index;
      }

      inline void save() { saves.push_back(index); }
      inline void drop() { saves.pop_back(); }
      inline void restore()
      {
        index = saves[saves.size() - 1];
        error.code = error_t::c_no_errors;
        error.message = "";
        saves.pop_back();
      }
    };
  }

  node *parse(std::vector<token> &tokens);
}}

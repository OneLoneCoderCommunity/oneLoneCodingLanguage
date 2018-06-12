#pragma once

namespace olcl
{
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
}

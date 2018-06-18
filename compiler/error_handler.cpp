#include <iostream>

#include "error_handler.h"
#include "parsing/tokenizer.h"
#include "parsing/parser.h"


namespace olcl
{
  template <typename T>
  static inline void log_error_common(T state)
  {
    std::cout << "Error #LC" << static_cast<int>(state.error.code) << ":\n";
    std::cout << state.error.message << "\n\n";
  }

  void log_error(parsing::tokenizer::state state)
  {
    log_error_common(state);

    std::cout << "Line: " << state.line << "\n";
    std::cout << "Column: " << state.column << "\n\n";

    throw std::runtime_error("halt");
  }

  void log_error(parsing::parser::state state)
  {
    log_error_common(state);

    std::cout << "Line: " << state.peek().line << "\n";
    std::cout << "Column: " << state.peek().column << "\n\n";
    // TODO: Implement specific error handling stuff
    // TODO: Implement specific error handling stuff
    // TODO: Implement specific error handling stuff
    throw std::runtime_error("halt");
  }
}

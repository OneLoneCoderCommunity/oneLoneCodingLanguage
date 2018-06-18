#pragma once


namespace olcl
{
  namespace parsing { namespace parser { class state; }}
  namespace parsing { namespace tokenizer { class state; }}

  void log_error(parsing::parser::state state);
  void log_error(parsing::tokenizer::state state);
}

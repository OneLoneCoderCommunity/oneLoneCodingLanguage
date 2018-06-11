#pragma once

#include "script.h"


namespace olcl
{
  struct symbol;
  
  class engine
  {
  public:
    static script * loadScript(std::string filename);
    static script * loadScriptFromString(std::string code);

  public:
    static symbol * call(symbol * symbol);
  };
}

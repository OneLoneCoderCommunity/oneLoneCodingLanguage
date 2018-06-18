#pragma once

#include "symbol.h"
#include "script.h"


namespace olcl
{
  class engine
  {
  public:
    static script * loadScript(std::string filename);
    static script * loadScriptFromString(std::string code);

  public:
    static symbol * call(symbol * symbol);
  };
}

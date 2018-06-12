#pragma once

#include <string>

#include "symbol.h"


namespace olcl
{
  struct node;

  class script
  {
  private:
    node * code;

  public:
    script(node * ast);

  public:
    symbol * findSymbol(std::string id) const;
  };
}

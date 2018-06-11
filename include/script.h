#pragma once

#include <string>


namespace olcl
{
  struct node;
  struct symbol;

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

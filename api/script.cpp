#include <cassert>

#include "../include/script.h"
#include "../compiler/parsing/node.h"


namespace olcl
{
  script::script(olcl::node * ast_)
    : code(ast_)
  {
    assert(ast_ != nullptr);
    assert(ast_->type == node_type::t_module);
  }

  symbol * script::findSymbol(std::string id) const
  {
    // TODO: Find that symbol
    return nullptr;
  }
}

#include <cassert>

#include "../include/script.h"
#include "../compiler/parsing/node.h"


namespace olcl
{
  namespace
  {
    symbol * val_to_symbol(node * type)
    {
      if (type->type != node_type::t_value)
      {
        // TODO: log error?
        return nullptr;
      }

      return &type->value;
    }
  }

  script::script(olcl::node * ast_)
    : code(ast_)
  {
    assert(ast_ != nullptr);
    assert(ast_->type == node_type::t_module);
  }

  symbol * script::findSymbol(std::string id) const
  {
    for (auto b : *code->module.bindings)
    {
      if (*b->binding.name == id)
        return val_to_symbol(b->binding.value);
    }

    return nullptr;
  }
}

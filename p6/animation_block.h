/*
  An Animation_block is a Statement_block with:
    a parameter symbol pointer (m_parameter_symbll)
    a name (m_name)
    a flag to indicate if the body exists 
        (forward + animation block -vs- forward w/o animation block)
 
  When an animation is executed, it needs to know both the formal and
  the actual parameters.  At parse time the formal parameter is known and
  is passed to Animation_block().

  At execution, the actual parameter is known and is passed to execute()

  p6 & p7:  do not have to change this class

  p8: must (1) implement flag to indicate if body exists
           (2) write the body of Animation_block::execute()
*/

#ifndef ANIMATION_BLOCK_H
#define ANIMATION_BLOCK_H

#include <string>

#include "symbol.h"
#include "statement_block.h"

class Animation_block : public Statement_block
{
  public:
    Animation_block(std::shared_ptr<Symbol> parameter_symbol, std::string name);
    void execute(Game_object* argument);

    std::shared_ptr<Symbol> get_parameter_symbol() {return m_parameter_symbol;}

    std::string name() {return m_name;}

    void mark_complete();
    bool complete();

    std::ostream &print(std::ostream &os) const;

  private:
    std::shared_ptr<Symbol> m_parameter_symbol;
    std::string m_name;
};

#endif // #ifndef ANIMATION_BLOCK_H

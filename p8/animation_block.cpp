#include <vector>
#include "parser.h"
#include "animation_block.h"
#include "game_object.h"
#include "indent.h"
#include "gpl_assert.h"
#include "symbol.h"
using namespace std;

Animation_block::Animation_block(int line, std::shared_ptr<Symbol> parameter_symbol, string name)
	: statement_block(line)
{
  m_name = name;
  m_parameter_symbol = parameter_symbol;
	_pExecuteThread = NULL;
}

Animation_block::~Animation_block()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if(_pExecuteThread) delete _pExecuteThread;
}


bool Animation_block::complete()
{
	if(_mutex.try_lock())
	{
		return _bRunning;
	}
	return false;
}

void Animation_block::execute(const std::shared_ptr<Game_object>& argument)
{
	std::lock_guard<std::mutex> lock(_mutex);	
	m_parameter_symbol->set_game_object(argument);

	if(_pExecuteThread) delete _pExecuteThread;
	_pExecuteThread = new std::thread(&Animation_block::executeAsync, this);
}

void Animation_block::execute()
{
	throw std::runtime_error("Animation_block::execute - method Deprecated");
}

void Animation_block::executeAsync()
{
	std::lock_guard<std::mutex> lock(_mutex);
	try
	{
		_bRunning = true;
		int count = get_count();
		for(int i = 0; i < count; i++)
		{
			get_statement(i)->execute();
		}
	}
	catch(...)
	{
		TRACE_ERROR("An error occurred while executing the Animation Block");
		_bRunning = false;
		throw;
	}
	_bRunning = false;
}

std::ostream& Animation_block::print(std::ostream &os) const
{
	//os << "Animation_block::print()\n";
	return os;
}

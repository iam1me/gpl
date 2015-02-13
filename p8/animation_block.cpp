#include <vector>
#include "parser.h"
#include "animation_block.h"
#include "game_object.h"
#include "indent.h"
#include "gpl_assert.h"
#include "gpl_exception.h"
#include "symbol.h"
using namespace std;

Animation_block::Animation_block(int line, std::shared_ptr<Symbol> parameter_symbol, string name)
	: statement_block(line)
{
	m_name = name;
	m_parameter_symbol = parameter_symbol;

	if(m_parameter_symbol->get_game_object(_pObjBackup) == CONVERSION_ERROR)
		throw undefined_error();

	_pExecuteThread = NULL;
}

Animation_block::~Animation_block()
{
	//std::lock_guard<std::mutex> lock(_mutex);
	//if(_pExecuteThread) delete _pExecuteThread;
}

bool Animation_block::complete()
{
	return _bRunning;
}

void Animation_block::execute(const std::shared_ptr<Game_object>& argument)
{
	//std::lock_guard<std::mutex> lock(_mutex);	
	//std::cout << "Animation_block::execute - There are " << get_count() << " statements.\n";
	m_parameter_symbol->set_game_object(argument);

	/*if(_pExecuteThread)
	{
		_pExecuteThread->join();
		delete _pExecuteThread;
	}
	_pExecuteThread = new std::thread(&Animation_block::executeAsync, this);*/
	execute();
}

void Animation_block::execute()
{
	//std::lock_guard<std::mutex> lock(_mutex);
	//std::cout << "Animation_block::executeAsync - BEGIN\n";
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
		//std::cerr << "Animation_block::executeAsync - ERROR\n";
		TRACE_ERROR("An error occurred while executing the Animation Block");
		
		//Restore the place-holder parameter object
		m_parameter_symbol->set_game_object(_pObjBackup);
		_bRunning = false;

		throw;
	}

	//Restore the place-holder parameter object
	//std::cout << "Animation_block::executeAsync - Complete\n";
	m_parameter_symbol->set_game_object(_pObjBackup);
	_bRunning = false;
}

std::ostream& Animation_block::print(std::ostream &os) const
{
	//os << "Animation_block::print()\n";
	return os;
}

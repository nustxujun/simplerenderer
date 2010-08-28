#include "CommandLine.h"
namespace RCP
{
	CommandLine::CommandLine()
	{}

	CommandLine::~CommandLine()
	{
		ProcessorFuncList::iterator i,endi;
		for (i = mProcessorFuncList.begin(); i != endi; ++i)
		{
			delete (*i);
		}
	}

	void CommandLine::pushCommand(unsigned int index,const ParameterList& params)
	{
		mCommadList.push_back(std::make_pair(index,params));
	}

	void CommandLine::pushCommand(unsigned int index,const Any& param)
	{
		ParameterList params;
		params.push_back(param);
		pushCommand(index,params);
	}

	void CommandLine::pushCommand(unsigned int index,const Any& param1,const Any& param2)
	{
		ParameterList params;
		params.push_back(param1);
		params.push_back(param2);
		pushCommand(index,params);	
	}

	void CommandLine::pushCommand(unsigned int index,const Any& param1,const Any& param2 ,const Any& param3)
	{
		ParameterList params;
		params.push_back(param1);
		params.push_back(param2);
		params.push_back(param3);
		pushCommand(index,params);
	}

	bool CommandLine::empty()
	{
		return mCommadList.empty();
	}

	void CommandLine::execute()
	{
		if (empty() || mProcessorFuncList.empty())
			return;
		CommadList::iterator comIter = mCommadList.begin();
		ProcessorFuncList::iterator i,endi = mProcessorFuncList.end();

		for (i = mProcessorFuncList.begin(); i != endi; ++i)
			(*i)->execute(comIter->first,comIter->second);

		mCommadList.pop_front();
	}

}
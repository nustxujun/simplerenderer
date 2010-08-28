#ifndef _CommandLine_H_
#define _CommandLine_H_
#include "Any.h"

namespace RCP
{
	class CommandLine
	{
	public:

		typedef std::vector<Any> ParameterList;

		class ProcessorFunc
		{
		public:
			virtual void execute(unsigned int index,const ParameterList& paras) = 0;
		};

		template<class T>
		class Function:public ProcessorFunc
		{
		public:
			typedef void (T::* FuncType)(unsigned int ,const ParameterList &);
		public:
			Function(T* inst, FuncType f):
			  instance(inst),func(f)
			{
			}
			
			virtual void execute(unsigned int index,const ParameterList& paras) 
			{
				(instance->*func)(index,paras);
			}
			

		public:

			T* instance;
			FuncType func;

		};

	public:
		CommandLine();
		~CommandLine();

		void pushCommand(unsigned int index,const ParameterList& params);
		void pushCommand(unsigned int index,const Any& param);
		void pushCommand(unsigned int index,const Any& param1,const Any& param2);
		void pushCommand(unsigned int index,const Any& param1,const Any& param2 ,const Any& param3);
		

		template<class T>
		void addProcessorFunc(T* inst,void (T::*func)(unsigned int ,const ParameterList& params ))
		{
			mProcessorFuncList.insert(new Function<T>(inst,func));
		}

		bool empty();

		void execute();
	private:

		typedef std::list<std::pair<unsigned int ,ParameterList>> CommadList;
		typedef std::set<ProcessorFunc*> ProcessorFuncList;

		CommadList mCommadList;
		ProcessorFuncList mProcessorFuncList;
	};
}
#endif//_CommandLine_H_

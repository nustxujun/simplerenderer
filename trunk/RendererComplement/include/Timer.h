#ifndef _Timer_H_
#define _Timer_H_

#include <windows.h>
#pragma comment(lib,"winmm.lib")



namespace RCP
{
	class Timer
	{
	public:
		Timer():
			time(0)
		{
			
		}

		void reset()
		{
			time = timeGetTime();
		}
		long getTime()
		{
			return timeGetTime() - time;
		}

	private:
		unsigned long time;
	};

	static Timer timer;
#define getWasteTime(func,fileName) { FILE* f;f = fopen(fileName,"a"); timer.reset(); func;fprintf(f,"%ld\n",timer.getTime());fclose(f); }
}
#endif//_Timer_H_

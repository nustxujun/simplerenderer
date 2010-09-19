#ifndef _PerformanceTester_H_
#define _PerformanceTester_H_

#include <map>
#include <string>
#include <list>
#include <vector>
class PerformanceTester
{
public :
	struct Result
	{
		unsigned int maxValue;
		unsigned int minValue;
		float average;
		std::list<float> sample;


		Result():
			maxValue(0),minValue(-1),average(0)
		{}
	};

	typedef std::map<std::string,Result> ResultList;
public :
	static PerformanceTester* mSingleton;

	static  PerformanceTester* getSingletonPtr()
	{
		return mSingleton;
	}

	PerformanceTester();

	void addItem(const std::string& item);
	void begin(const std::string& testItem);
	const Result& end();

	const Result& getResult(const std::string& item);
	const ResultList& getResultList();
private:
	ResultList mResultList;
	typedef std::vector<std::pair<Result*,unsigned long>> TimerStack;
	TimerStack mTimerStack;

};

#ifndef PERFORMANCE_TEST
#define PERFORMANCE_TEST(item,func) {PerformanceTester::getSingletonPtr()->begin(item); func; PerformanceTester::getSingletonPtr()->end();}
#endif//PERFORMANCE_TEST

#endif//_PerformanceTester_H_
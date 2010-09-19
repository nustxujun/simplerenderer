#include "PerformanceTester.h"
#include <windows.h>
#pragma comment(lib,"winmm.lib")

const int sampleSize = 10;
PerformanceTester* PerformanceTester::mSingleton = NULL;

PerformanceTester::PerformanceTester()
{
	mSingleton = this;
}

void PerformanceTester::addItem(const std::string& item)
{
	std::pair<ResultList::iterator,bool> result = mResultList.insert(ResultList::value_type(item,Result()));
	if (result.second)
		result.first->second.sample.resize(sampleSize);
}
void PerformanceTester::begin(const std::string& testItem)
{
	ResultList::iterator result = mResultList.find(testItem);
	if (result != mResultList.end())
	{
		mTimerStack.push_back(std::make_pair(&result->second,timeGetTime()));
	}
	else
	{
		mTimerStack.push_back(std::make_pair((Result*)NULL,timeGetTime()));
	}
	
}
const PerformanceTester::Result& PerformanceTester::end()
{
	Result* current = mTimerStack[mTimerStack.size() - 1].first;
	unsigned int timer = mTimerStack[mTimerStack.size() - 1].second;
	timer = timeGetTime() - timer;
	mTimerStack.pop_back();

	if (timer == 0)
		return *current;

	float time = (float)timer / 1000.f;
	float frame = 1.0f / time;

	current->maxValue = current->maxValue > frame? current->maxValue:frame;
	current->minValue = current->minValue < frame? current->minValue:frame;
	current->sample.push_back(frame / (float)sampleSize);
	current->average = current->average - (*current->sample.begin());
	current->average += (*current->sample.rbegin());
	current->sample.pop_front();

	return *current;
}

const PerformanceTester::Result& PerformanceTester::getResult(const std::string& item)
{
	return mResultList.find(item)->second;
}
const PerformanceTester::ResultList& PerformanceTester::getResultList()
{
	return mResultList;
}
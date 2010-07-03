#ifndef _Pipeline_H_
#define _Pipeline_H_

#include "Prerequisites.h"
#include "MemoryBuffer.h"
#include "RenderData.h"

namespace RCP
{
	class Pipeline
	{
	public:
		class ResultListener
		{
		public:
			virtual ~ResultListener();
			virtual void exportResult(const MemoryBuffer& membuffer) = 0;
		};


	public:
		Pipeline();
		virtual ~Pipeline();

		void initialize(const RendererParameters& rp);
		virtual void initImpl() = 0;

		//将数据填入流水线
		void import(RenderData& data);
		//自定义流水线方法
		virtual void execute(const RenderData& renderData) = 0;
		//将listener之类的数据拷贝
		void copyState(const Pipeline& pipeline);
		//当mRenderDataList有数据时禁止拷贝状态。恩防止多线程出错？
		//总之通过这个判断
		bool isWorking();

		const RendererParameters& getRendererParameters()const ;

		template<class T>
		void clear(const T& t)
		{
			
		}
	protected:
		//通知接受结果的单位，并删除该RenderElement
		void notifyCompleted();
	private:
		//当前渲染的数据
		RenderData* mRenderData;

		RendererParameters mRendererParameters;

	};
}
#endif//_Pipeline_H_

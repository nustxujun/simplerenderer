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

		//������������ˮ��
		void import(RenderData& data);
		//�Զ�����ˮ�߷���
		virtual void execute(const RenderData& renderData) = 0;
		//��listener֮������ݿ���
		void copyState(const Pipeline& pipeline);
		//��mRenderDataList������ʱ��ֹ����״̬������ֹ���̳߳���
		//��֮ͨ������ж�
		bool isWorking();

		const RendererParameters& getRendererParameters()const ;

		template<class T>
		void clear(const T& t)
		{
			
		}
	protected:
		//֪ͨ���ܽ���ĵ�λ����ɾ����RenderElement
		void notifyCompleted();
	private:
		//��ǰ��Ⱦ������
		RenderData* mRenderData;

		RendererParameters mRendererParameters;

	};
}
#endif//_Pipeline_H_

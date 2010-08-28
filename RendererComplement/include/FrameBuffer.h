#ifndef _FrameBuffer_H_
#define _FrameBuffer_H_

#include "Prerequisites.h"
#include "RenderTarget.h"

namespace RCP
{

	class FrameBuffer
	{
	public :
		//ע���������ʹ��Ĭ�Ϲ��죬��������޷�setBuffer������������ԣ�
		//�������������봫����ֵ
		//Ĭ�Ϲ���ֻ����ֻ���п�����ʱ��ʹ��
		FrameBuffer();
		FrameBuffer(unsigned int w,unsigned int h);
		FrameBuffer(const FrameBuffer& fb);
		~FrameBuffer();

		void setBuffer(BufferTpye type,RenderTarget* rt);
		RenderTarget* getBuffer(BufferTpye type);


		/*
		Ϊ�����Renderer��ͳһ��Ⱦ(��Ⱦ״̬�����浽��renderNow�ٿ�ʼdraw)
		clear����Ҳ��Ϊ״̬��ʱ��ʵ��д��֮ǰ���С�
		�����Ҫע��ĵ��ⲿ����setValue getvalue��ʱ��
		�����դ����renderer��drawPrimitive���첽�ģ����ܻ�ʹframebuffer��ǰclear;
		��˽�ֹ�ⲿֱ�ӵ���setValue getvalue��ֻ���ڹ�դ����ʱ��ʹ�á�
		ͬ����ֹ���ⲿ�ṩframeBuffer;
		*/

		template<class T>
		void clear(BufferTpye type, const T& value)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
			{
				return ;
			}

			unsigned int valueSize = sizeof(value);
			if (rt->getColourDepth() != valueSize)
				THROW_EXCEPTION("value of clearing does not match colourDepth.");


			size_t size = rt->getWidth() * rt->getHeight();
			rt->seek(0);

			for (size_t i = 0; i < size; ++i)
			{
				rt->write(&value,valueSize);
			}

		}


		template<class T>
		inline void setValue(BufferTpye type,unsigned int x, unsigned int y,const T& value)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
				return;

			unsigned colorDepth = rt->getColourDepth();
			if (colorDepth != sizeof (value))
				THROW_EXCEPTION("value does not match colourDepth.");
			size_t pos = getBufferPos(x,y,rt->getWidth(),colorDepth);
			rt->seek(pos);
			rt->write(&value,sizeof (value));
		}

		template<class T>
		inline void getValue(T& t ,BufferTpye type,unsigned int x, unsigned int y)
		{
			assert(type < BT_COUNT);
			RenderTarget* rt = mBuffers[type];

			if (rt == NULL)
			{
				//����Ҫ�ÿ��������ܷ�������
				//memset(&t,0,sizeof(T));
				return;
			}

			unsigned colorDepth = rt->getColourDepth();
			if (colorDepth != sizeof (t))
				THROW_EXCEPTION("container does not match colourDepth.");
			size_t pos = getBufferPos(x,y,rt->getWidth(),colorDepth);
			rt->seek(pos);
			rt->read(&t,sizeof (T));
		}

		void operator = (const FrameBuffer& fb);
	private:


		inline size_t getBufferPos(unsigned int x, unsigned int y, unsigned int width, unsigned int colorDepth)
		{
			return (x + y * width) * colorDepth;
		}


	private:
		RenderTarget* mBuffers[BT_COUNT];

		unsigned int mWidth;
		unsigned int mHeight;

	};
}
#endif//_FrameBuffer_H_

#ifndef _ViewerParas_H_
#define _ViewerParas_H_

namespace Viewer
{
	class CViewerParas
	{
	public:
		CViewerParas()
		{
			mShow = true;
			mShowVertex = false;
			mShowEdge = false;
			mShowFace = true;
			useFaceNormal = false;

			computeNormal = true;
			cutPlane = false;
		};
		~CViewerParas() {};
	public:/*������ʾ����*/
		bool mShow;
		bool mShowVertex;
		bool mShowEdge;
		bool mShowFace;
		bool useFaceNormal;
		/* Ԥ������Ƽ��������
		   ��ز���ִֻ��һ��
		   �������������Ŀ��Ʋ���*/
		bool computeNormal;
		bool cutPlane;
	};
}




#endif

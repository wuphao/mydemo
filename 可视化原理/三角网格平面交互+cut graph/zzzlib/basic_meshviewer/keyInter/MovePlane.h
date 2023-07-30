#ifndef _MOVEPLANE_H_
#define _MOVEPLANE_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"

namespace Viewer
{

	template<typename M>
	class CMovePlane :public CKeyInter
	{
	public:
		CMovePlane(unsigned char _key,CTMesh * _pMesh, CViewerParas* _pParas,double step) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
			m_step = step;
		}
		~CMovePlane() {};
		virtual void keyInter(int _x, int _y)
		{
			position += m_step;
			if (m_pParas->cutPlane) {
				CPoint pA = plane_normal * position;

				//cout << pA[0] <<":"<<pA[1]<<":"<<pA[2] << endl;
				for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
				{
					M::CFace* pF = *fiter;
					bool flag = true;
					for (M::FaceVertexIterator fviter(pF); !fviter.end(); ++fviter)
					{
						CPoint p((*fviter)->point()[0], (*fviter)->point()[1], (*fviter)->point()[2]);
						if ((p - pA) * plane_normal < 0) {//p在平面法向量反方向
							flag = false;
						}
					}
					pF->setVisible(flag);
				}
			}
		}
		CViewerParas* m_pParas;
		CTMesh* m_pMesh;
		double m_step;
	};

	
}






#endif

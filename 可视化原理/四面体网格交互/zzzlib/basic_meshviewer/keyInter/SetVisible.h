#ifndef _SETVISIBLE_H_
#define _SETVISIBLE_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"

namespace Viewer
{

	template<typename M>
	class CSetVisible :public CKeyInter
	{
	public:
		CSetVisible(unsigned char _key,CTTMesh * _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CSetVisible() {};
		virtual void keyInter(int _x, int _y)
		{
			m_pParas->cutPlane = !(m_pParas->cutPlane);
			if (m_pParas->cutPlane) {
				CPoint pA = plane_normal * position;

				//cout << pA[0] <<":"<<pA[1]<<":"<<pA[2] << endl;
				for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
				{
					M::CFace* pF = *fiter;
					bool flag = true;
					for (M::FaceVertexIterator fviter(m_pMesh, pF); !fviter.end(); ++fviter)
					{
						CPoint p((*fviter)->position()[0], (*fviter)->position()[1], (*fviter)->position()[2]);
						if ((p - pA) * plane_normal < 0) {//p在平面法向量反方向
							flag = false;
						}
					}
					pF->setVisible(flag);
				}
			}
		}
		CViewerParas* m_pParas;
		CTTMesh* m_pMesh;
	};

	
}






#endif

#ifndef _BFS_H_
#define _BFS_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"
#include<queue>

namespace Viewer
{

	template<typename M>
	class CBFS :public CKeyInter
	{
	public:
		CBFS(unsigned char _key,CTMesh * _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CBFS() {};
		virtual void keyInter(int _x, int _y)
		{
			for (M::MeshVertexIterator iter(m_pMesh); !iter.end(); ++iter) {
				CVertex* vert = *iter;
				vert->setVisit(false);
			}
			for (M::MeshEdgeIterator iter(m_pMesh); !iter.end(); ++iter) {
				CEdge* pE = *iter;
				pE->setRGB(0, 0, 128.0 / 255.0);
			}
			queue<CVertex*> que;
			CVertex* root = *m_pMesh->vertices().begin();
			root->setVisit(true);
			que.push(root);
			while (!que.empty()) {
				CVertex* pV = que.front();
				que.pop();
				for (MeshLib::VertexEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> iter(pV); !iter.end(); ++iter) {
					CEdge* pE = *iter;
					for (int i = 0; i < 2; i++) {
						CVertex* desti = pE->halfedge(i)->target();
						if (!desti->getVisit()) {
							pE->setRGB(1, 1, 0);
							que.push(desti);
							desti->setVisit(true);
						}
					}
				}
			}
			
		}
		CViewerParas* m_pParas;
		CTMesh* m_pMesh;
	};

	
}






#endif

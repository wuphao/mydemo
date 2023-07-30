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
		CBFS(unsigned char _key,CTTMesh * _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CBFS() {};
		virtual void keyInter(int _x, int _y)
		{
			for (M::MeshVertexIterator iter(m_pMesh); !iter.end(); ++iter) {
				M::CVertex* vert = *iter;
				vert->vis = false;
			}
			for (M::MeshEdgeIterator iter(m_pMesh); !iter.end(); ++iter) {
				M::CEdge* pE = *iter;
				pE->setRGB(0, 0, 128.0 / 255.0);
			}
			queue<M::CVertex*> que;
			M::CVertex* root = *m_pMesh->vertices().begin();
			root->vis = true;
			que.push(root);
			while (!que.empty()) {
				M::CVertex* pV = que.front();
				que.pop();
				for (M::VertexEdgeIterator iter(m_pMesh,pV); !iter.end(); ++iter) {
					M::CEdge* pE = *iter;
					if (pE->boundary()) {
						M::CVertex* desti = (M::CVertex*)pE->vertex1();
						if (!desti->vis) {
							pE->setRGB(1, 1, 0);
							que.push(desti);
							desti->vis = true;
						}
						desti = (M::CVertex*)pE->vertex2();
						if (!desti->vis) {
							pE->setRGB(1, 1, 0);
							que.push(desti);
							desti->vis = true;
						}
					}
				}
			}
			
		}
		CViewerParas* m_pParas;
		CTTMesh* m_pMesh;
	};

	
}






#endif

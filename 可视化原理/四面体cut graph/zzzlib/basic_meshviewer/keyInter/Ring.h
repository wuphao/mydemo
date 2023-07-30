#ifndef _RING_H_
#define _RING_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"
#include<queue>
#include<set>
#include<algorithm>

namespace Viewer
{

	template<typename M>
	class CRing :public CKeyInter
	{
	public:
		CRing(unsigned char _key, CTTMesh* _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CRing() {}

		virtual void keyInter(int _x, int _y)
		{
			for (M::MeshVertexIterator iter(m_pMesh); !iter.end(); ++iter) {
				CVertex* vert = *iter;
				vert->prev = nullptr;
				vert->base = nullptr;
				vert->vis = false;
			}
			for (M::MeshEdgeIterator iter(m_pMesh); !iter.end(); ++iter) {
				CEdge* pE = *iter;
				pE->setRGB(0, 0, 128.0 / 255.0);
				pE->vis = false;
			}
			queue<CVertex*> que;
			vector<CEdge*> edges;
			CVertex* root = *m_pMesh->vertices().begin();
			root->vis = true;
			que.push(root);
			while (!que.empty()) {
				CVertex* pV = que.front();
				que.pop();
				for (M::VertexEdgeIterator iter(m_pMesh, (M::CVertex*)pV); !iter.end(); ++iter) {
					CEdge* pE = *iter;
					if(pE->boundary()){
						CVertex* desti = pE->vertex1();
						if (!desti->vis) {
							pE->vis = true;
							desti->prev = pV;
							desti->base = pV->base;
							if (desti->base == nullptr)desti->base = root;
							if (desti->base == root)desti->base = pV;
							que.push(desti);
							desti->vis = true;
						}
						desti = pE->vertex2();
						if (!desti->vis) {
							pE->vis = true;
							desti->prev = pV;
							desti->base = pV->base;
							if (desti->base == nullptr)desti->base = root;
							if (desti->base == root)desti->base = pV;
							que.push(desti);
							desti->vis = true;
						}
						if (!pE->vis) {
							edges.push_back(pE);

						}
					}
				}
			}

			random_shuffle(edges.begin(), edges.end());
			CVertex* selected = nullptr;
			for (CEdge* e : edges) {
				if(e->vertex1()->base != e->vertex2()->base){
					e->setRGB(1, 1, 0);
					CVertex* desti = e->vertex1();
					if (desti != root) {
						selected = desti;
						while (selected != nullptr && selected->prev != nullptr) {
							CEdge* edge = fromVV(selected, selected->prev);
							edge->setRGB(1, 1, 0);
							selected = selected->prev;
						}
					}
					desti = e->vertex2();
					if (desti != root) {
						selected = desti;
						while (selected != nullptr && selected->prev != nullptr) {
							CEdge* edge = fromVV(selected, selected->prev);
							edge->setRGB(1, 1, 0);
							selected = selected->prev;
						}
					}
					break;
				}
			}
			

		}

		CEdge* fromVV(CVertex* vert1, CVertex* vert2) {
			for (M::VertexEdgeIterator iter(m_pMesh, (M::CVertex*)vert1); !iter.end(); ++iter) {
				CEdge* pE = *iter;
				if (pE->boundary()) {
					CVertex* desti = pE->vertex1();
					if (desti == vert2) {
						return pE;
					}
					desti = pE->vertex2();
					if (desti == vert2) {
						return pE;
					}
				}
			}
			return nullptr;
		}
		CViewerParas* m_pParas;
		CTTMesh* m_pMesh;
	};

	
}






#endif

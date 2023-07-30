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

	CEdge* fromVV(CVertex* vert1, CVertex* vert2) {
		for (MeshLib::VertexEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> iter(vert1); !iter.end(); ++iter) {
			CEdge* pE = *iter;
			for (int i = 0; i < 2; i++) {
				CVertex* desti = pE->halfedge(i)->target();
				if (desti == vert2) {
					return pE;
				}
			}
		}
		return nullptr;
	}

	template<typename M>
	class CRing :public CKeyInter
	{
	public:
		CRing(unsigned char _key,CTMesh * _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CRing() {};
		virtual void keyInter(int _x, int _y)
		{
			for (M::MeshVertexIterator iter(m_pMesh); !iter.end(); ++iter) {
				CVertex* vert = *iter;
				vert->prev = nullptr;
				vert->base = nullptr;
				vert->setVisit(false);
			}
			for (M::MeshEdgeIterator iter(m_pMesh); !iter.end(); ++iter) {
				CEdge* pE = *iter;
				pE->setRGB(0, 0, 128.0 / 255.0);
				pE->vis = false;
			}
			queue<CVertex*> que;
			vector<CEdge*> edges;
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
							pE->vis = true;
							desti->prev = pV;
							desti->base = pV->base;
							if (desti->base == nullptr)desti->base = root;
							if (desti->base == root)desti->base = pV;
							que.push(desti);
							desti->setVisit(true);
						}
					}
					if (!pE->vis) {
						edges.push_back(pE);
					}
				}
			}

			random_shuffle(edges.begin(), edges.end());
			CVertex* selected = nullptr;
			for (CEdge* e : edges) {
				if(e->halfedge(0)->target()->base != e->halfedge(1)->target()->base){
					e->setRGB(1, 1, 0);
					for (int i = 0; i < 2; i++) {
						CVertex* desti = e->halfedge(i)->target();
						if (desti != root) {
							selected = desti;
							while (selected != nullptr && selected->prev != nullptr) {
								CEdge* edge = fromVV(selected, selected->prev);
								edge->setRGB(1, 1, 0);
								selected = selected->prev;
							}
						}
					}
					break;
				}
			}
			

		}
		CViewerParas* m_pParas;
		CTMesh* m_pMesh;
	};

	
}






#endif

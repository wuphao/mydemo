#ifndef _VIEWER_PREPARES_H_
#define _VIEWER_PREPARES_H_

#include"../viewerParas.h"
using namespace Viewer;
#include"../ImpOperS.h"
using namespace AlgoFrame;
#include"../attrCompute/MeshNormal.h"
using namespace TMeshLib;

namespace Viewer
{
	
	template<typename M>
	class CPreNormal : public COper
	{
	public:
		CPreNormal(typename M* _pMesh,CViewerParas* _pParas=NULL) :COper()
		{
			pNormal = new CMeshNormal<M>(_pMesh);
			m_pParas = _pParas;
		}
		~CPreNormal() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->computeNormal == true)
			{
				pNormal->_face_normal();
				//pNormal->_vertex_face_normal();
				m_pParas->computeNormal = false;
			}
			if (m_pParas == NULL)
			{
				pNormal->_face_normal();
				//pNormal->_vertex_face_normal();
			}

		}
	protected:
		CMeshNormal<M>* pNormal;
		CViewerParas* m_pParas;
	};
	
	template<typename M>
	class CPreBoundary : public COper
	{
	public:
		CPreBoundary(typename M* _pMesh,CViewerParas* _pParas=NULL) :COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		}
		~CPreBoundary() {};
	public:
		virtual void impOper()
		{
			for (M::MeshTetIterator titer(m_pMesh); !titer.end(); titer++) {
				M::CTet* pT = *titer;
				for (M::TetHalfFaceIterator fiter(m_pMesh, pT); !fiter.end(); fiter++) {
					M::CHalfFace* pHF = *fiter;
					
					pHF->face()->boundary() = !pHF->face()->boundary();
				}
			}

			for (M::MeshTetIterator titer(m_pMesh); !titer.end(); titer++) {
				M::CTet* pT = *titer;
				for (M::TetHalfFaceIterator fiter(m_pMesh, pT); !fiter.end(); fiter++) {
					M::CHalfFace* pHF = *fiter;

					for (M::HalfFaceHalfEdgeIterator eiter(m_pMesh, pHF); !eiter.end(); eiter++) {
						M::CHalfEdge* pHE = *eiter;
						pHE->tedge()->edge()->boundary() |= pHF->face()->boundary();
					}
				}
			}

		}
	protected:
		M* m_pMesh;
		CViewerParas* m_pParas;
	};
}






#endif

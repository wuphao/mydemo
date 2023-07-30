#ifndef _VIEWER_DRAW_H_
#define _VIEWER_DRAW_H_

#include<vector>
#include"../freeglut/glut.h"
#include"../freeglut/freeglut.h"
#include"ImpOperS.h"
#include"viewerParas.h"
#include"ColorSet.h"
#include"../../TMeshLib/Mesh/titerators.h"

using namespace std;

using namespace Viewer;
using namespace ViewerColor;
using namespace TMeshLib;

namespace Viewer
{

	template<typename M>
	class CDrawColoredEdges : public COper
	{
	public:
		CDrawColoredEdges(typename M* _pMesh, CViewerParas* _pParas = NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawColoredEdges() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL &&
				m_pParas->mShowEdge == false
				)
				return;

			glDisable(GL_LIGHTING);
			glLineWidth(0.6f);
			glBegin(GL_LINES);
			for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); ++eiter)
			{
				M::CEdge* pE = *eiter;

				if (pE->boundary()) {
					glColor3f(pE->getColor(0), pE->getColor(1), pE->getColor(2));
					glVertex3f(pE->vertex1()->position()[0],
						pE->vertex1()->position()[1],
						pE->vertex1()->position()[2]);
					glVertex3f(pE->vertex2()->position()[0],
						pE->vertex2()->position()[1],
						pE->vertex2()->position()[2]);
				}
			}
			glEnd();
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};

	template<typename M>
	class CDrawEdges : public COper
	{
	public:
		CDrawEdges(typename M* _pMesh,CViewerParas* _pParas=NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawEdges() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && 
				m_pParas->mShowEdge == false
				)
				return;
			
			glDisable(GL_LIGHTING);
			glLineWidth(0.6f);
			glColor3f(0.0f / 255.0f, 0.0f / 255.0f, 128.0f / 255.0f);
			glBegin(GL_LINES);
			for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); ++eiter)
			{
				M::CEdge* pE = *eiter;
				if (pE->boundary()) {
					glVertex3f(pE->vertex1()->position()[0],
						pE->vertex1()->position()[1],
						pE->vertex1()->position()[2]);
					glVertex3f(pE->vertex2()->position()[0],
						pE->vertex2()->position()[1],
						pE->vertex2()->position()[2]);
				}
			}
			glEnd();
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};

	template<typename M>
	class CDrawFaces : public COper
	{
	public:
		CDrawFaces(typename M* _pMesh,CViewerParas* _pParas=NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawFaces() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->mShowFace == false)
				return;

			glEnable(GL_POLYGON_OFFSET_FILL);
			glEnable(GL_DEPTH_TEST);
			glPolygonOffset(2.0f, 2.0f);
			glEnable(GL_LIGHTING);
			//glColor3f(135.0 / 255.0f, 206.0 / 255.0f, 250.0 / 255.0f); 
			glColor3f(229.0 / 255.0, 162.0 / 255.0, 141.0 / 255.0);
			for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
			{
				M::CFace* pF = *fiter;
				if (pF->highlight) {
					glDisable(GL_CULL_FACE);
					glBegin(GL_POLYGON);
					TMeshLib::CHalfFace* pHF = pF->left();

					glNormal3d(pHF->normal()[0], pHF->normal()[1], pHF->normal()[2]);
					for (M::HalfFaceVertexIterator fviter(m_pMesh, pHF); !fviter.end(); ++fviter)
					{
						//glNormal3d((*fviter)->normal()[0], (*fviter)->normal()[1], (*fviter)->normal()[2]);
						glVertex3d((*fviter)->position()[0], (*fviter)->position()[1], (*fviter)->position()[2]);
					}
					glEnd();
				}else if ((pF->boundary() && (pF->getVisible() || !m_pParas->cutPlane))) {
					glBegin(GL_POLYGON);
					TMeshLib::CHalfFace* pHF = pF->left();

					glNormal3d(pHF->normal()[0], pHF->normal()[1], pHF->normal()[2]);
					for (M::HalfFaceVertexIterator fviter(m_pMesh, pHF); !fviter.end(); ++fviter)
					{
						//glNormal3d((*fviter)->normal()[0], (*fviter)->normal()[1], (*fviter)->normal()[2]);
						glVertex3d((*fviter)->position()[0], (*fviter)->position()[1], (*fviter)->position()[2]);
					}
					glEnd();
				}

			}
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};

	template<typename M>
	class CDrawTet : public COper
	{
	public:
		CDrawTet(typename M* _pMesh, CViewerParas* _pParas = NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawTet() {};
	public:
		virtual void impOper()
		{
			if (m_pParas != NULL && m_pParas->mShowFace == false)
				return;

			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(2.0f, 2.0f);
			glEnable(GL_LIGHTING);
			//glColor3f(135.0 / 255.0f, 206.0 / 255.0f, 250.0 / 255.0f); 
			glColor3f(229.0 / 255.0, 162.0 / 255.0, 141.0 / 255.0);
			for (M::MeshTetIterator fiter(m_pMesh); !fiter.end(); ++fiter)
			{
				M::CTet* pT = *fiter;
				if (pT->getVisible() || !m_pParas->cutPlane) {
					glBegin(GL_POLYGON);
					for (M::TetHalfFaceIterator fviter(m_pMesh, pT); !fviter.end(); ++fviter)
					{
						TMeshLib::CHalfFace* pHF = *fviter;

						glNormal3d(pHF->normal()[0], pHF->normal()[1], pHF->normal()[2]);
						for (M::HalfFaceVertexIterator iter(m_pMesh, pHF); !iter.end(); ++iter) {

							glVertex3d((*iter)->position()[0], (*iter)->position()[1], (*iter)->position()[2]);
						}
					}
					glEnd();
				}
			}
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};

}






#endif

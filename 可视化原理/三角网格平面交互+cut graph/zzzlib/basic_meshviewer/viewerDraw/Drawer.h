#ifndef _DRAWER_H_
#define _DRAWER_H_

#include<vector>
using namespace std;

#include"../freeglut/glut.h"
#include"../freeglut/freeglut.h"
#include"ImpOperS.h"
#include"viewerParas.h"
using namespace Viewer;
#include"ColorSet.h"
using namespace ViewerColor;

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

				glColor3f(pE->getColor(0), pE->getColor(1), pE->getColor(2));
				glVertex3f(pE->halfedge(0)->source()->point()[0],
					pE->halfedge(0)->source()->point()[1],
					pE->halfedge(0)->source()->point()[2]);
				glVertex3f(pE->halfedge(0)->target()->point()[0],
					pE->halfedge(0)->target()->point()[1],
					pE->halfedge(0)->target()->point()[2]);
			}
			glEnd();
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};

	template<typename M>
	class CDrawVisibleFaces : public COper
	{
	public:
		CDrawVisibleFaces(typename M* _pMesh,CViewerParas* _pParas=NULL) : COper()
		{
			m_pMesh = _pMesh;
			m_pParas = _pParas;
		};
		~CDrawVisibleFaces() {};
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
			for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); ++fiter)
			{
				glBegin(GL_POLYGON);
				M::CFace* pF = *fiter;
				if (pF->getVisible() || !m_pParas->cutPlane) {
					glNormal3d(pF->normal()[0], pF->normal()[1], pF->normal()[2]);
					for (M::FaceVertexIterator fviter(pF); !fviter.end(); ++fviter)
					{
						glVertex3d((*fviter)->point()[0], (*fviter)->point()[1], (*fviter)->point()[2]);
					}
				}
				glEnd();
			}
		}
	protected:
		typename M* m_pMesh;
		CViewerParas* m_pParas;
	};	

}






#endif

#ifndef _Mesh_Normal_H_
#define _Mesh_Normal_H_

#include<vector>
#include<math.h>


namespace TMeshLib
{
	using namespace std;

	template<typename M>
	class CMeshNormal
	{
	public:
		CMeshNormal(M* pMesh);
		~CMeshNormal() {};

		/* compute the face normal */
		void _face_normal(bool _normalize = false);
		/* compute vertex normal from adjcient faces, should have face normal first */
		void _vertex_face_normal(bool _normalize = false);
		/* compute face normal from adjcient vertices, should have vertex normal first*/
		void _face_vertex_normal(bool _normalize = false);

	protected:
		typename M* m_pMesh;
	};

	template<typename M>
	CMeshNormal<M>::CMeshNormal(M* pMesh)
	{
		m_pMesh = pMesh;
	}

	template<typename M>
	void CMeshNormal<M>::_face_normal(bool _normalize)
	{
		CPoint p0;
		CPoint p1;
		CPoint p2;
		for (M::MeshTetIterator mt(m_pMesh); !mt.end(); mt++)
		{
			M::CTet* pT = *mt;

			for (M::TetHalfFaceIterator hf(m_pMesh, pT); !hf.end(); hf++) {

				M::CHalfFace* pHF = *hf;

				p0 = pHF->half_edge()->source()->position();
				p1 = pHF->half_edge()->target()->position();
				p2 = pHF->half_edge()->prev()->source()->position();
				
				pHF->normal() = (p1 - p0) ^ (p2 - p0);
				if (_normalize)
				{
					pHF->normal() /= pHF->normal().norm();
				}
			}
		}
	}

	//template<typename M>
	//void CMeshNormal<M>::_vertex_face_normal(bool _normalize)
	//{
	//	/* vertices normal */
	//	for (M::MeshVertexIterator mv(m_pMesh); !mv.end(); mv++)
	//	{
	//		M::CVertex* pVertex = mv.value();
	//		pVertex->normal() = CPoint(0, 0, 0);
	//		for (M::VertexFaceIterator vf(pVertex); !vf.end(); vf++)
	//		{
	//			M::CFace* pFace = static_cast<M::CFace*>(vf.value());
	//			pVertex->normal() += pFace->normal();
	//			if (_normalize)
	//			{
	//				pVertex->normal() /= pVertex->normal().norm();
	//			}
	//		}
	//	}
	//}

	template<typename M>
	void CMeshNormal<M>::_face_vertex_normal(bool _normalize)
	{
		/* face normal */
		for (M::MeshFaceIterator mf(m_pMesh); !mf.end(); mf++)
		{
			M::CFace* pFace = mf.value();
			pFace->normal() = CPoint(0, 0, 0);
			for (M::FaceVertexIterator fv(pFace); !fv.end(); fv++)
			{
				M::CVertex* pVertex = static_cast<M::CVertex*>(fv.value());
				pFace->normal() += pVertex->normal();
			}
			if (_normalize)
			{
				pFace->normal() /= pFace->normal().norm();
			}
		}
	}

}
#endif
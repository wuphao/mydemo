#ifndef _HL_H_
#define _HL_H_

#include<vector>
using namespace std;

#include"viewerKeyInter.h"
#include"viewerParas.h"
#include<queue>
#include<set>
#include<stack>
#include<algorithm>

namespace Viewer
{

    template <class T>
    class Cycle
    {
    public:
        /*�ú����������е�Ԫ�أ��ҵ�������� id ֵ��Ԫ�أ������ظ�Ԫ�ص�ָ�롣*/
        
        T* head()
        {
            T* phead = NULL;
            int max_id = -1;
            vector<T*>::iterator iter;

            for (vector<T*>::iterator viter = m_cycle.begin(); viter != m_cycle.end(); viter++)
            {
                T* pv = *viter;
                if (pv->id() > max_id)
                {
                    phead = pv;
                    max_id = pv->id();
                }
            }

            return phead;
        }
       /* ���Ԫ�� pw �Ѿ������ڻ��У�����ɾ�������Ԫ�� pw �������ڻ��У�������ӵ�����ĩβ��*/
        void add(T* pw)
        {
            auto viter = find(m_cycle.begin(), m_cycle.end(), pw);
            if (viter != m_cycle.end())
                m_cycle.erase(viter);
            else
                m_cycle.push_back(pw);
        };

        bool empty() { return m_cycle.empty(); };

    protected:
        vector<T*> m_cycle;
    };

	template<typename M>
	class CHL :public CKeyInter
	{
	public:
		CHL(unsigned char _key, CTTMesh* _pMesh, CViewerParas* _pParas) :CKeyInter(_key)
		{
			m_pParas = _pParas;
			m_pMesh = _pMesh;
		}
		~CHL() {}

        set<CEdge*>   m_generators;

        virtual void keyInter(int _x, int _y)
        {

            for (M::MeshVertexIterator viter(m_pMesh); !viter.end(); viter++)
            {
                M::CVertex* pV = *viter;
                pV->pair() = NULL;
            }

            int eid = 0;

            for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); eiter++)
            {
                M::CEdge* pE = *eiter;
                pE->pair() = NULL;
                if (pE->boundary())pE->id() = eid++;

            }

            for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); eiter++)
            {
                M::CEdge* pE = *eiter;
                if (!pE->boundary())pE->id() = eid++;
            }

            boundary_surface_pair();
            interior_volume_pair();
        }

        void markHandleLoop(CFace* pF)
        {
            pF->highlight = true;
            Cycle<CEdge> ecycle;

            set<CEdge*> fe;
            for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, (M::CHalfFace*)pF->left()); !feiter.end(); ++feiter) {
                M::CHalfEdge* pE = *feiter;
                fe.insert(pE->tedge()->edge());
            }

            for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, (M::CHalfFace*)pF->right()); !feiter.end(); ++feiter) {
                M::CHalfEdge* pE = *feiter;
                fe.insert(pE->tedge()->edge());
            }

            for (CEdge* pE : fe) {
                ecycle.add(pE);
            }
            CEdge* e = NULL;
            e = ecycle.head(); 

            while (e != NULL && !e->generator()) { 
                ecycle.add(e);
                e = ecycle.head();
            }

            while (e != NULL && e->pair() != NULL && !ecycle.empty()) {
                CFace* f = e->pair();
                f->highlight = true;
                fe.clear();
                for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, f->left()); !feiter.end(); ++feiter) {
                    M::CHalfEdge* pE = *feiter;
                    fe.insert(pE->tedge()->edge());
                }

                for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, f->right()); !feiter.end(); ++feiter) {
                    M::CHalfEdge* pE = *feiter;
                    fe.insert(pE->tedge()->edge());
                }

                for (CEdge* pE : fe) {
                    ecycle.add(pE);
                }

                e = ecycle.head();
                while (e != NULL && (!e->generator() || e->boundary())) {
                    if (e->boundary()) {
                        e->setRGB(1,0,0);
                    }
                    ecycle.add(e);
                    e = ecycle.head();
                }
            }
        }

      
        void interior_volume_pair()
        {
            pairE(false);
            std::cout << "finished inner edges pair " << std::endl;
            pairF(false);
            std::cout << "finished inner faces pair " << std::endl;

            for (auto eiter = m_generators.begin(); eiter != m_generators.end(); eiter++)
            {
                CEdge* pE = *eiter;
                if (!pE->generator() || pE->pair() != NULL)
                {
                    CFace* pF = pE->pair();
                    markHandleLoop(pF);
                }
            }
        }

  
        void boundary_surface_pair()
        {
            pairE(true);
            std::cout << "finished boundary edges pair " << std::endl;
            pairF(true);
            std::cout << "finished boundary faces pair " << std::endl;

            for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); eiter++)
            {
                M::CEdge* pE = *eiter;
                if (pE->boundary() && pE->generator() && pE->pair() == NULL)
                {
                    m_generators.insert(pE);
                }
            }
        }

        /*���ڱ߽�ߣ�isboundary Ϊ true��
        ����ߵ�һ�������Ѿ�����һ������ԣ��򽫸ñߵ���һ�������뵱ǰ����ԡ�
        ����ߵ��������㶼δ����������ԣ��򽫵�ǰ�߱��Ϊ���ɱߣ������ߵ� generator() ��������Ϊ true��
        */
        /*
         ���ڷǱ߽�ߣ�isboundary Ϊ false����
         ����ߵ�һ�������Ѿ�����һ������ԣ��򽫸ñߵ���һ�������뵱ǰ����ԡ�
         ����ߵ��������㶼δ����������ԣ��򽫵�ǰ�߱��Ϊ���ɱߣ������ߵ� generator() ��������Ϊ true��
        */
        void pairE(bool isboundary)
        {

            for (M::MeshEdgeIterator eiter(m_pMesh); !eiter.end(); eiter++)
            {
                M::CEdge* pE = *eiter;
                if (pE->boundary() == isboundary) {

                    Cycle<CVertex> vcycle;
                    CVertex* v = NULL;
                    CVertex* v1 = pE->vertex1(); 
                    CVertex* v2 = pE->vertex2();   

                    vcycle.add(v1);
                    vcycle.add(v2);

                    v = vcycle.head();

                    while (v != NULL && v->pair() != NULL && !vcycle.empty()) {
                        CEdge* e = v->pair();
                        CVertex* v1 = pE->vertex1();
                        CVertex* v2 = pE->vertex2();

                        vcycle.add(v1);
                        vcycle.add(v2);

                        v = vcycle.head();
                    }

                    if (!vcycle.empty()) {
                        v->pair() = pE;
                    }
                    else {
                        pE->generator() = true;
                    }
                }
            }
        }

       /*
       * �ú������ڶ��������ԡ������������е��棬��ÿ������һ������ԣ��γ�һ������
       ��ԵĹ����ǣ�������һ�����Ѿ���ԣ��򽫸�������һ������ԣ�
       �����������߶�δ��ԣ��򽫸����һ���߱��Ϊ���ɱߡ���Ե��潫�γ�һ������
       */
        void pairF(bool isboundary)
        {
            for (M::MeshFaceIterator fiter(m_pMesh); !fiter.end(); fiter++)
            {
                M::CFace* pF = *fiter;
                if (pF->boundary() == isboundary) {
                    Cycle<CEdge> ecycle;

                    set<CEdge*> fe;
                    for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, pF->left()); !feiter.end(); ++feiter) {
                        M::CHalfEdge* pE = *feiter;
                        fe.insert(pE->tedge()->edge());
                    }
                    if (pF->right() != nullptr)
                        for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, pF->right()); !feiter.end(); ++feiter) {
                            M::CHalfEdge* pE = *feiter;
                            fe.insert(pE->tedge()->edge());
                        }

                    for (CEdge* pE : fe) {
                        ecycle.add(pE);
                    }

                    CEdge* e = NULL;
                    e = ecycle.head(); 
                    while (e != NULL && !e->generator()) {
                        ecycle.add(e);
                        e = ecycle.head();
                    }
                    while (e != NULL && e->pair() != NULL && !ecycle.empty()) {
                        CFace* f = e->pair();
                        fe.clear();
                        for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, f->left()); !feiter.end(); ++feiter) {
                            M::CHalfEdge* pE = *feiter;
                            fe.insert(pE->tedge()->edge());
                        }

                        if (f->right() != nullptr)
                            for (M::HalfFaceHalfEdgeIterator feiter(m_pMesh, f->right()); !feiter.end(); ++feiter) {
                                M::CHalfEdge* pE = *feiter;
                                fe.insert(pE->tedge()->edge());
                            }

                        for (CEdge* pE : fe) {
                            ecycle.add(pE);
                        }

                        e = ecycle.head(); 
                        while (e != NULL && !e->generator()) { 
                            ecycle.add(e);
                            e = ecycle.head();
                        }
                    }

                    if (!ecycle.empty()) {
                        e->pair() = pF;
                    }
                }
            }
        }


		CViewerParas* m_pParas;
		CTTMesh* m_pMesh;

	};

	
}

#endif

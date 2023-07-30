#include<iostream>
using namespace std;
/* Mesh */
#include"ToolMesh.h"
using namespace MeshLib;
/* viewer */
#include"viewerCore/viewer.h"
#include"viewerDraw/viewerDraw.h"
#include"preprocess/viewerPrepares.h"
#include"keyInter/Resize_multi_08.h"
#include"keyInter/Resize_divid_08.h"
#include"keyInter/ShowEdgeInter.h"
#include"keyInter/SetVisible.h"
#include"keyInter/BFS.h"
#include"keyInter/Ring.h"
#include"keyInter/MovePlane.h"
#include"viewerDraw/Drawer.h"
using namespace Viewer;
/* main  */
int main(int argc, char * argv[])
{
	/* �����ļ� */
	CTMesh mesh;
	mesh.read_m(argv[1]);
	/* Ԥ���� */
	PreOpers.addOper(new CPreNormal<CTMesh>(&mesh));
	/* ���ӻ� */
	DrawOpers.addOper(new CDrawColoredEdges<CTMesh>(&mesh,&mViewerParas));
	DrawOpers.addOper(new CDrawVisibleFaces<CTMesh>(&mesh,&mViewerParas));
	DrawOpers.addOper(new CDrawVertices<CTMesh>(&mesh,&mViewerParas));
	/* ���̽��� */
	KeyBdInter.addKeyInter(new CKeyResizeMulti08<CTMesh>('*',&mesh));
	KeyBdInter.addKeyInter(new CKeyResizeDivid08<CTMesh>('/',&mesh));
	KeyBdInter.addKeyInter(new CShowEdgeInter<CTMesh>('e',&mViewerParas));
	KeyBdInter.addKeyInter(new CSetVisible<CTMesh>('v',&mesh,&mViewerParas));
	KeyBdInter.addKeyInter(new CMovePlane<CTMesh>('-', &mesh, &mViewerParas,-0.05));
	KeyBdInter.addKeyInter(new CMovePlane<CTMesh>('+', &mesh, &mViewerParas,0.05));
	KeyBdInter.addKeyInter(new CBFS<CTMesh>('b', &mesh, &mViewerParas));
	KeyBdInter.addKeyInter(new CRing<CTMesh>('r', &mesh, &mViewerParas));

	/* OpenGL */
	viewer(argc, argv, NULL);
}
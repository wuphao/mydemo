#include<iostream>
using namespace std;
/* Mesh */
#include"TetTestMesh.h"
using namespace TMeshLib;
/* viewer */
#include"viewerCore/viewer.h"
#include"viewerDraw/viewerDraw.h"
#include"preprocess/viewerPrepares.h"
#include"keyInter/Resize_multi_08.h"
#include"keyInter/Resize_divid_08.h"
#include"keyInter/ShowEdgeInter.h"
#include"keyInter/MovePlane.h"
#include"keyInter/SetVisible.h"
#include"keyInter/BFS.h"
#include"keyInter/Ring.h"
#include"keyInter/HL.h"
using namespace Viewer;

/* main  */
int main(int argc, char * argv[])
{
	/* 读入文件 */
	CTTMesh mesh;
	mesh._load_t(argv[1]);
	/* 预处理 */
	PreOpers.addOper(new CPreNormal<CTTMesh>(&mesh));
	/* 可视化 */
	DrawOpers.addOper(new CDrawEdges<CTTMesh>(&mesh,&mViewerParas));
	DrawOpers.addOper(new CDrawFaces<CTTMesh>(&mesh,&mViewerParas));
	/* 键盘交互 */
	KeyBdInter.addKeyInter(new CKeyResizeMulti08<CTTMesh>('*',&mesh));
	KeyBdInter.addKeyInter(new CKeyResizeDivid08<CTTMesh>('/',&mesh));
	KeyBdInter.addKeyInter(new CShowEdgeInter<CTTMesh>('e',&mViewerParas));
	KeyBdInter.addKeyInter(new CSetVisible<CTTMesh>('v', &mesh, &mViewerParas));
	KeyBdInter.addKeyInter(new CMovePlane<CTTMesh>('+', &mesh, &mViewerParas, -0.05));
	KeyBdInter.addKeyInter(new CMovePlane<CTTMesh>('-', &mesh, &mViewerParas, 0.05));
	KeyBdInter.addKeyInter(new CBFS<CTTMesh>('b', &mesh, &mViewerParas));
	KeyBdInter.addKeyInter(new CRing<CTTMesh>('r', &mesh, &mViewerParas));
	KeyBdInter.addKeyInter(new CHL<CTTMesh>('h', &mesh, &mViewerParas));
	/* OpenGL */
	viewer(argc, argv, NULL);
}
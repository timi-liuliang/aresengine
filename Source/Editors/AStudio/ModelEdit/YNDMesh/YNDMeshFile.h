#pragma once
#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <map>
#include <Core/3rdParty/LZO/minilzo.h>

using namespace std;

//初始化字符数组
#define STRING_INIT(chararray)	  chararray[0] = '\0';
//取得数组大小
#define ARRAY_SIZE(chararray)	  (sizeof(chararray)/sizeof(chararray[0]))
//字符串安全拷贝
//参1目标串
//参2拷贝串
#define STRING_COPY_ALL( chararray ,srcstr )	strcpy_s(chararray,ARRAY_SIZE(chararray),srcstr);

#define FILE_HEADER_NAME_MAX_SIZE	50
#define FILE_HEADER_AUTHOR_MAX_SIZE	60
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

//子模型渲染状态m_dwRenderFlag低位
#define RENDERFLAG_LOW_NOALPHA					0				//普通无ALPHA
#define RENDERFLAG_LOW_ALPHABLEND				1				//使用ALPHA混合
#define RENDERFLAG_LOW_ALPHATEST				2				//使用ALPHA测试

//子模型渲染状态m_dwRenderFlag高位
#define RENDERFLAG_HIGH_REFLECT					1				//反射
#define RENDERFLAG_HIGH_DROPNORMAL				2				//丢弃法线计算
#define RENDERFLAG_HIGH_FOGINVALID				4				//不受雾影响
#define RENDERFLAG_HIGH_CULLMODE_NONE			8				//单面渲染
#define RENDERFLAG_HIGH_COMBOBATCH				16				//组合打包渲染
#define RENDERFLAG_HIGH_NOUSETEXLOD				32				//不便用纹理LOD

//导出游戏资源类型
#define GAMERES_MESH_S						0				//静态模型
#define GAMERES_MESH_D						1				//带动作模型
#define GAMERES_MATERIAL					2				//材质
#define GAMERES_SKINANI						3				//动画
#define GAMERES_UVANI						4				//UV动画
#define GAMERES_BOUNDINGBOX					5				//绑定盒


//模型标记类型
#define MESHFLAG_NORMAL						0				//普通
#define MESHFLAG_USEBOUNDINGBOXMESH			1				//使用绑定盒模型
#define MESHFLAG_USEBUMPVERTEX				2				//使用凹凸贴图顶点格式
#define MESHFLAG_USEALPHABLEND				4				//是否使用ALPHA渲染
#define MESHFLAG_BOUNDINGMESHPICKTYPE		8				//按照绑定模型方式进行捡选
#define	MESHFLAG_SWING						16				//是否摇摆
#define MESHFLAG_ALPHACOLOR					32				//色彩值代表ALPHA，色彩为白
#define MESHFLAG_NOCHECKDISTANCEVISIBLE		64				//距离不影响显示
#define MESHFLAG_WATER						128				//属于水模型
#define MESHFLAG_LIGHTMAPUV					256				//使用LightMap UV


//模型类型
#define MESHTYPE_SHAPE						0				//线条图形
#define MESHTYPE_PATCHOBJ					1				//PATCH BOJ							
#define MESHTYPE_TRIOBJ						2				//三角模型
#define MESHTYPE_BONE						3				//骨骼模型

struct	SFileHeader
{
	BYTE	Ver;										// 版本
	char	Name[FILE_HEADER_NAME_MAX_SIZE];			// 名称
	char	Author[FILE_HEADER_AUTHOR_MAX_SIZE];		// 作者IP
	DWORD	Type;										//类型	[低8位是类型，高24位用于标记]
	int		NodeNum;									//节点数量

	SFileHeader()
	{
		STRING_INIT(Name);
		STRING_INIT(Author);
	}

};
//优化导出的顶点
struct  SExport_Vertex
{
	float	m_fPosX;					//X，Y，Z	
	float	m_fPosY;
	float	m_fPosZ;

	float   m_fNPosX;					//Normal
	float	m_fNPosY;
	float   m_fNPosZ;

	float   m_fRed;					//顶点色
	float	m_fGreen;
	float   m_fBlue;

	float   m_fU;						//纹理
	float   m_fV;		

public:
	//重载操作符
	bool   operator == (SExport_Vertex   &v)
	{
		if(m_fPosX != v.m_fPosX)return false;
		if(m_fPosY != v.m_fPosY)return false;
		if(m_fPosZ != v.m_fPosZ)return false;
		if(m_fNPosX != v.m_fNPosX)return false;
		if(m_fNPosY != v.m_fNPosY)return false;
		if(m_fNPosZ != v.m_fNPosZ)return false;
		if(m_fRed != v.m_fRed)return false;
		if(m_fGreen != v.m_fGreen)return false;
		if(m_fBlue != v.m_fBlue)return false;
		if(m_fU != v.m_fU)return false;
		if(m_fV != v.m_fV)return false;
		return true;
	}
}
;

//优化导出的面
struct   SExport_Face
{
	int		m_nVertexIndex1;			//顶点索引
	int		m_nVertexIndex2;
	int		m_nVertexIndex3;
}
;


//顶点受骨骼权重影响信息
struct	SVertex_BoneWeight
{
	int		m_nVertexIndex;			//顶点索引
	int		m_nBoneIndex;			//骨骼索引
	float   m_fWeight;				//影响权重
}
;

//子模型
struct	SSubMesh
{
	char	m_szSubMeshName[60];		//子模型名称(子材质分解的模型名称)
	char	m_szOldMeshName[60];		//原模型名称(子材质分解前的模型名称)
	DWORD	m_dwMeshType;				//模型类型
	int		m_nMaterialID;			//材质名称
	int		m_nRed;					//色彩
	int		m_nGreen;
	int		m_nBlue;
	int		m_nAlpha;
	int		m_nVertexNum;				//顶点数
	int		m_nEdgeNum;				//边数
	int		m_nFaceNum;				//面数
}
;
//模型矩阵
struct SMeshMatrix
{
	FLOAT	 m[16];
}
;

struct	SSubMesh_Temp
{
	SSubMesh						m_sSubMeshInfo;				//子模型信息
	D3DXMATRIX						m_matSubMeshMatrix;			//子模型的矩阵
	vector<SExport_Vertex>			m_pVertexArray;				//顶点信息
	vector<SExport_Face>			m_pFaceArray;				//面信息
	int								m_nBoneInfoIndex;			//骨骼信息索引
	int								m_nVertexBoneWeightNum;		//权重数量
	vector<SVertex_BoneWeight>		m_pVertexBoneWeightArray;	//顶点受骨骼权重影响
	vector<D3DXVECTOR2>				m_pLightMapUVArray;			//LightMap的UV信息数组
	DWORD							m_dwRenderFlag;				//渲染标志
	int								m_nSwingSpeed;				//摇摆速度
	int								m_nSwingRegion;				//摇摆幅度
	float							m_fNormalLevel;				//法线强弱值
}
;
struct SMeshNode_Temp
{
	//数量
	int								m_nSubMeshNum;
	//D3D模型体
	vector<SSubMesh_Temp>			m_pSubMeshArray; 
}
;

struct	STexture
{
	int					m_nIndex;						//对应的通道索引
	char				m_szFileName[60];				//文件名
}
;
//材质结点
struct	SMatNode_New
{
	DWORD	m_dwMatType;											//材质类型	(0 :普通  1:多重材质)
	DWORD	m_dwGHBump;											//贴图或材质数量
	int		m_nNum;												//贴图或材质数量
	int		m_nUVAniNum;											//UV动画数量	
}
;
//材质
struct	SMatNode_Temp
{
	SMatNode_New			m_sMatInfo;
	float					m_fShininess ;							//光泽系数
	float					m_fShinStr ;							//高光系数
	int						m_nMaterialNum;
	vector<int>				m_pMaterialArray;
	int						m_nTextureNum;
	vector<STexture>		m_pTextureArray;
}
;
//凹凸贴图信息
struct	SBumpInfo
{
	//设置凹凸贴图参数
	D3DXVECTOR4					m_vec4BumpACColor;
	D3DXVECTOR4					m_vec4BumpSCColor;
	FLOAT						m_fBumpSPValue;
	FLOAT						m_fBumpSLValue;
	D3DXVECTOR4					m_vec4BumpECColor;
	D3DXVECTOR4					m_vec4BumpSkCColor;
	D3DXVECTOR4					m_vec4BumpSkACColor;
}
;


class YNDMeshFile
{

public:
	//构造
	YNDMeshFile();
	//析构
	~YNDMeshFile();

	//清空
	VOID						CleanUp();
	//从文件中读取数据
	BOOL						ReadDataFromFile(const char* szFileName);
	//取得模型体数量
	int							Fun_GetMeshNodeNum();
	//取得对应模型体
	SMeshNode_Temp*				Fun_GetMeshNodePtr(int vIndex);

	//取得材质数量
	int							Fun_GetTextureNum();
	//取得对应的材质
	STexture*					Fun_GetTexturePtr( int vIndex);
	//取得材质的贴图名称
	const char*					Fun_GetTextureName( int	vIndex);
	//取得标记
	BOOL						CheckFlag(DWORD vHeaderType,WORD	vFlag);
private:

	//头信息
	SFileHeader					m_sFileHeader;
	//动作模型导出选项
	int							m_nAniMeshType;
	//模型数量
	int							m_nMeshNum;
	//模型体
	vector<SMeshNode_Temp>		m_pMeshNodeArray;	

	//总模型数量
	int							m_nAllSubMeshNum;

	//贴图数
	int							m_nTextureNum;
	vector<STexture>			m_pTextureVec;

	//加入这两个数组是用于计算碰撞用，当然，也可以供其它计算使用。
	//顶点的数量
	int							m_nAllBoundingMeshVertexSize;
	//顶点
	vector<D3DXVECTOR3>			m_pAllBoundingMeshVertexArray;

	//面的数量
	int							m_nAllBoundingMeshFaceSize;
	//面信息
	vector<SExport_Face>		m_pAllBoundingMeshFaceArray;

	//是否有骨骼动画
	bool						m_bHasSkinAni;

	//是否是帧动画
	bool						m_bHasFrameAni;

	//是否是多子模型的动作模型文件
	bool						m_bMultiSubMesh;

	//是否存在绑定盒模型
	bool						m_bBoundingBoxMesh;

	//是否加载了动作帧子模型包围盒
	bool						m_bLoadAniFrameBoundingBoxOK;

	//是否使用渲染标记
	bool						m_bUserRenderFlag;

	//是否是凹凸贴图顶点格式
	bool						m_bUseBumpRender;

	//是否摇摆
	bool						m_bUseSwingRender;

	//alpha色彩
	bool						m_bAlphaColorRender;

	//距离影响显示
	bool						m_bDistanceVisible;

	//是否使用绑定模型进行捡选
	bool						m_bBoundingMeshPickType;

	//是否使用LIGHTMAP UV
	bool						m_bUseLightMapUV;

	//是否是水模型
	bool						m_bIsWaterMesh;

	//是否在创建完成后删除绑定碰撞体
	bool						m_bAutoDeleteBoundingVertexAndFace;
	//指定的骨骼文件
	char						m_szActionFileName[100];
};
#include "YNDMeshFile.h"
//检查标记
inline  BOOL  CheckFlag_WORD(WORD vFlag,WORD vCheckValue)
{
	return (vCheckValue == (vFlag&vCheckValue));
}

//构造
YNDMeshFile::YNDMeshFile()
{
	m_nMeshNum = 0;
	m_nAllSubMeshNum = 0;
	m_nTextureNum = 0;
	m_nAllBoundingMeshVertexSize = 0;
	m_nAllBoundingMeshFaceSize = 0;
	m_bHasSkinAni = false;
}
//析构
YNDMeshFile::~YNDMeshFile()
{

}
//清空
VOID	YNDMeshFile::CleanUp()
{
	m_nMeshNum = 0;
	m_pMeshNodeArray.clear();
	m_nAllSubMeshNum = 0;
	m_nTextureNum = 0;
	m_pTextureVec.clear();
	m_nAllBoundingMeshVertexSize = 0;
	m_pAllBoundingMeshVertexArray.clear();
	m_nAllBoundingMeshFaceSize = 0;
	m_pAllBoundingMeshFaceArray.clear();
	m_bHasSkinAni = false;

}
//从文件中读取数据
BOOL	YNDMeshFile::ReadDataFromFile(const char* szFileName)
{
	CleanUp();
	try
	{
		FILE*	hFile = fopen(szFileName,"rb");
		if(!hFile)return FALSE;
		//模型=====================================================
		//读取
		fread(&m_sFileHeader , sizeof(SFileHeader),1,hFile);
		//包围体
		m_bBoundingBoxMesh = false;
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_USEBOUNDINGBOXMESH))
		{
			m_bBoundingBoxMesh = true;
		}

		//渲染标记
		m_bUserRenderFlag = false;
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_USEALPHABLEND))
		{
			m_bUserRenderFlag = true;
		}

		//凹凸贴图
		m_bUseBumpRender = false;
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_USEBUMPVERTEX))
		{
			m_bUseBumpRender = true;
		}

		m_bUseSwingRender = false;
		//摇摆值
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_SWING))
		{
			m_bUseSwingRender = true;
		}

		m_bAlphaColorRender = false;
		m_bIsWaterMesh = false;
		//摇摆值
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_ALPHACOLOR))
		{
			m_bAlphaColorRender = true;
			m_bIsWaterMesh = true;
		}
		//水
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_WATER))
		{
			m_bAlphaColorRender = true;
			m_bIsWaterMesh = true;
		}
		//不受视距影响
		m_bDistanceVisible = true;
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_NOCHECKDISTANCEVISIBLE))
		{
			m_bDistanceVisible = false;
		}

		m_bBoundingMeshPickType = false;
		//使用绑定模型进行PICK拾选
		if( TRUE == CheckFlag(m_sFileHeader.Type ,MESHFLAG_BOUNDINGMESHPICKTYPE))
		{
			m_bBoundingMeshPickType = true;
			m_bAutoDeleteBoundingVertexAndFace = false;
		}
		m_bUseLightMapUV = false;
		//使用LightMap 的UV
		if( TRUE == CheckFlag(m_sFileHeader.Type , MESHFLAG_LIGHTMAPUV))
		{
			m_bUseLightMapUV = true;
		}


		DWORD	tType = m_sFileHeader.Type ;
		//模型类型
		WORD	tMeshType = tType &0x00ff;

		if( GAMERES_MESH_D == tMeshType )
		{
			m_bHasSkinAni = false;
			m_bHasFrameAni = false;

			//读取
			//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &m_nAniMeshType , sizeof(int));
			fread(&m_nAniMeshType , sizeof(int),1,hFile);
			if( 0 == m_nAniMeshType)
			{
				//骨骼动画_单一模型
				m_bHasSkinAni = true;
				m_bMultiSubMesh = false;
				m_bHasFrameAni = false;
				//读取
				//ReadMemoryData(pCurrData, nMaxSize , CurrSize , m_szActionFileName , sizeof(m_szActionFileName));
				fread(m_szActionFileName , sizeof(m_szActionFileName),1,hFile);
			}
			else if( 1 == m_nAniMeshType)
			{
				//骨骼动画_多模型
				m_bHasSkinAni = true;
				m_bMultiSubMesh = true;
				m_bHasFrameAni = false;
				//读取
				//ReadMemoryData(pCurrData, nMaxSize , CurrSize , m_szActionFileName , sizeof(m_szActionFileName));
				fread(m_szActionFileName , sizeof(m_szActionFileName),1,hFile);
			}
			else
			{
				//帧动画_模型
				m_bHasSkinAni = false;
				m_bMultiSubMesh = false;
				m_bHasFrameAni = true;
			}
		}
		else 
		{
			m_bHasSkinAni = false;
			m_bHasFrameAni = false;
		}

		//子模型数量
		if(m_sFileHeader.NodeNum > 0)
		{
			//===================================================
			m_nAllSubMeshNum = 0;

			for(int i = 0 ; i < m_sFileHeader.NodeNum ; i++)
			{
				SMeshNode_Temp	tMeshNode;
				int tSubMeshSize = 0;
				//读取
				//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshSize , sizeof(int));
				fread(&tSubMeshSize , sizeof(int),1,hFile);

				m_nAllSubMeshNum += tSubMeshSize ;
				tMeshNode.m_nSubMeshNum = tSubMeshSize;
				for(int j = 0 ; j < tSubMeshSize; j++)
				{	
					SSubMesh_Temp	tSubMeshInfo_Temp ;

					//读取
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_sSubMeshInfo , sizeof(SSubMesh));
					fread(&tSubMeshInfo_Temp.m_sSubMeshInfo , sizeof(SSubMesh),1,hFile);

					if(tSubMeshInfo_Temp.m_sSubMeshInfo.m_dwMeshType > MESHTYPE_BONE)
					{
						fclose(hFile);
						return FALSE;
					}

					if(true == m_bHasFrameAni)
					{
						wsprintfA(tSubMeshInfo_Temp.m_sSubMeshInfo.m_szSubMeshName,"%s_%d",tSubMeshInfo_Temp.m_sSubMeshInfo.m_szOldMeshName,i);
					}

					SMeshMatrix	tM16;
					//读取
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tM16 , sizeof(SMeshMatrix));
					fread(&tM16 , sizeof(SMeshMatrix),1,hFile);

					tSubMeshInfo_Temp.m_matSubMeshMatrix = D3DXMATRIX(tM16.m);

					if(tSubMeshInfo_Temp.m_sSubMeshInfo.m_dwMeshType == MESHTYPE_SHAPE)
					{
						//Vertex
						int		tVertexNum = tSubMeshInfo_Temp.m_sSubMeshInfo.m_nVertexNum ;
						//====================内存申请=======================
						if( tVertexNum > 0 )
						{
							if(m_sFileHeader.Ver > 1)
							{
								lzo_uint DataLen = 0;
								UINT	 DataComLen = 0;

								//读取
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataLen , sizeof(UINT));
								fread(&DataLen , sizeof(UINT),1,hFile);
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataComLen , sizeof(UINT));
								fread(&DataComLen , sizeof(UINT),1,hFile);

								if(DataLen > DataComLen)
								{

									BYTE*	tpCompressMemory = new BYTE[DataComLen];
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpCompressMemory , DataComLen);
									fread(tpCompressMemory ,DataComLen,1,hFile);

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									lzo1x_decompress(tpCompressMemory, DataComLen, tpDecCompressMemory, &DataLen, NULL);
									SExport_Vertex*	tpVertexArray =  (SExport_Vertex*)tpDecCompressMemory ;
									for(int v = 0 ; v < tVertexNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexArray.push_back(tpVertexArray[v]);
									}

									delete[] tpCompressMemory ;
									delete[] tpDecCompressMemory ;
								}
								else
								{

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpDecCompressMemory , DataLen);
									fread(tpDecCompressMemory ,DataLen,1,hFile);

									SExport_Vertex*	tpVertexArray =  (SExport_Vertex*)tpDecCompressMemory ;
									for(int v = 0 ; v < tVertexNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexArray.push_back(tpVertexArray[v]);
									}
									delete[] tpDecCompressMemory ;
								}
							}
							else
							{

								for(int v = 0 ; v < tVertexNum ; v++)
								{
									SExport_Vertex	tVertex;
									fread(&tVertex ,sizeof(SExport_Vertex),1,hFile);
									tSubMeshInfo_Temp.m_pVertexArray.push_back(tVertex);
								}
							}
						}
						//===================================================


					}	
					else if(tSubMeshInfo_Temp.m_sSubMeshInfo.m_dwMeshType == MESHTYPE_PATCHOBJ)
					{

					}
					else
					{
						//Vertex
						int		tVertexNum = tSubMeshInfo_Temp.m_sSubMeshInfo.m_nVertexNum ;
						//====================内存申请=======================
						if( tVertexNum > 0)
						{

							if(m_sFileHeader.Ver > 1)
							{
								lzo_uint DataLen = 0;
								UINT	 DataComLen = 0;
								//读取
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataLen , sizeof(UINT));
								fread(&DataLen , sizeof(UINT),1,hFile);
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataComLen , sizeof(UINT));
								fread(&DataComLen , sizeof(UINT),1,hFile);

								if(DataLen > DataComLen)
								{

									BYTE*	tpCompressMemory = new BYTE[DataComLen];
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpCompressMemory , DataComLen);
									fread(tpCompressMemory ,DataComLen,1,hFile);

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									lzo1x_decompress(tpCompressMemory, DataComLen, tpDecCompressMemory, &DataLen, NULL);

									SExport_Vertex*	tpVertexArray =  (SExport_Vertex*)tpDecCompressMemory ;
									for(int v = 0 ; v < tVertexNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexArray.push_back(tpVertexArray[v]);
									}

									delete[] tpCompressMemory ;
									delete[] tpDecCompressMemory ;
								}
								else
								{

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpDecCompressMemory , DataLen);
									SExport_Vertex*	tpVertexArray =  (SExport_Vertex*)tpDecCompressMemory ;
									for(int v = 0 ; v < tVertexNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexArray.push_back(tpVertexArray[v]);
									}
									delete[] tpDecCompressMemory ;

								}

							}
							else
							{

								for(int v = 0 ; v < tVertexNum ; v++)
								{
									SExport_Vertex	tVertex;
									fread(&tVertex ,sizeof(SExport_Vertex),1,hFile);
									tSubMeshInfo_Temp.m_pVertexArray.push_back(tVertex);
								}
							}


							//LightMap的UV通道
							if(m_bUseLightMapUV)
							{
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tSubMeshInfo_Temp.m_pLightMapUVArray , sizeof(D3DXVECTOR2)*tVertexNum);
								for(int v = 0 ;v < tVertexNum ;v++)
								{
									D3DXVECTOR2	tVertex_LightMapUV;
									fread(&tVertex_LightMapUV,sizeof(D3DXVECTOR2),tVertexNum,hFile);
									tSubMeshInfo_Temp.m_pLightMapUVArray.push_back(tVertex_LightMapUV);
								}

							}


						}
						//===================================================

						//Faces
						int		tFaceNum = tSubMeshInfo_Temp.m_sSubMeshInfo.m_nFaceNum ;

						//====================内存申请=======================
						if( tFaceNum > 0 )
						{
							//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tSubMeshInfo_Temp.m_pFaceArray , sizeof(SExport_Face)*tFaceNum);
							for(int f = 0 ;f < tFaceNum ;f++)
							{
								SExport_Face	tFace;
								fread(&tFace,sizeof(SExport_Face),1,hFile);
								tSubMeshInfo_Temp.m_pFaceArray.push_back(tFace);
							}
						}
						//===================================================



						//m_ActionIndex
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_nBoneInfoIndex , sizeof(int));
						fread(&tSubMeshInfo_Temp.m_nBoneInfoIndex ,sizeof(int),1,hFile);
						//m_nVertexBoneWeightNum
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_nVertexBoneWeightNum , sizeof(int));
						fread(&tSubMeshInfo_Temp.m_nVertexBoneWeightNum ,sizeof(int),1,hFile);

						if(tSubMeshInfo_Temp.m_nVertexBoneWeightNum > 0)
						{
							if(m_sFileHeader.Ver > 1)
							{
								lzo_uint DataLen = 0;
								UINT	 DataComLen = 0;
								//读取
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataLen , sizeof(UINT));
								fread(&DataLen , sizeof(UINT),1,hFile);
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataComLen , sizeof(UINT));
								fread(&DataComLen , sizeof(UINT),1,hFile);

								if(DataLen > DataComLen)
								{

									BYTE*	tpCompressMemory = new BYTE[DataComLen];
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpCompressMemory , DataComLen);
									fread(tpCompressMemory ,DataComLen,1,hFile);

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									lzo1x_decompress(tpCompressMemory, DataComLen, tpDecCompressMemory, &DataLen, NULL);

									SVertex_BoneWeight*	tpVertexArray =  (SVertex_BoneWeight*)tpDecCompressMemory ;
									for(int v = 0 ; v < tSubMeshInfo_Temp.m_nVertexBoneWeightNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexBoneWeightArray.push_back(tpVertexArray[v]);
									}

									delete[] tpCompressMemory ;
									delete[] tpDecCompressMemory ;
								}
								else
								{

									BYTE*	tpDecCompressMemory = new BYTE[DataLen];
									// 解压缩 
									//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpDecCompressMemory , DataLen);
									fread(tpDecCompressMemory ,DataLen,1,hFile);

									SVertex_BoneWeight*	tpVertexArray =  (SVertex_BoneWeight*)tpDecCompressMemory ;
									for(int v = 0 ; v < tSubMeshInfo_Temp.m_nVertexBoneWeightNum ; v++)
									{
										tSubMeshInfo_Temp.m_pVertexBoneWeightArray.push_back(tpVertexArray[v]);
									}
									delete[] tpDecCompressMemory ;
								}

							}
							else
							{
								for(int v = 0 ; v < tSubMeshInfo_Temp.m_nVertexBoneWeightNum ; v++)
								{
									SVertex_BoneWeight	tVertex_BoneWeight;
									fread(&tVertex_BoneWeight ,sizeof(SVertex_BoneWeight),1,hFile);
									tSubMeshInfo_Temp.m_pVertexBoneWeightArray.push_back(tVertex_BoneWeight);
								}

							}
						}

					}


					if( true == m_bUserRenderFlag )
					{
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_dwRenderFlag , sizeof(DWORD));
						fread(&tSubMeshInfo_Temp.m_dwRenderFlag ,sizeof(DWORD),1,hFile);
					}
					else
					{
						if(true == m_bIsWaterMesh || true == m_bAlphaColorRender )
						{
							tSubMeshInfo_Temp.m_dwRenderFlag = RENDERFLAG_LOW_ALPHABLEND;
						}
						else
						{
							tSubMeshInfo_Temp.m_dwRenderFlag = RENDERFLAG_LOW_NOALPHA;
						}
					}

					if( true == m_bUseSwingRender )
					{
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_nSwingSpeed , sizeof(int));
						fread(&tSubMeshInfo_Temp.m_nSwingSpeed ,sizeof(int),1,hFile);
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_nSwingRegion , sizeof(int));
						fread(&tSubMeshInfo_Temp.m_nSwingRegion ,sizeof(int),1,hFile);
					}
					else
					{
						tSubMeshInfo_Temp.m_nSwingSpeed = 1;
						tSubMeshInfo_Temp.m_nSwingRegion = 0;
					}

					//如果有法线
					if(m_sFileHeader.Ver > 3)
					{
						BOOL	bDropNormal = CheckFlag_WORD(HIWORD(tSubMeshInfo_Temp.m_dwRenderFlag), RENDERFLAG_HIGH_DROPNORMAL);
						if( FALSE == bDropNormal )
						{
							//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tSubMeshInfo_Temp.m_fNormalLevel , sizeof(float));
							fread(&tSubMeshInfo_Temp.m_fNormalLevel ,sizeof(float),1,hFile);
						}
						else
						{
							tSubMeshInfo_Temp.m_fNormalLevel = 1.0;
						}
					}
					else
					{
						if( m_bHasSkinAni )
						{
							tSubMeshInfo_Temp.m_fNormalLevel = 0.4f;
						}
						else
						{
							tSubMeshInfo_Temp.m_fNormalLevel = 1.0;
						}
					}

					tMeshNode.m_pSubMeshArray.push_back(tSubMeshInfo_Temp);
				}


				m_pMeshNodeArray.push_back(tMeshNode);
			}
		}

		m_nMeshNum	= m_sFileHeader.NodeNum ;	//节点数量

		//如果不是水
		if(false == m_bIsWaterMesh)
		{
			//有绑定盒模型
			if( true == m_bBoundingBoxMesh )
			{
				if(m_sFileHeader.Ver > 1)
				{
					m_pAllBoundingMeshVertexArray.clear();
					m_pAllBoundingMeshFaceArray.clear();
					m_nAllBoundingMeshVertexSize = 0;
					m_nAllBoundingMeshFaceSize = 0;

					D3DXVECTOR3		tMinVertex  = D3DXVECTOR3(10000,10000,10000);
					D3DXVECTOR3		tMaxVertex  = D3DXVECTOR3(-10000,-10000,-10000); 

					int		tVertexSize = 0;
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tVertexSize , sizeof(int));
					fread(&tVertexSize ,sizeof(int),1,hFile);
					int		tFaceSize = 0 ;
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tFaceSize , sizeof(int));
					fread(&tFaceSize ,sizeof(int),1,hFile);

					lzo_uint DataLen = 0;
					UINT	 DataComLen = 0;
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataLen , sizeof(UINT));
					fread(&DataLen ,sizeof(DataLen),1,hFile);
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &DataComLen , sizeof(UINT));
					fread(&DataComLen ,sizeof(DataComLen),1,hFile);

					if(DataLen > DataComLen)
					{

						BYTE*	tpCompressMemory = new BYTE[DataComLen];
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpCompressMemory , DataComLen);
						fread(tpCompressMemory ,DataComLen,1,hFile);

						BYTE*	tpDecCompressMemory = new BYTE[DataLen];
						// 解压缩 
						lzo1x_decompress(tpCompressMemory, DataComLen, tpDecCompressMemory, &DataLen, NULL);

						D3DXVECTOR3*	pVertexArray = (D3DXVECTOR3*)tpDecCompressMemory;
						m_nAllBoundingMeshVertexSize = m_nAllBoundingMeshVertexSize ;
						for(int v = 0 ; v < tVertexSize ; v++)
						{
							m_pAllBoundingMeshVertexArray.push_back(pVertexArray[v]);
						}
						SExport_Face*	pFaceArray  = (SExport_Face*)(tpDecCompressMemory + tVertexSize * sizeof(D3DXVECTOR3));
						m_nAllBoundingMeshFaceSize = tFaceSize ;
						for(int f = 0 ; f < tFaceSize ; f++)
						{
							m_pAllBoundingMeshFaceArray.push_back(pFaceArray[f]);
						}
						delete[] tpDecCompressMemory ;
						delete[] tpCompressMemory ;
					}
					else
					{

						BYTE*	tpDecCompressMemory = new BYTE[DataLen];
						// 解压缩 
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , tpDecCompressMemory , DataLen);
						fread(tpDecCompressMemory ,DataLen,1,hFile);

						D3DXVECTOR3*	pVertexArray = (D3DXVECTOR3*)tpDecCompressMemory;
						m_nAllBoundingMeshVertexSize = m_nAllBoundingMeshVertexSize ;
						for(int v = 0 ; v < tVertexSize ; v++)
						{
							m_pAllBoundingMeshVertexArray.push_back(pVertexArray[v]);
						}
						SExport_Face*	pFaceArray  = (SExport_Face*)(tpDecCompressMemory + tVertexSize * sizeof(D3DXVECTOR3));
						m_nAllBoundingMeshFaceSize = tFaceSize ;
						for(int f = 0 ; f < tFaceSize ; f++)
						{
							m_pAllBoundingMeshFaceArray.push_back(pFaceArray[f]);
						}
						delete[] tpDecCompressMemory ;
					}
				}
				else
				{
					m_pAllBoundingMeshVertexArray.clear();
					m_pAllBoundingMeshFaceArray.clear();
					m_nAllBoundingMeshVertexSize = 0;
					m_nAllBoundingMeshFaceSize = 0;

					D3DXVECTOR3		tMinVertex  = D3DXVECTOR3(10000,10000,10000);
					D3DXVECTOR3		tMaxVertex  = D3DXVECTOR3(-10000,-10000,-10000); 
					//顶点数量
					//读取
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tVertexSize , sizeof(int));
					fread( &m_nAllBoundingMeshVertexSize ,sizeof(int),1,hFile);
					for(int v = 0 ; v < m_nAllBoundingMeshVertexSize ; v++)
					{
						D3DXVECTOR3	tVertex;
						fread( &tVertex ,sizeof(D3DXVECTOR3),1,hFile);	
						m_pAllBoundingMeshVertexArray.push_back(tVertex);
					}
					//面数量
					//读取
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tFaceSize , sizeof(int));
					fread( &m_nAllBoundingMeshFaceSize ,sizeof(int),1,hFile);
					//ReadMemoryData(pCurrData, nMaxSize , CurrSize , m_pAllBoundingMeshFaceArray , sizeof(SExport_Face) * tFaceSize);
					for(int v = 0 ; v < m_nAllBoundingMeshFaceSize ; v++)
					{
						SExport_Face	tFace;
						fread( &tFace ,sizeof(SExport_Face),1,hFile);	
						m_pAllBoundingMeshFaceArray.push_back(tFace);
					}
				}
			}
			//凹凸贴图渲染方式
			if( true == m_bUseBumpRender )
			{
				SBumpInfo	tBumpInfo;
				fread( &tBumpInfo ,sizeof(SBumpInfo),1,hFile);
			}

			//(2.0版本GMe中包含了材质)
			if(m_sFileHeader.Ver == 2 )
			{
				int		tMatNum  = 0 ;
				//读取
				//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tMatNum , sizeof(int));
				fread( &tMatNum ,sizeof(int),1,hFile);

				if(tMatNum > 0)
				{

					//保存结果
					map<int,STexture>	tTextureMap;

					for(int k = 0 ; k < tMatNum ; k++)
					{
						SMatNode_Temp	tTempMatNode;
						//读取
						//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tTempMatNode.m_sMatInfo , sizeof(SMatNode_New));
						fread( &tTempMatNode ,sizeof(SMatNode_New),1,hFile);
						//单材质
						if(tTempMatNode.m_sMatInfo.m_dwMatType == 0)
						{
							//如果有高光和凹凸贴图系数调整
							if(tTempMatNode.m_sMatInfo.m_dwGHBump == 1)
							{
								//读取
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tTempMatNode.m_fShininess , sizeof(float));
								fread( &tTempMatNode.m_fShininess ,sizeof(float),1,hFile);
								//读取
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tTempMatNode.m_fShinStr , sizeof(float));
								fread( &tTempMatNode.m_fShinStr ,sizeof(float),1,hFile);
							}

							//贴图	
							//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tTempMatNode.m_nTextureNum , sizeof(int));
							fread( &tTempMatNode.m_nTextureNum ,sizeof(int),1,hFile);

							for(int t = 0 ; t < tTempMatNode.m_nTextureNum ; t++)
							{
								STexture	tTexture_Read;
								//贴图	
								//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tTexture_Read , sizeof(STexture_Read));
								fread( &tTexture_Read ,sizeof(STexture),1,hFile);
								STexture		tTexture;

								//原来是通道索引，现在改为纹理索引
								//tTexture.m_nIndex = tTexture_Read.m_nIndex ;
								tTexture.m_nIndex = t ;
								STRING_COPY_ALL( tTexture.m_szFileName ,  tTexture_Read.m_szFileName);

								int len = strlen(tTexture.m_szFileName);
								if(len > 0)
								{

									map<int,STexture>::iterator Iter = tTextureMap.find(k);
									if(Iter == tTextureMap.end())
									{
										tTextureMap.insert( make_pair< int,STexture >( k, tTexture ));
									}
								}
							}

						}
						else
						{
							int		tMaterialNum = 0 ;
							//多材质
							//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &tMaterialNum , sizeof(int));
							fread( &tMaterialNum ,sizeof(int),1,hFile);
						}
					}

					if(false == tTextureMap.empty())
					{
						//================================================================================
						//重新设置材质ID
						vector<int>	tResTextureVec;
						for(int i = 0 ; i < m_nMeshNum ; i++)
						{
							int tSubMeshSize = m_pMeshNodeArray[i].m_nSubMeshNum;;
							for(int j = 0 ; j < tSubMeshSize; j++)
							{	
								//模型信息节点
								SSubMesh_Temp*	tpSubMeshInfo_Temp = &m_pMeshNodeArray[i].m_pSubMeshArray[j];

								map<int,STexture>::iterator tMapIter;
								for(tMapIter = tTextureMap.begin() ; tMapIter != tTextureMap.end(); tMapIter++)
								{
									if(tpSubMeshInfo_Temp->m_sSubMeshInfo.m_nMaterialID == tMapIter->first)
									{
										int		tCount = 0;
										bool	tFind = false;
										vector<int>::iterator tResTextureIter;
										for(tResTextureIter = tResTextureVec.begin() ; tResTextureIter != tResTextureVec.end(); tResTextureIter++)
										{
											if((*tResTextureIter) == tMapIter->first)
											{
												tFind = true;
												tpSubMeshInfo_Temp->m_sSubMeshInfo.m_nMaterialID = tCount;
												break;
											}
											tCount++;
										}
										;

										if(false == tFind)
										{
											tpSubMeshInfo_Temp->m_sSubMeshInfo.m_nMaterialID = tResTextureVec.size();

											tResTextureVec.push_back(tMapIter->first);
										}

										break;
									}

								}

							}
						}

						m_nTextureNum = tResTextureVec.size();


						if(m_nTextureNum > 0)
						{

							m_pTextureVec.clear();
							for(int t = 0 ; t < m_nTextureNum ; t++)
							{
								int		tFindIndex = tResTextureVec[t];
								map<int,STexture>::iterator tFindIter = tTextureMap.find(tFindIndex);
								if(tFindIter != tTextureMap.end())
								{
									STexture	tNewTexture;
									tNewTexture.m_nIndex = t;
									STRING_COPY_ALL(tNewTexture.m_szFileName,tFindIter->second.m_szFileName);
									m_pTextureVec.push_back(tNewTexture);
								}
								else
								{
									STexture	tNewTexture;
									STRING_INIT(tNewTexture.m_szFileName);
									tNewTexture.m_nIndex = -1;
									m_pTextureVec.push_back(tNewTexture);
								}
							}
						}
						else
						{
							//::MessageBox(NULL,"有材质但没贴图","提示",MB_OK);
						}
					}
					else
					{
						m_nTextureNum = 0;
					}
				}
			}
			else if(m_sFileHeader.Ver > 2 )
			{
				//读取
				//ReadMemoryData(pCurrData, nMaxSize , CurrSize , &m_nTextureNum , sizeof(int));
				fread( &m_nTextureNum ,sizeof(int),1,hFile);
				m_pTextureVec.clear();
				if(m_nTextureNum > 0)
				{
					//保存结果
					for(int t = 0 ; t < m_nTextureNum ; t++)
					{
						STexture	tTexture_Read;
						fread( &tTexture_Read ,sizeof(STexture),1,hFile);
						tTexture_Read.m_nIndex = t;
						m_pTextureVec.push_back(tTexture_Read);
					}
				}
			}

		}

		fclose(hFile);
		return TRUE;
	}
	catch (...)
	{
		//MessageBox(NULL,szFileName,"读取失败",MB_OK);
	}
	return FALSE;
}
//取得模型体数量
int		YNDMeshFile::Fun_GetMeshNodeNum()
{
	return m_nMeshNum;
}
//取得对应模型体
SMeshNode_Temp*		YNDMeshFile::Fun_GetMeshNodePtr(int vIndex)
{
	if(vIndex >=0 && vIndex < m_nMeshNum)
	{
		return &m_pMeshNodeArray[vIndex];
	}
	return NULL;
}

//取得材质数量
int		YNDMeshFile::Fun_GetTextureNum()
{
	return	m_nTextureNum;
}
//取得对应的材质
STexture*		YNDMeshFile::Fun_GetTexturePtr( int vIndex)
{
	if(vIndex >= 0 && vIndex < m_nTextureNum )
	{
		return	&m_pTextureVec[vIndex];
	}
	return NULL;
}
//取得材质的贴图名称
const char*		YNDMeshFile::Fun_GetTextureName( int	vIndex)
{
	return	m_pTextureVec[vIndex].m_szFileName;
}
//取得标记
BOOL	YNDMeshFile::CheckFlag(DWORD vHeaderType,WORD	vFlag)
{
	//模型标记
	WORD	tMeshFlag = vHeaderType >> 8;
	WORD	tResult = (tMeshFlag&vFlag);
	return  (tResult>0);
}
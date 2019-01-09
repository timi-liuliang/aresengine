#include "Physics/AresRecast.h"

namespace Ares
{
	// Calc Distance field ( 到边界的最短距离)
	static void CalculateDistanceField(  SRcCompactHeightField& chf, vector<WORD>& src, unsigned short& maxDist)
	{
		const int w = chf.m_width;
		const int h = chf.m_height;

		// Init distance and points( 初始为最大值)
		for ( int i=0; i<chf.m_spanCount; i++)
			src[i] = 0xffff;

		// Mark boundary cells( 也就是至少有一个方向没有链接span)
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index + cell.m_count); i<ni; i++)
				{
					const RcCompactSpan&   s = chf.m_spans[i];
					const BYTE			 area = chf.m_areaIDs[i];

					int  numCell = 0;
					for ( int dir=0; dir<4; dir++)
					{
						// 有链接
						if( RcGetCon( s, dir) != RC_NOT_CONNECTED)
						{
							const int ax = x + RcGetDirOffsetX( dir);
							const int ay = y + RcGetDirOffsetY( dir);
							const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon(s, dir);
							if (area == chf.m_areaIDs[ai])
								numCell++;
						}
					}

					// 不为四,说明此CompactSpan为一边界
					if( numCell != 4)
						src[i] = 0;
				}
			}
		}

		// Pass 1
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					const RcCompactSpan& s = chf.m_spans[i];

					if( RcGetCon( s, 0) != RC_NOT_CONNECTED)
					{
						//( -1, 0)
						const int ax = x + RcGetDirOffsetX( 0);
						const int ay = y + RcGetDirOffsetY( 0);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, 0);
						const RcCompactSpan& as = chf.m_spans[ai];
						if ( src[ai]+2 < src[i])
						{
							src[i] = src[ai] + 2;
						}

						// ( -1, -1)
						if( RcGetCon( as, 3) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX( 3);
							const int aay = ay + RcGetDirOffsetY( 3);
							const int aai = (int)chf.m_cells[aax + aay*w].m_index + RcGetCon( as, 3);
							if( src[aai] + 3 < src[i])
								src[i] = src[aai] + 3;
						}
					}

					if ( RcGetCon( s, 3) != RC_NOT_CONNECTED)
					{
						//( 0, -1)
						const int ax = x + RcGetDirOffsetX( 3);
						const int ay = y + RcGetDirOffsetY( 3);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, 3);
						const RcCompactSpan& as = chf.m_spans[ai];
						if ( src[ai]+2 < src[i])
							src[i] = src[ai] + 2;

						// ( 1, -1)
						if( RcGetCon( as, 2) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX( 2);
							const int aay = ay + RcGetDirOffsetY( 2);
							const int aai = (int)chf.m_cells[aax + aay*w].m_index + RcGetCon( as, 2);
							if( src[aai] + 3 < src[i])
								src[i] = src[aai] + 3;
						}
					}
				}
			}
		}

		// Pass 2
		for ( int y=h-1; y>=0; y--)
		{
			for ( int x=w-1; x>=0; x--)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					const RcCompactSpan& s = chf.m_spans[i];
					if( RcGetCon( s, 2) != RC_NOT_CONNECTED)
					{
						//( 1, 0)
						const int ax = x + RcGetDirOffsetX( 2);
						const int ay = y + RcGetDirOffsetY( 2);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, 2);
						const RcCompactSpan& as = chf.m_spans[ai];
						if ( src[ai]+2 < src[i])
							src[i] = src[ai] + 2;

						// ( -1, -1)
						if( RcGetCon( as, 1) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX( 1);
							const int aay = ay + RcGetDirOffsetY( 1);
							const int aai = (int)chf.m_cells[aax + aay*w].m_index + RcGetCon( as, 1);
							if( src[aai] + 3 < src[i])
								src[i] = src[aai] + 3;
						}
					}

					if ( RcGetCon( s, 1) != RC_NOT_CONNECTED)
					{
						//( 0, 1)
						const int ax = x + RcGetDirOffsetX( 1);
						const int ay = y + RcGetDirOffsetY( 1);
						const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, 1);
						const RcCompactSpan& as = chf.m_spans[ai];
						if ( src[ai]+2 < src[i])
							src[i] = src[ai] + 2;

						// ( -1, 1)
						if( RcGetCon( as, 0) != RC_NOT_CONNECTED)
						{
							const int aax = ax + RcGetDirOffsetX( 0);
							const int aay = ay + RcGetDirOffsetY( 0);
							const int aai = (int)chf.m_cells[aax + aay*w].m_index + RcGetCon( as, 0);
							if( src[aai] + 3 < src[i])
								src[i] = src[aai] + 3;
						}
					}
				}
			}
		}

		maxDist = 0;
		for ( int i=0; i<chf.m_spanCount; i++)
			maxDist = max( src[i], maxDist);
	}

	// 对distance 采用模糊处理
	static void BoxBlur( SRcCompactHeightField& chf, int thr, vector<WORD>& src)
	{
		vector<WORD> dst = src;

		const int w = chf.m_width;
		const int h = chf.m_height;

		thr *= 2;

		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index + cell.m_count); i<ni; i++)
				{
					const RcCompactSpan& s = chf.m_spans[i];
					int cd = src[i];

					if( cd <= thr)
					{
						dst[i] = cd;
						continue;
					}

					int d = cd;
					for ( int dir=0; dir<4; dir++)
					{
						// 非边界
						if( RcGetCon( s, dir) != RC_NOT_CONNECTED)
						{
							const int ax = x + RcGetDirOffsetX( dir);
							const int ay = y + RcGetDirOffsetY( dir);
							const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, dir);
							d += (int)src[ai];

							const RcCompactSpan& as = chf.m_spans[ai];
							const int  dir2 = (dir+1) & 0x3;
							if( RcGetCon( as, dir2) != RC_NOT_CONNECTED)
							{
								const int ax2 = ax + RcGetDirOffsetX( dir2);
								const int ay2 = ay + RcGetDirOffsetY( dir2);
								const int ai2 = (int)chf.m_cells[ax2+ay2*w].m_index + RcGetCon( as, dir2);
								d += (int)src[ai2];
							}
							else
								d += cd;
						}
						else
						{
							d += cd*2;
						}
					}

					dst[i] = (unsigned short)((d+5)/9);
				}
			}
		}

		src.swap( dst);
	}

	// Builds distance field and stores it into the combat heightfield
	// Params:
	// chf - compact heightfield representing the open space
	// Returns false if operation run out of memory
	bool RcBuildDistanceField( SRcCompactHeightField& chf)
	{
		vector<WORD> src( chf.m_spanCount);

		unsigned short  maxDist = 0;

		CalculateDistanceField( chf, src, maxDist);

		// 记录最大距离
		chf.m_iMaxDistance = maxDist;

		// Blur
		BoxBlur( chf, 1, src);

		// Store distance
		for( int i=0; i<chf.m_spanCount; i++)
			chf.m_spans[i].m_dist = src[i];

		return true;
	}

	// 区域否合法( 到边界的距离是否大于等于代理需站立的区域大小)
	static void PaintRectRegion( int minX, int maxX, int minY, int maxY, unsigned short reginID, SRcCompactHeightField& chf, vector<WORD>& srcReg)
	{
		const int w = chf.m_width;

		for ( int y=minY; y<maxY; y++)
		{
			for ( int x=minX; x<maxX; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					if( chf.m_areaIDs[i]!=RC_NULL_AREA)
						srcReg[i] = reginID;
				}
			}
		}
	}

	static bool FloodRegion( int x, int y, int i, unsigned short level, unsigned short regionId, SRcCompactHeightField& chf, vector<WORD>& srcReg,  vector<WORD>& srcDist, RcIntArray& stack)
	{
		const int  w	= chf.m_width;
		const BYTE area = chf.m_areaIDs[i];

		// flood will mark region
		stack.resize( 0);
		stack.push_back( (int)x);
		stack.push_back( (int)y);
		stack.push_back( (int)i);

		srcReg[i]  = regionId;
		srcDist[i] = 0;

		unsigned short lev = level >= 2 ? level-2 : 0;
		int count = 0;

		while( stack.size() > 0)
		{
			int ci = stack.back(); stack.pop_back();
			int cy = stack.back(); stack.pop_back();
			int cx = stack.back(); stack.pop_back();

			const RcCompactSpan& cs = chf.m_spans[ci];

			// check if any of the neighbours already have a valid region set
			unsigned short ar = 0;
			for ( int dir=0; dir<4; dir++)
			{
				// 8 connected
				if( RcGetCon( cs, dir) != RC_NOT_CONNECTED)
				{
					const int ax = cx + RcGetDirOffsetX(dir);
					const int ay = cy + RcGetDirOffsetY(dir);
					const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( cs, dir);
					if( chf.m_areaIDs[ai] != area)
						continue;

					unsigned short nr = srcReg[ai];
					if( nr != 0  && nr != regionId)
						ar = nr;

					const RcCompactSpan& as = chf.m_spans[ai];

					const int dir2 = (dir+1) & 0x3;
					if ( RcGetCon( as, dir2) != RC_NOT_CONNECTED)
					{
						const int ax2 = ax + RcGetDirOffsetX( dir2);
						const int ay2 = ay + RcGetDirOffsetY( dir2);
						const int ai2 = (int)chf.m_cells[ax2+ay2*w].m_index + RcGetCon( as, dir2);
						if( chf.m_areaIDs[ai2] != area)
							continue;

						unsigned short nr2 = srcReg[ai2];
						if( nr2 != 0 && nr2 != regionId)
							ar = nr2;
					}
				}
			}

			if( ar != 0)
			{
				srcReg[ci] = 0;
				continue;
			}
			count++;

			// expand neighbours
			for ( int dir=0; dir<4; ++dir)
			{
				if ( RcGetCon( cs, dir) != RC_NOT_CONNECTED)
				{
					const int ax = cx + RcGetDirOffsetX( dir);
					const int ay = cy + RcGetDirOffsetY( dir);

					const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( cs, dir);
					if( chf.m_areaIDs[ai]!=area)
						continue;

					if( chf.m_spans[ai].m_dist >= lev && srcReg[ai]==0)
					{
						srcReg[ai]   = regionId;
						srcDist[ai] = 0;
						stack.push_back( ax);
						stack.push_back( ay);
						stack.push_back( ai);
					}
				}
			}
		}

		return count > 0;
	}

	// Expand region
	static void ExpandRegions( int maxIter, unsigned short level, SRcCompactHeightField& chf, vector<WORD>& srcReg, vector<WORD>& srcDist, RcIntArray& stack)
	{
		vector<WORD> dstReg( chf.m_spanCount);
		vector<WORD> dstDist( chf.m_spanCount);

		const int w = chf.m_width;
		const int h = chf.m_height;

		// Find cells revealed by the raised level
		stack.resize(0);
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index+cell.m_count); i<ni; i++)
				{
					if ( chf.m_spans[i].m_dist >= level && srcReg[i]==0 && chf.m_areaIDs[i]!=RC_NULL_AREA)
					{
						stack.push_back(x);
						stack.push_back(y);
						stack.push_back(i);
					}
				}
			}
		}

		bool  isSwap = false;
		int   iter	 = 0;
		while( stack.size() > 0)
		{
			int failed = 0;

			dstReg  = srcReg;
			dstDist = srcDist;

			for( size_t j=0; j<stack.size(); j+=3)
			{
				int x = stack[j+0];
				int y = stack[j+1];
				int i = stack[j+2];
				if( i<0)
				{
					failed++;
					continue;
				}

				unsigned short r  = srcReg[i];
				unsigned short d2 = 0xFFFF;
				const unsigned char area = chf.m_areaIDs[i];
				const RcCompactSpan& s  = chf.m_spans[i];

				for ( int dir=0; dir<4; dir++)
				{
					if( RcGetCon( s, dir)== RC_NOT_CONNECTED)
						continue;

					const int ax = x + RcGetDirOffsetX(dir);
					const int ay = y + RcGetDirOffsetY(dir);
					const int ai = (int)chf.m_cells[ax+ay*w].m_index + RcGetCon( s, dir);
					if( chf.m_areaIDs[ai]!=area) 
						continue;

					if( srcReg[ai]>0 && ( srcReg[ai] & RC_BORDER_REG)==0)
					{
						if( (int)srcDist[ai]+2 < (int)d2)
						{
							r  = srcReg[ai];
							d2 = srcDist[ai] + 2;
						}
					}
				}
				if ( r)
				{
					// mark as used
					stack[j+2] = -1;
					dstReg[i] = r;
					dstDist[i] = d2;
				}
				else
				{
					failed++;
				}
			}

			// swap source and dst
			srcReg.swap( dstReg);
			srcDist.swap(dstDist);
			isSwap = !isSwap;

			if ( failed*3 == stack.size())
				break;

			if ( level > 0)
			{
				++iter;
				if( iter >= maxIter)
					break;
			}
		}

		if( isSwap)
		{
			srcReg.swap( dstReg);
			srcDist.swap(dstDist);
		}
	}

	// RcRegion
	struct RcRegion
	{
		inline RcRegion() 
			: m_spanCount(0), m_id(0), m_areaType(0), m_remap(false), m_visited(false)
		{}

		int				m_spanCount;
		unsigned short	m_id;
		unsigned char	m_areaType;	// AreaType
		bool			m_remap;
		bool			m_visited;
		RcIntArray		m_connections;
		RcIntArray		m_floors;
	};

	// 添加独一无二的Region
	static void AddUniqueFloorRegion( RcRegion& reg, unsigned short n)
	{
		for ( size_t i=0; i<reg.m_floors.size(); i++)
		{
			if( reg.m_floors[i] == n)
				return;
		}

		reg.m_floors.push_back( n);
	}

	static void RemoveAdjacentNeighbours(RcRegion& reg)
	{
		// Remove adjacent duplicates.
		for( size_t i=0; i<reg.m_connections.size() && reg.m_connections.size()>1; )
		{
			int ni = (i+1) % reg.m_connections.size();
			if (reg.m_connections[i] == reg.m_connections[ni])
			{
				// Remove duplicate
				for (int j = i; j < (int)reg.m_connections.size()-1; ++j)
					reg.m_connections[j] = reg.m_connections[j+1];
				reg.m_connections.pop_back();
			}
			else
				++i;
		}
	}

	// 替换邻接
	static void ReplaceNeighbour( RcRegion& reg, unsigned short oldId, unsigned short newId)
	{
		bool neiChanged = false;
		for( size_t i = 0; i < reg.m_connections.size(); ++i)
		{
			if (reg.m_connections[i] == oldId)
			{
				reg.m_connections[i] = newId;
				neiChanged = true;
			}
		}
		for( size_t i = 0; i < reg.m_floors.size(); ++i)
		{
			if (reg.m_floors[i] == oldId)
				reg.m_floors[i] = newId;
		}
		if (neiChanged)
			RemoveAdjacentNeighbours(reg);
	}

	// 是否连接到边缘
	static bool IsRegionConnectedToBorder( const RcRegion& reg)
	{
		// Region is connected to border if one of the neighbours is null id
		for ( size_t i = 0; i < reg.m_connections.size(); ++i)
		{
			if (reg.m_connections[i] == 0)
				return true;
		}
		return false;
	}

	// 是否可以合并
	static bool CanMergeWithRegion( RcRegion& rega, RcRegion& regb)
	{
		if( rega.m_areaType != regb.m_areaType)
			return false;

		int n = 0;
		for( size_t i = 0; i<rega.m_connections.size(); ++i)
		{
			if (rega.m_connections[i] == regb.m_id)
				n++;
		}

		if (n > 1)
			return false;

		for ( size_t i= 0; i<rega.m_floors.size(); ++i)
		{
			if (rega.m_floors[i] == regb.m_id)
				return false;
		}

		return true;
	}

	// 合并区域
	static bool MergeRegions(RcRegion& rega, RcRegion& regb)
	{
		unsigned short aid = rega.m_id;
		unsigned short bid = regb.m_id;

		// Duplicate current neighbourhood.
		RcIntArray acon;
		acon.resize(rega.m_connections.size());

		for ( size_t i = 0; i < rega.m_connections.size(); ++i)
			acon[i] = rega.m_connections[i];

		RcIntArray& bcon = regb.m_connections;

		// Find insertion point on A.
		int insa = -1;
		for ( size_t i = 0; i < acon.size(); ++i)
		{
			if (acon[i] == bid)
			{
				insa = i;
				break;
			}
		}

		if (insa == -1)
			return false;

		// Find insertion point on B.
		int insb = -1;
		for ( size_t i = 0; i < bcon.size(); ++i)
		{
			if (bcon[i] == aid)
			{
				insb = i;
				break;
			}
		}
		if (insb == -1)
			return false;

		// Merge neighbours.
		rega.m_connections.resize(0);
		for (int i = 0, ni = acon.size(); i < ni-1; ++i)
			rega.m_connections.push_back(acon[(insa+1+i) % ni]);

		for (int i = 0, ni = bcon.size(); i < ni-1; ++i)
			rega.m_connections.push_back(bcon[(insb+1+i) % ni]);

		RemoveAdjacentNeighbours(rega);

		for ( size_t j = 0; j < regb.m_floors.size(); ++j)
			AddUniqueFloorRegion(rega, regb.m_floors[j]);

		rega.m_spanCount += regb.m_spanCount;
		regb.m_spanCount = 0;
		regb.m_connections.resize(0);

		return true;
	}

	// IsSolidEdge
	static bool IsSolidEdge( SRcCompactHeightField& chf, vector<WORD>& srcReg, int x, int y, int i, int dir)
	{
		const RcCompactSpan& s = chf.m_spans[i];
		unsigned short r = 0;

		if( RcGetCon( s, dir) != RC_NOT_CONNECTED)
		{
			const int ax = x + RcGetDirOffsetX( dir);
			const int ay = y + RcGetDirOffsetY( dir);
			const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon( s, dir);
			r = srcReg[ai];
		}

		if( r == srcReg[i])
			return false;

		return true;
	}

	static void WalkContour( int x, int y, int i, int dir, SRcCompactHeightField& chf, vector<WORD>& srcReg, RcIntArray& cont)
	{
		int startDir = dir;
		int starti   = i;

		const RcCompactSpan& ss     = chf.m_spans[i];
		unsigned short        curReg = 0;

		if (RcGetCon(ss, dir) != RC_NOT_CONNECTED)
		{
			const int ax = x + RcGetDirOffsetX(dir);
			const int ay = y + RcGetDirOffsetY(dir);
			const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon(ss, dir);
			curReg = srcReg[ai];
		}

		cont.push_back(curReg);

		int iter = 0;
		while (++iter < 40000)
		{
			const RcCompactSpan& s = chf.m_spans[i];

			if (IsSolidEdge(chf, srcReg, x, y, i, dir))
			{
				// Choose the edge corner
				unsigned short r = 0;
				if (RcGetCon(s, dir) != RC_NOT_CONNECTED)
				{
					const int ax = x + RcGetDirOffsetX(dir);
					const int ay = y + RcGetDirOffsetY(dir);
					const int ai = (int)chf.m_cells[ax+ay*chf.m_width].m_index + RcGetCon(s, dir);
					r = srcReg[ai];
				}
				if (r!=curReg)
				{
					curReg = r;
					cont.push_back(curReg);
				}

				dir = (dir+1) & 0x3;  // Rotate CW
			}
			else
			{
				int ni = -1;
				const int nx = x + RcGetDirOffsetX(dir);
				const int ny = y + RcGetDirOffsetY(dir);

				if (RcGetCon(s, dir) != RC_NOT_CONNECTED)
				{
					const SRcCompactCell& nc = chf.m_cells[nx+ny*chf.m_width];
					ni = (int)nc.m_index + RcGetCon(s, dir);
				}
				if (ni == -1)
				{
					// Should not happen.
					return;
				}
				x = nx;
				y = ny;
				i = ni;
				dir = (dir+3) & 0x3;	// Rotate CCW
			}

			if (starti == i && startDir == dir)
			{
				break;
			}
		}

		// Remove adjacent duplicates.
		if (cont.size() > 1)
		{
			for ( size_t j=0; j<cont.size(); )
			{
				int nj = (j+1) % cont.size();
				if (cont[j] == cont[nj])
				{
					for (int k = j; k<(int)cont.size()-1; ++k)
						cont[k] = cont[k+1];
					cont.pop_back();
				}
				else
					++j;
			}
		}
	}


	static bool FilterSmallRegions(int minRegionSize, int mergeRegionSize, unsigned int& maxRegionId, SRcCompactHeightField& chf, vector<WORD>& srcReg)
	{
		const int w = chf.m_width;
		const int h = chf.m_height;

		int nreg = maxRegionId + 1;
		vector<RcRegion> regions(nreg);
		for ( int i=0; i<nreg; i++)
			regions[i].m_id = (unsigned short)i;

		// Find edge of a region and find connections around the contour
		for ( int y=0; y<h; y++)
		{
			for ( int x=0; x<w; x++)
			{
				const SRcCompactCell& cell = chf.m_cells[x+y*w];
				for ( int i=(int)cell.m_index, ni=(int)(cell.m_index + cell.m_count); i<ni; i++)
				{
					unsigned short r = srcReg[i];
					if ( r==0 || r>= nreg)
						continue;

					RcRegion& reg = regions[r];
					reg.m_spanCount++;

					// update floors
					for ( int j=(int)cell.m_index; j<ni; j++)
					{
						if( i==j)
							continue;

						unsigned short floorId = srcReg[j];
						if ( floorId == 0 || floorId>=nreg)
							continue;

						AddUniqueFloorRegion( reg, floorId);
					}

					// Have found contour
					if ( reg.m_connections.size() > 0)
						continue;

					reg.m_areaType = chf.m_areaIDs[i];

					// check if this cell is next to a border
					int ndir = -1;
					for (int dir = 0; dir < 4; ++dir)
					{
						if( IsSolidEdge( chf, srcReg, x, y, i, dir))
						{
							ndir = dir;
							break;
						}
					}

					if( ndir != -1)
					{
						// the cell is at border
						// walk around the contour to find all the neighbours
						WalkContour( x, y, i, ndir, chf, srcReg, reg.m_connections);
					}
				}
			}
		}

		// remove too small unconnected regions( 移除太小的无链接的区域)
		RcIntArray stack(32);
		RcIntArray trace(32);
		for ( int i=0; i<nreg; i++)
		{
			RcRegion& reg = regions[i];

			if( reg.m_id==0 || (reg.m_id & RC_BORDER_REG))
				continue;
			if ( reg.m_spanCount == 0)
				continue;
			if( reg.m_visited)
				continue;

			// count the total size of all the connected regions
			// also keep track of the regions connects to a tile border
			bool conectToBorder = false;
			int	 spanCount		 = 0;
			stack.resize( 0);
			trace.resize( 0);
			reg.m_visited = true;
			stack.push_back( i);
			while( stack.size())
			{
				// pop
				int ri = stack.back(); stack.pop_back();
				RcRegion& creg  = regions[ri];

				spanCount += creg.m_spanCount;
				trace.push_back( ri);

				for ( size_t j=0; j<creg.m_connections.size(); j++)
				{
					if( creg.m_connections[j] & RC_BORDER_REG)
					{
						conectToBorder = true;
						continue;
					}

					RcRegion& neireg = regions[creg.m_connections[j]];
					if( neireg.m_visited)
						continue;
					if( neireg.m_id == 0 || (neireg.m_id & RC_BORDER_REG))
						continue;

					// visit
					stack.push_back( neireg.m_id);
					neireg.m_visited = true;
				}
			}

			// If the accumulated regions size is too small, remove it.
			// Do not remove areas which connect to tile borders
			// as their size cannot be estimated correctly and removing them
			// can potentially remove necessary areas.
			if (spanCount < minRegionSize && !conectToBorder)
			{
				// Kill all visited regions.
				for ( size_t j = 0; j < trace.size(); ++j)
				{
					regions[trace[j]].m_spanCount = 0;
					regions[trace[j]].m_id		  = 0;
				}
			}
		}

		// Megre too small regions to neighbour regions(合并小区域到领区域)
		int mergeCount = 0;
		do 
		{
			mergeCount = 0;
			for ( int i=0; i<nreg; i++)
			{
				RcRegion& reg = regions[i];
				if ( reg.m_id == 0 || (reg.m_id & RC_BORDER_REG))
					continue;
				if( reg.m_spanCount == 0)
					continue;

				// check to see if the region should be merged
				if( reg.m_spanCount > mergeRegionSize && IsRegionConnectedToBorder( reg))
					continue;

				// Small region with more than one connection
				// or region which is not connected to a border at all.
				// Find smallest neighbour region that connects to this one
				int smallest = 0xfffffff;
				unsigned short mergeId = reg.m_id;
				for ( size_t j=0; j<reg.m_connections.size(); j++)
				{
					if( reg.m_connections[j] & RC_BORDER_REG)
						continue;

					RcRegion& mreg = regions[ reg.m_connections[j]];
					if( mreg.m_id == 0 || (mreg.m_id & RC_BORDER_REG))
						continue;

					if ( mreg.m_spanCount < smallest && CanMergeWithRegion( reg, mreg) && CanMergeWithRegion( mreg, reg))
					{
						smallest = mreg.m_spanCount;
						mergeId  = mreg.m_id;
					}
				}

				// Found new id
				if ( mergeId != reg.m_id)
				{
					unsigned short oldId = reg.m_id;
					RcRegion& target = regions[mergeId];

					// Merge neighbours
					if ( MergeRegions( target, reg))
					{
						// Fixup regions pointing to current region
						for (int j = 0; j < nreg; ++j)
						{
							if (regions[j].m_id == 0 || (regions[j].m_id & RC_BORDER_REG))
								continue;

							// If another region was already merged into current region
							// change the nid of the previous region too.
							if (regions[j].m_id == oldId)
								regions[j].m_id = mergeId;

							// Replace the current region with the new one if the
							// current regions is neighbour.
							ReplaceNeighbour(regions[j], oldId, mergeId);
						}
						mergeCount++;
					}
				}
			}
		} 
		while( mergeCount>0);

		// Compress region Ids
		for ( int i=0; i<nreg; i++)
		{
			regions[i].m_remap = false;

			// skip all regions
			if ( regions[i].m_id == 0)
				continue;

			// skip external regions
			if( regions[i].m_id & RC_BORDER_REG)
				continue;

			regions[i].m_remap = true;
		}

		unsigned short regIdGen = 0;
		for ( int i=0; i<nreg; i++)
		{
			if ( !regions[i].m_remap)
				continue;

			unsigned short oldId = regions[i].m_id;
			unsigned short newId = ++regIdGen;

			for ( int j=i; j<nreg; j++)
			{
				if( regions[j].m_id == oldId)
				{
					regions[j].m_id = newId;
					regions[j].m_remap = false;
				}
			}
		}

		maxRegionId = regIdGen;

		// Remap regions
		for ( int i=0; i<chf.m_spanCount; i++)
		{
			if( (srcReg[i] & RC_BORDER_REG)==0)
				srcReg[i] = regions[srcReg[i]].m_id;
		}

		return true;
	}


	// Divides the walkable heighfied into simple regions.
	// Each region has only one contour and no overlaps.
	// The regions are stored in the compact heightfield 'reg' field.
	// The regions will be shrinked by the radius of the agent.
	// The process sometimes creates small regions. The parameter
	// 'minRegionSize' specifies the smallest allowed regions size.
	// If the area of a regions is smaller than allowed, the regions is
	// removed or merged to neighbour region.
	// Params:
	// chf				 - compact heightfield representing the open space
	// walkableRadius	 - the radius of the agent
	// minRegionSize	 - the smallest allowed regions size.
	// maxMergeRegionSize- the largest allowed regions size which can be merged
	// return false if operation ran out of memory
	bool RcBuildRegions( SRcCompactHeightField& chf, int borderSize, int minRegionArea, int mergeRegionArea)
	{
		const int w = chf.m_width;
		const int h = chf.m_height;

		RcIntArray stack(1024);
		RcIntArray visited(1024);

		vector<WORD> srcReg( chf.m_spanCount, 0);
		vector<WORD> srcDist( chf.m_spanCount, 0);

		unsigned short regionId = 1;
		unsigned short level    =(chf.m_iMaxDistance+1) & ~1;

		const int expandIters = 8;

		// Mark border regions
		if( borderSize>0)
		{
			// Make sure border will not overflow
			const int bw = min( w, borderSize);
			const int bh = min( h, borderSize);

			// Paint regions
			PaintRectRegion( 0,           bw,		   0,		     h,		regionId|RC_BORDER_REG, chf, srcReg); regionId++;
			PaintRectRegion( w-bw,		   w,		   0,			 h,		regionId|RC_BORDER_REG, chf, srcReg); regionId++;
			PaintRectRegion( 0,            w,		   0,            bh,	regionId|RC_BORDER_REG, chf, srcReg); regionId++;
			PaintRectRegion( 0,            w,		   h-bh,		 h,		regionId|RC_BORDER_REG, chf, srcReg); regionId++;
		
			chf.m_borderSize = borderSize;
		}
		
		while( level > 0)
		{
			level = level >= 2 ? level-2 : 0;

			// Expand current regions until no empty connected cells found
			ExpandRegions( expandIters, level, chf, srcReg, srcDist, stack);
				
			// Mark new regions with IDs
			for ( int y=0; y<h; y++)
			{
				for ( int x=0; x<w; x++)
				{
					const SRcCompactCell& cell = chf.m_cells[x+y*w];
					for ( int i = (int)cell.m_index, ni = (int)(cell.m_index+cell.m_count); i<ni; i++)
					{
						if ( chf.m_spans[i].m_dist < level || srcReg[i] != 0 || chf.m_areaIDs[i]==RC_NULL_AREA)
							continue;

						if( FloodRegion( x, y, i, level, regionId, chf, srcReg, srcDist, stack))
						{
							regionId++;
						}
					}
				}
			}
		}

		// Expand current regions until no empty connected cells found
		ExpandRegions( expandIters*8, 0, chf, srcReg, srcDist, stack);

		// Filter out small regions( 过滤小区域)
		chf.m_iMaxRegions = regionId;
		if ( !FilterSmallRegions( minRegionArea, mergeRegionArea, chf.m_iMaxRegions, chf, srcReg))
			return false;

		// write the result out
		for ( int i=0; i<chf.m_spanCount; i++)
		{
			chf.m_spans[i].m_regID = srcReg[i];
		}

		return true;
	}
}
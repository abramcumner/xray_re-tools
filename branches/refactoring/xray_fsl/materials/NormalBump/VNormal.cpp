/**********************************************************************
 *<
	FILE: VNormal.cpp

	DESCRIPTION:

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#include "VNormal.h"

//-----------------------------------------------------------------------------
//--- Constants

//Defines an invalid or "null" map channel number
#define NULL_MESHMAP (-(NUM_HIDDENMAPS+1))

Point3 zeroNormal(0,0,0);
Point3 stubNormal(0,0,1);
TangentBasis zeroTangentBasis = ConstructTangentBasis( Point3(0,0,0), Point3(0,0,0) );
TangentBasis stubTangentBasis = ConstructTangentBasis( Point3(1,0,0), Point3(0,1,0) );

//-----------------------------------------------------------------------------
//--- General utilities

void memswp( void* a, void* b, int size ) {
	char* temp = new char[size];
	memcpy( temp, a, size );
	memcpy( a, b, size );
	memcpy( b, temp, size );
	delete[] temp;
}

TangentBasis ConstructTangentBasis( Point3 uBasis, Point3 vBasis ) {
	TangentBasis retVal;
	retVal.uBasis=uBasis, retVal.vBasis=vBasis;
	return retVal;
}


//===========================================================================
//
// Class VertFaceLookup
//
// Indicates the faces bordering each vertex in a mesh
// Helper for VertFaceGrouping
//
//===========================================================================

VertFaceLookup::VertFaceLookup() {
	vertFaces = NULL, vertFaceIndex = NULL;
}

VertFaceLookup::~VertFaceLookup() {
	if( vertFaces!=NULL )		delete[] vertFaces;
	if( vertFaceIndex!=NULL )	delete[] vertFaceIndex;
	vertFaces = NULL, vertFaceIndex = NULL;
}

int VertFaceLookup::GetCount( int vertIndex ) {
	return vertFaceIndex[ vertIndex+1 ] - vertFaceIndex[ vertIndex ];
}

VertFace VertFaceLookup::GetVertFace( int vertIndex, int i ) {
	int index = i + vertFaceIndex[ vertIndex ];
	return vertFaces[ index ];
}

void VertFaceLookup::Init( Mesh& mesh ) {
	int numVerts = mesh.numVerts;
	int numFaces = mesh.numFaces;
	int i, f, v, fv; //loop variables

	// 1. Calculate number of faces per vert
	vertFaceIndex = new int[ numVerts + 1 ];
	int* vertFaceCount = vertFaceIndex;
	int  vertFaceTotal = 0;
	if( numVerts>0 ) memset( vertFaceCount, 0, numVerts*sizeof(int) );
	for( f=0; f<numFaces; f++ ) {
		int numFaceVerts = 3; //GetNumFaceVerts(i);
		for( int fv=0; fv<numFaceVerts; fv++ ) {
			int vertIndex = mesh.faces[ f ].getVert( fv );
			vertFaceCount[ vertIndex ]++;
			vertFaceTotal++;
		}
	}


	// 2. Calculate the vert -> vertFace indexing.
	// Each vert will have a contiguous set of vertFaces in the vertFace array;
	// calculate the index of the first entry in the array for each vert
	int vertFaceCurIndex = 0;
	for( v=0; v<numVerts; v++ ) {
		int temp = vertFaceCount[v];
		vertFaceIndex[v] = vertFaceCurIndex;
		vertFaceCurIndex += temp;
	}
	vertFaceIndex[numVerts] = vertFaceCurIndex;


	// 3. Allocate the vert -> face lookup (the vertFace array)
	vertFaces = new VertFace[ vertFaceTotal>0? vertFaceTotal:1 ];
	if( vertFaceTotal>0 ) memset( vertFaces, 0xFFFFFFFF, vertFaceTotal * sizeof(VertFace) );


	// 4. Initialize the vert -> face lookup
	for( f=0; f<numFaces; f++ ) {
		int numFaceVerts = 3; //GetNumFaceVerts(i);
		for( fv=0; fv<numFaceVerts; fv++ ) {
			int vertIndex = mesh.faces[ f ].getVert( fv );
			int vertFaceCur =  vertFaceIndex[ vertIndex ];
			int vertFaceNext = vertFaceIndex[ vertIndex+1 ];
			for( i=vertFaceCur; i<vertFaceNext; i++ )
				if( vertFaces[i].faceIndex==0xFFFFFFFF ) break;
			DbgAssert( i!=vertFaceNext );
			vertFaces[i].faceIndex = f;
			vertFaces[i].faceVertIndex = fv;
		}
	}
}


//===========================================================================
//
// Class VertFaceGrouping
//
// Indicates the faces bordering each vertex in a mesh
//
//===========================================================================


void VertFaceGrouping::Init( Mesh& mesh, int mapChannel ) {
	int numFaces = mesh.getNumFaces();
	int numVerts = mesh.getNumVerts();

	MeshData meshData;
	meshData.Init( mesh, mapChannel );

	VertFaceLookup lookup;
	lookup.Init( mesh );

	faceBits.SetCount( numFaces );
	groupCounts.SetCount( numVerts );
	if( numFaces>0 ) memset( faceBits.Addr(0), 0, numFaces*sizeof(WORD) );
	if( numVerts>0 ) memset( groupCounts.Addr(0), 0, numVerts*sizeof(WORD) );

	Tab<FaceData> faceData;
	faceData.SetCount( numFaces );
	for( int f=0; f<numFaces; f++ ) {
		faceData[f].Init( meshData, f );
		if( faceData[f].IsFaceted() ) faceBits[f] = BIT_ISFACETED;
	}

	for( int v=0; v<numVerts; v++ ) {
		InitVert( faceData, lookup, v );
	}
	}

void VertFaceGrouping::Free() {
	faceBits.SetCount(0);
	groupCounts.SetCount(0);
}

int VertFaceGrouping::GetGroup( int faceIndex, int faceVertIndex ) {
	WORD bits = faceBits[ faceIndex ];
	switch( faceVertIndex ) {
	case 0:		return (0x001F & (bits));
	case 1:		return (0x001F & (bits>>5));
	case 2:		return (0x001F & (bits>>10));
	}
	return -1; //error
}

void VertFaceGrouping::SetGroup( int faceIndex, int faceVertIndex, int group ) {
	WORD& bits = faceBits[ faceIndex ];
	group = group & 0x001F;
	switch( faceVertIndex ) {
	case 0:		bits = (bits & 0xFFE0) | (group);			break;
	case 1:		bits = (bits & 0xFC1F) | (group<<5);		break;
	case 2:		bits = (bits & 0x83FF) | (group<<10);		break;
	}
}

WORD VertFaceGrouping::GetGroupCount( int vertIndex ) {return groupCounts[vertIndex];}

void VertFaceGrouping::Compact() {
	groupCounts.Resize(0);
}

void VertFaceGrouping::InitVert( Tab<FaceData>& faceData, VertFaceLookup& lookup, int vertIndex ) {
	int faceCount = lookup.GetCount( vertIndex );

	GroupData** groupDataList = new GroupData*[ faceCount ];
	int groupCount = 0;

	for( int i=0; i<faceCount; i++ ) {
		VertFace vertFace = lookup.GetVertFace( vertIndex, i );
		int f = vertFace.faceIndex;
		int fv = vertFace.faceVertIndex;

		if( faceData[f].IsFaceted() ) continue;
		GroupData& groupDataCur = faceData[f][fv];

      int j;
		for( j=0; j<groupCount; j++ )
			if( groupDataList[j]->IsMatch( groupDataCur ) ) {
				groupDataList[j]->Merge( groupDataCur );
				break;
			}

		if( j==32 ) j=31; //NOTE: limit 32 groups per vert

		if( j==groupCount ) {
			groupDataList[j] = &groupDataCur;
			groupCount++;
		}

		SetGroup( f, fv, j );
	}
	groupCounts[ vertIndex ] = groupCount;

	delete[] groupDataList;
}

//===========================================================================
//
// Class MeshData and GroupData; subclasses of VertFaceGrouping
// Helper classes
//
//===========================================================================

void VertFaceGrouping::MeshData::Init( Mesh& mesh, int mapChannel ) {
	geomFaces	 = mesh.faces;
	mapFaces	 = mesh.mapFaces( mapChannel );
	normalSpec	 = mesh.GetSpecifiedNormals();
	if( (normalSpec!=NULL) && (normalSpec->GetNumNormals()==0) )
		normalSpec=NULL;

	// Initialize the mapBackfacing bits
	int numFaces = mesh.getNumFaces();
	mapBackfacing.SetSize( numFaces );
	mapBackfacing.ClearAll();
	UVVert* tv = mesh.mapVerts( mapChannel );
	if( (mapFaces!=NULL) && (tv!=NULL) ) {
		Point3 mapNormal;
		for( int f=0; f<numFaces; f++ ) {
			DWORD* tf = mapFaces[f].t;
			mapNormal = FNormalize( (tv[tf[1]] - tv[tf[0]]) ^ (tv[tf[2]] - tv[tf[1]]) );
			mapBackfacing.Set( f, (mapNormal.z<0? TRUE:FALSE) );
		}
	}
}

void VertFaceGrouping::GroupData::Init( MeshData& meshData, int faceIndex, int faceVertIndex ) {
	smGroup = meshData.geomFaces[faceIndex].smGroup;
	if( meshData.mapFaces!=NULL ) {
		mapVert = meshData.mapFaces[faceIndex].t[faceVertIndex];
		if( meshData.mapBackfacing[faceIndex] )
			mapVert |= 0x80000000; //backfacing flag held in upper bit
		}
	else mapVert = -1;
	if( meshData.normalSpec!=NULL )
		 normalSpec = meshData.normalSpec->GetNormal( faceIndex, faceVertIndex );
	else normalSpec.Set(0,0,0);
	}

BOOL VertFaceGrouping::GroupData::IsMatch( GroupData& that ) {
	if( !(this->smGroup & that.smGroup) )		return FALSE;
	if( this->normalSpec != that.normalSpec )	return FALSE;
	//note: backfacing flag held in upper bit of mapVert value
	if( this->mapVert != that.mapVert )			return FALSE;
	return TRUE;
		}

void VertFaceGrouping::GroupData::Merge( GroupData& that ) {
	this->smGroup |= that.smGroup;
	}

void VertFaceGrouping::FaceData::Init( MeshData& meshData, int faceIndex ) {
	groupData[0].Init( meshData, faceIndex, 0 );
	groupData[1].Init( meshData, faceIndex, 1 );
	groupData[2].Init( meshData, faceIndex, 2 );
}

BOOL VertFaceGrouping::FaceData::IsFaceted() {
	DWORD smGroup = groupData[0].smGroup; //arbitrarily use the first item
	DbgAssert( ((groupData[1].smGroup & smGroup) || (groupData[1].smGroup ==0 ))
		&& ((groupData[2].smGroup & smGroup) || (groupData[2].smGroup==0 )) );
	return (smGroup==0? TRUE:FALSE);
		}

//===========================================================================
//
// VNormalChannel
//
// Holds a normal vector and related data at each vertex & smoothing group;
// that is, one piece of data per vertex per smoothing group at the vertex.
//
//===========================================================================

VNormalChannel::VNormalChannel() {
	valid = validTangentBasis = validNormals = FALSE;
	mapChannel = 0;
	numItems = 0;
}

void VNormalChannel::Init( Mesh& mesh, int mapChannel ) {
	int numFaces = mesh.getNumFaces(), numVerts = mesh.getNumVerts();
	vertOffset.SetCount( numVerts );
	vertBase.SetCount( 1+(numVerts>>8) );
	faceBase.SetCount( 1+(numFaces>>8) );
	this->mapChannel = mapChannel;

	// 1. Calculate the surface normal groups for each vert
	grouping.Init( mesh, mapChannel );

	// The array of surface normals will have two sections;
	// first the vert normals, then the face normals for faceted faces.

	// 2. Initialize the lookup values for the vert normals
	int itemCount = 0, itemBase = 0;
	for( int v=0; v<numVerts; v++ ) {
		if( (v&0xFF)==0 )
			vertBase[v>>8] = itemBase = itemCount;
		WORD offset = (WORD)(itemCount-itemBase);
		vertOffset[v] = offset;
		itemCount += grouping.GetGroupCount(v);
	}

	// 3. Initialize the lookup values for the face normals
	for( int f=0; f<numFaces; f++ ) {
		if( (f&0xFF)==0 )
			faceBase[f>>8] = itemBase = itemCount;
		WORD offset = (WORD)(itemCount-itemBase);
		if( grouping.GetBits(f) & VertFaceGrouping::BIT_ISFACETED ) {
			grouping.SetBits( f,  VertFaceGrouping::BIT_ISFACETED | offset );
			itemCount++;
		}
	}

	// 4. Delete the group count data from the grouping object (conserves memory)
	grouping.Compact();

	this->numItems = itemCount;
	valid = TRUE;
}

void VNormalChannel::Free() {
	grouping.Free();
	faceBase.SetCount(0);
	vertBase.SetCount(0);
	vertOffset.SetCount(0);
	tangentBasisSet.SetCount(0);
	numItems = 0;
	valid = FALSE;
}

BOOL VNormalChannel::Valid() {return valid;}

void VNormalChannel::InitNormals( Mesh& mesh, Matrix3& tm ) {
	Tab<Point3> normalsSet; //temp array

	// 1. Allocate the tangent array
	normalsSet.SetCount( numItems );
	if( numItems>0 ) memset( normalsSet.Addr(0), 0, numItems*sizeof(Point3) );

	// 2. Loop through the faces, calculating the tangent for each
	int numFaces = mesh.getNumFaces();
	for( int f=0; f<numFaces; f++ ) {
		Point3 norm = mesh.getFaceNormal(f);
		Face& geomFace = mesh.faces[f];

		// 3a. For smoothed faces,
		// add the normal to the sums for the three verts, and normalize below
		if( !IsFaceted(f) ) {
			for( int fv=0; fv<3; fv++ ) {
				int v = geomFace.v[fv];
				int index = FindIndex( f, v, fv );
				normalsSet[index] += norm;
			}
		}
		// 3b. For faceted faces, store one face normal
		else {
			int index = FindIndex( f, 0, 0 );
			normalsSet[index] = norm;
	}
}

	// 4. Normalize the normals
	for( int i=0; i<numItems; i++ ) {
		Point3& norm = normalsSet[i];
		norm = tm.VectorTransform( norm );
		norm.Unify();
}

	// 5. Finalize the operation
	memswp( &(this->normalsSet), &normalsSet, sizeof(normalsSet) );
	validNormals = TRUE;
}

void VNormalChannel::InitTangentBasis( Mesh& mesh, Matrix3& tm, int mapChannel ) {
	int numFaces = mesh.getNumFaces();
	Face   *geomFaces = mesh.faces;
	TVFace *mapFaces = mesh.mapFaces( mapChannel );
	Point3 *geomVerts = mesh.verts, *mapVerts = mesh.mapVerts( mapChannel );
	Point3  geomTri[3], mapTri[3], basisVec[2];
	Tab<TangentBasis> tangentBasisSet; //temp array

	//Error checking
	if( (mapFaces==NULL) || (mapVerts==NULL) ) {
		tangentBasisSet.SetCount(0);
		validTangentBasis = TRUE;
		return;
}

	// 1. Allocate the tangent array
	tangentBasisSet.SetCount( numItems );
	if( numItems>0 ) memset( tangentBasisSet.Addr(0), 0, numItems*sizeof(TangentBasis) );

	// 2. Loop through the faces, calculating the tangent for each
	for( int f=0; f<numFaces; f++ ) {
		Face& geomFace = geomFaces[f];
		geomTri[0] = geomVerts[ geomFace.v[0] ];
		geomTri[1] = geomVerts[ geomFace.v[1] ];
		geomTri[2] = geomVerts[ geomFace.v[2] ];

		TVFace& mapFace = mapFaces[f];
		mapTri[0] = mapVerts[ mapFace.t[0] ];
		mapTri[1] = mapVerts[ mapFace.t[1] ];
		mapTri[2] = mapVerts[ mapFace.t[2] ];

		ComputeTangentAndBinormal( mapTri, geomTri, basisVec );

		Point3 mapNormal = FNormalize( (mapTri[1] - mapTri[0]) ^ (mapTri[2] - mapTri[1]) );
		if( mapNormal.z<0 ) basisVec[1] = -basisVec[1]; //is the UV face flipped? flip the binormal

		// 3a. For smoothed faces,
		// add the tangent to the sums for the three verts, and normalize below
		if( !IsFaceted(f) ) {
			for( int fv=0; fv<3; fv++ ) {
				int v = geomFace.v[fv];
				int index = FindIndex( f, v, fv );
				tangentBasisSet[index].uBasis += basisVec[0];
				tangentBasisSet[index].vBasis += basisVec[1];
			}
		}
		// 3b. For faceted faces, store one face tangent
		else {
			int index = FindIndex( f, 0, 0 );
			tangentBasisSet[index].uBasis = basisVec[0];
			tangentBasisSet[index].vBasis = basisVec[1];
		}
}

	// 4. Normalize the tangents
	for( int i=0; i<numItems; i++ ) {
		TangentBasis& bv = tangentBasisSet[i];
		bv.uBasis = tm.VectorTransform( bv.uBasis );
		bv.vBasis = tm.VectorTransform( bv.vBasis );
		bv.uBasis.Unify();
		bv.vBasis.Unify();
	}

	// 5. Finalize the operation
	memswp( &(this->tangentBasisSet), &tangentBasisSet, sizeof(tangentBasisSet) );
	validTangentBasis = TRUE;
	}

BOOL VNormalChannel::ValidTangentBasis() {
	return validTangentBasis;
}

BOOL VNormalChannel::ValidNormals() {
	return validNormals;
}

Point3& VNormalChannel::GetNormal( int faceIndex, int vertIndex, int faceVertIndex ) {
	int index = FindIndex( faceIndex, vertIndex, faceVertIndex );
	if( index>=normalsSet.Count() ) return stubNormal;
	return normalsSet[index];
}

TangentBasis& VNormalChannel::GetTangentBasis( int faceIndex, int vertIndex, int faceVertIndex ) {
	int index = FindIndex( faceIndex, vertIndex, faceVertIndex );
	if( index>=tangentBasisSet.Count() ) return stubTangentBasis;
	return tangentBasisSet[index];
}

BOOL VNormalChannel::IsFaceted( int faceIndex ) {
	WORD faceBits = grouping.GetBits(faceIndex);
	return (faceBits & VertFaceGrouping::BIT_ISFACETED?  TRUE:FALSE);
}

int VNormalChannel::FindIndex( int faceIndex, int vertIndex, int faceVertIndex ) {
	int index;
	WORD faceBits = grouping.GetBits( faceIndex );
	if( !IsFaceted(faceIndex) ) {
		int group = grouping.GetGroup( faceIndex, faceVertIndex );
		index = vertBase[vertIndex>>8] + vertOffset[vertIndex] + group;
	}
	else
		index = faceBase[faceIndex>>8] + (faceBits&0x7FFF);
	return index;
}



//===========================================================================
//
// Class VNormalBank
//
// Holds one VNormalChannel for each mapping channel of an object
//
//===========================================================================

VNormalBank::VNormalBank() { }

void VNormalBank::Free() {
	int count = channels.Count();
	for( int i=0; i<count; i++ )
		if( channels[i]!=NULL ) delete channels[i];
	channels.SetCount(0);
}

void VNormalBank::InitChannel( Mesh& mesh, int mapChannel ) {
	int m = mapChannel + NUM_HIDDENMAPS;
	VNormalChannel* null = NULL;

	int numAppend = ((m+1) - channels.Count());
	for( int i=0; i<numAppend; i++ )
		channels.Append( 1, &null );

	VNormalChannel* channel = channels[m];
	if( channel==NULL )
		channel = new VNormalChannel;

	if( !channel->Valid() )
		channel->Init( mesh, mapChannel );

	channels[m] = channel; //Finalize the operation
	}
		
BOOL VNormalBank::ValidChannel( int mapChannel ) {
	VNormalChannel* channel = GetChannel(mapChannel);
	return ((channel!=NULL) && channel->Valid()?  TRUE:FALSE);
	}

VNormalChannel* VNormalBank::GetChannel( int mapChannel ) {
	int m = mapChannel + NUM_HIDDENMAPS;
	if( m>=channels.Count() ) return NULL;
	return channels[m];
}



//===========================================================================
//
// Class VNormalMgr
//
// Holds a normal vector and related data at each vertex & smoothing group;
// that is, one piece of data per vertex per smoothing group at the vertex.
//
//===========================================================================

VNormalMgr theVNormalMgr;


BaseInterface* IVNormalMgr::GetInterface(Interface_ID id) {
	if( id==GetID() ) return this;
	return NULL;
}

VNormalMgr::VNormalMgr()
	: IVNormalMgr(
		IVNORMALMGR_INTERFACE_ID, _T("IVNormalMgr"), 0, NULL, FP_CORE | FP_TEST_INTERFACE,
		end
	)
{
}

VNormalMgr::~VNormalMgr() {
	Free();
}

void VNormalMgr::Free() {
	lock.EnterWrite();
	for( int i=0; i<banks.Count(); i++ )
		if( banks[i]!=NULL ) delete banks[i];
	banks.SetCount(0);
	lock.ExitWrite();
}

void VNormalMgr::InitChannel( Mesh& mesh, int nodeID, int mapChannel ) {
	lock.EnterWrite();
	InitChannel_Internal( mesh, nodeID, mapChannel );
	lock.ExitWrite();
}

void VNormalMgr::InitChannel_Internal( Mesh& mesh, int nodeID, int mapChannel ) {
	if( ValidChannel_Internal(nodeID,mapChannel) ) { // initialized while waiting on the semaphore?
		return;
	}

	VNormalBank* null = NULL;
	int numAppend = ((nodeID+1) - banks.Count());
	for( int i=0; i<numAppend; i++ )
		banks.Append( 1, &null );

	if( banks[nodeID]==NULL ) {
		banks[nodeID] = new VNormalBank;
		banks[nodeID]->Init();
	}
	banks[nodeID]->InitChannel( mesh, mapChannel );
}

BOOL VNormalMgr::ValidChannel( int nodeID, int mapChannel ) {
	return (GetChannel(nodeID,mapChannel)==NULL?  FALSE:TRUE);
}

BOOL VNormalMgr::ValidChannel_Internal( int nodeID, int mapChannel ) {
	return (GetChannel_Internal(nodeID,mapChannel)==NULL?  FALSE:TRUE);
}

VNormalChannel* VNormalMgr::GetChannel( int nodeID, int mapChannel ) {
	lock.EnterRead();
	VNormalChannel* retval = GetChannel_Internal(nodeID,mapChannel);
	lock.ExitRead();
	return retval;
}

VNormalChannel* VNormalMgr::GetChannel_Internal( int nodeID, int mapChannel ) {
	if( banks.Count()<=nodeID ) return FALSE;

	VNormalBank* bank = banks[nodeID];
	if( bank==NULL ) return NULL;

	return bank->GetChannel( mapChannel );
	}

//-- Convenience methods

void VNormalMgr::InitTangentBasis( ShadeContext& sc, int mapChannel ) {
	int nodeID = sc.NodeID();
	RenderInstance* inst = sc.globContext->GetRenderInstance(nodeID);
	if( (inst==NULL) || (inst->mesh==NULL) ) return;

	lock.EnterWrite();

	InitChannel_Internal( *(inst->mesh), nodeID, mapChannel );
	VNormalChannel* channel = GetChannel_Internal( nodeID, mapChannel );
	if( !channel->ValidTangentBasis() ) // initialized while waiting on the semaphore?
		 channel->InitTangentBasis( *(inst->mesh), inst->objToCam, mapChannel );

	lock.ExitWrite();
}

void VNormalMgr::GetTangentBasis( ShadeContext& sc, int mapChannel, TangentBasis& tangentBasis ) {
	DbgAssert( sc.globContext!=NULL );
	int nodeID = sc.NodeID();
	RenderInstance* inst = sc.globContext->GetRenderInstance(nodeID);
	if( (inst==NULL) || 
		 (inst->mesh==NULL) ||
		 NULL == inst->mesh->faces ||
		 inst->mesh->getNumFaces() <= sc.FaceNumber() )
	{
		return;
	} 

	lock.EnterRead();

	VNormalChannel* channel = GetChannel_Internal( nodeID, mapChannel );
	while( (channel==NULL) || (!channel->ValidTangentBasis()) ) {
		lock.ExitRead(); //release lock while initializing
		InitTangentBasis( sc, mapChannel );
		lock.EnterRead(); //regain lock after initializing
		channel = GetChannel_Internal( nodeID, mapChannel );
	}


	int faceIndex = sc.FaceNumber();
	Face& f = inst->mesh->faces[faceIndex];
	DWORD smGroup = f.smGroup;
	if( smGroup==0 )
		tangentBasis = channel->GetTangentBasis( faceIndex, 0, 0 );
	else {
		DWORD *v = f.v;
		TangentBasis& b0 = channel->GetTangentBasis( faceIndex, v[0], 0 ); //returned in camera space
		TangentBasis& b1 = channel->GetTangentBasis( faceIndex, v[1], 1 ); //returned in camera space
		TangentBasis& b2 = channel->GetTangentBasis( faceIndex, v[2], 2 ); //returned in camera space

		Point3 bary = sc.BarycentricCoords();
		tangentBasis.uBasis = ((bary.x*b0.uBasis) + (bary.y*b1.uBasis) + (bary.z*b2.uBasis)).Normalize();
		tangentBasis.vBasis = ((bary.x*b0.vBasis) + (bary.y*b1.vBasis) + (bary.z*b2.vBasis)).Normalize();
	}

	lock.ExitRead();
}

VNormalMgr* pVNormalMgr = NULL;
VNormalMgr* GetVNormalMgr() {
	if( pVNormalMgr==NULL ) {
		pVNormalMgr = (VNormalMgr*)GetCOREInterface( IVNORMALMGR_INTERFACE_ID );
		if( pVNormalMgr==NULL ) pVNormalMgr = new VNormalMgr;
	}

	return pVNormalMgr;
}

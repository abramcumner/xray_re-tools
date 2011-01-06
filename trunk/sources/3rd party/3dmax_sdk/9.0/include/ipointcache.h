//-------------------------------------------------------------
// Access to Point Cache
//


#ifndef __IPOINTCACHE__H
#define __IPOINTCACHE__H

#include "iFnPub.h"

//new class IDs

#define POINTCACHEOSM_CLASS_ID	Class_ID(0x270f1fe3, 0x3b14999)
#define POINTCACHEWSM_CLASS_ID	Class_ID(0x370f1fe3, 0x3b14999)

#define POINTCACHE_CLASS POINTCACHEOSM_CLASS_ID

//deprecated class IDS..
#define OLDPOINTCACHE_CLASS_ID	Class_ID(0x21d07ae1, 0x48d30bec)
#define OLDPOINTCACHEWSM_CLASS_ID	Class_ID(0x21d07ae1, 0x48d30bed)
#define OLDPARTICLECACHE_CLASS_ID	Class_ID(0x21d07ae1, 0x48d30bee)




class IPointCache;
class IPointCacheWSM;
class IParticleCache;

//***************************************************************
//Function Publishing System stuff   
//****************************************************************
#define POINTCACHE_INTERFACE Interface_ID(0x53b4409b, 0x18ee7cc8)

#define GetIPointCacheInterface(cd) \
			(IPointCache *)(cd)->GetInterface(POINTCACHE_INTERFACE)

#define POINTCACHEWSM_INTERFACE Interface_ID(0x53b4409b, 0x18ee7cc9)

#define GetIPointCacheWSMInterface(cd) \
			(IPointCacheWSM *)(cd)->GetInterface(POINTCACHEWSM_INTERFACE)

#define PARTICLECACHE_INTERFACE Interface_ID(0x53b4409b, 0x18ee7cd0)

#define GetIParticleCacheInterface(cd) \
			(IParticleCache *)(cd)->GetInterface(PARTICLECACHE_INTERFACE)

enum {  pointcache_record, pointcache_setcache, pointcache_enablemods,pointcache_disablemods


		};
//****************************************************************


class IPointCache :  public FPMixinInterface 
	{
	public:

		//Function Publishing System
		//Function Map For Mixin Interface
		//*************************************************
		BEGIN_FUNCTION_MAP
			VFN_0(pointcache_record, fnRecord);
			VFN_0(pointcache_setcache, fnSetCache);
			VFN_0(pointcache_enablemods, fnEnableMods);
			VFN_0(pointcache_disablemods, fnDisableMods);

		END_FUNCTION_MAP

		virtual FPInterfaceDesc* GetDesc()=0;    // <-- must implement 

		virtual void	fnRecord()=0;
		virtual void	fnSetCache()=0;
		virtual void	fnEnableMods()=0;
		virtual void	fnDisableMods()=0;

	};


class IPointCacheWSM :  public FPMixinInterface 
	{
	public:

		//Function Publishing System
		//Function Map For Mixin Interface
		//*************************************************
		BEGIN_FUNCTION_MAP
			VFN_0(pointcache_record, fnRecord);
			VFN_0(pointcache_setcache, fnSetCache);
			VFN_0(pointcache_enablemods, fnEnableMods);
			VFN_0(pointcache_disablemods, fnDisableMods);

		END_FUNCTION_MAP

		virtual FPInterfaceDesc* GetDesc()=0;    // <-- must implement 

		virtual void	fnRecord()=0;
		virtual void	fnSetCache()=0;
		virtual void	fnEnableMods()=0;
		virtual void	fnDisableMods()=0;

	};

class IParticleCache :  public FPMixinInterface 
	{
	public:

		//Function Publishing System
		//Function Map For Mixin Interface
		//*************************************************
		BEGIN_FUNCTION_MAP
			VFN_0(pointcache_record, fnRecord);
			VFN_0(pointcache_setcache, fnSetCache);

		END_FUNCTION_MAP

		virtual FPInterfaceDesc* GetDesc()=0;    // <-- must implement 

		virtual void	fnRecord()=0;
		virtual void	fnSetCache()=0;

	};


#endif // __IPOINTCACHE__H

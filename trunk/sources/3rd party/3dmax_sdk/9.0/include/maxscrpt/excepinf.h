#ifndef _H_EXCEPINF
#define _H_EXCEPINF

// Helper class wrapping EXCEPINFO
// ctor initializes EXCEPINFO fields to null
// dtor calls Clear(), which calls SysFreeString on non-null BSTR fields
class CExcepInfo :
   public EXCEPINFO
{
public:
   ScripterExport CExcepInfo();
   ScripterExport ~CExcepInfo();

   ScripterExport void Clear();
};

#endif

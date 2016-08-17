#include "StdAfx.h"
#include "Surface.h"
#include "Reader.h"

namespace xrFSL
{
	//************************************
	// Method:    Surface
	// FullName:  xrFSL::Surface::Surface
	// Access:    public 
	// Returns:   
	// Qualifier: 
	// Parameter: bool bSkeletal
	//************************************
	Surface::Surface( bool bSkeletal /*= false*/ ):
	m_sEShader(bSkeletal ? "models\\model" : "default"),
	m_sCShader("default"),
	m_sGameMtl(bSkeletal ? "default_object" : "default"),
	m_sVmap("Texture"),
	m_uFlags(0),
	m_uFvf(ESURFACE_DEFAULT_FVF)
	{

	}

	//************************************
	// Method:    ~Surface
	// FullName:  xrFSL::Surface::~Surface
	// Access:    public 
	// Returns:   // Qualifier:
	// Parameter: void
	//************************************
	Surface::~Surface(void)
	{
	}

	//************************************
	// Method:    Load_0
	// FullName:  xrFSL::Surface::Load_0
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void Surface::Load_0( Reader& r )
	{
		r.r_sz(m_sName);
		r.r_sz(m_sEShader);

		m_sCShader = "default";
		m_sGameMtl = "default";

		if (r.r_bool())
			m_uFlags = ESF_TWO_SIDED;

		m_uFvf = r.r_u32();

		unsigned tc = r.r_u32();
		ASSERT_F(tc <= 1, (""));

		r.r_sz(m_sTexture);
		r.r_sz(m_sVmap);
	}

	//************************************
	// Method:    Load_1
	// FullName:  xrFSL::Surface::Load_1
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void Surface::Load_1( Reader& r )
	{
		r.r_sz(m_sName);
		r.r_sz(m_sEShader);
		r.r_sz(m_sCShader);
		m_sGameMtl = "default";
		r.r_sz(m_sTexture);
		r.r_sz(m_sVmap);

		m_uFlags = r.r_u32();
		m_uFvf = r.r_u32();

		unsigned tc = r.r_u32();
		ASSERT_F(tc <= 1, (""));
	}

	//************************************
	// Method:    Load_2
	// FullName:  xrFSL::Surface::Load_2
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void Surface::Load_2( Reader& r )
	{
		r.r_sz(m_sName);
		r.r_sz(m_sEShader);
		r.r_sz(m_sCShader);
		r.r_sz(m_sGameMtl);
		r.r_sz(m_sTexture);
		r.r_sz(m_sVmap);

		m_uFlags = r.r_u32();
		m_uFvf = r.r_u32();

		unsigned tc = r.r_u32();
		ASSERT_F(tc <= 1, (""));
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::Surface::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void Surface::Save( Writer& w ) const
	{
		w.w_sz(m_sName);
		w.w_sz(m_sEShader);
		w.w_sz(m_sCShader);
		w.w_sz(m_sGameMtl);
		w.w_sz(m_sTexture);
		w.w_sz(m_sVmap);
		w.w_u32(m_uFlags);
		w.w_u32(m_uFvf);
		w.w_u32(1);
	}
}

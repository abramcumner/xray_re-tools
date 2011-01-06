/*	
 *		nurbspro.h - MAX NURBS protocols
 *
 *			Copyright © Autodesk, Inc. 1997
 *			Author: John Wainwright
 *
 */

	def_visible_generic_debug_ok	( evalPos,			"evalPos" );
	def_visible_generic_debug_ok	( evalTangent,		"evalTangent" );
//	def_visible_generic_debug_ok	( getTrimPoint,		"getTrimPoint" );
	def_visible_generic_debug_ok	( getKnot,			"getKnot" );
	def_visible_generic				( setKnot,			"setKnot" );
	def_visible_generic_debug_ok	( getCV,			"getCV" );
	def_visible_generic				( setCV,			"setCV" );
	def_visible_generic				( refine,			"refine" );
	def_visible_generic_debug_ok	( getPoint,			"getPoint" );
	def_visible_generic				( setPoint,			"setPoint" );
	def_visible_generic_debug_ok	( evalUTangent,		"evalUTangent" );
	def_visible_generic_debug_ok	( evalVTangent,		"evalVTangent" );
	def_visible_generic				( setTiling,		"setTiling" );
	def_visible_generic_debug_ok	( getTiling,		"getTiling" );
	def_visible_generic				( setTilingOffset,	"setTilingOffset" );
	def_visible_generic_debug_ok	( getTilingOffset,	"getTilingOffset" );
	def_visible_generic				( setTextureUVs,	"setTextureUVs" );
	def_visible_generic_debug_ok	( getTextureUVs,	"getTextureUVs" );
	def_visible_generic				( setGenerateUVs,	"setGenerateUVs" );
	def_visible_generic_debug_ok	( getGenerateUVs,	"getGenerateUVs" );
	def_visible_generic				( closeU,			"closeU" );
	def_visible_generic				( closeV,			"closeV" );
	def_visible_generic_debug_ok	( getUKnot,			"getUKnot" );
	def_visible_generic_debug_ok	( getVKnot,			"getVKnot" );
	def_visible_generic				( setUKnot,			"setUKnot" );
	def_visible_generic				( setVKnot,			"setVKnot" );
	def_visible_generic				( refineU,			"refineU" );
	def_visible_generic				( refineV,			"refineV" );
	def_visible_generic				( appendCurve,		"appendCurve" );
	def_visible_generic				( appendCurveByID,  "appendCurveByID" );
	def_visible_generic_debug_ok	( getCurve,			"getCurve" );
	def_visible_generic				( setCurve,			"setCurve" );
	def_visible_generic_debug_ok	( getCurveID,		"getCurveID" );
	def_visible_generic				( setCurveByID,		"setCurveByID" );
	def_visible_generic_debug_ok	( getFlip,			"getFlip" );
	def_visible_generic				( setFlip,			"setFlip" );
	def_visible_generic_debug_ok	( getObject,		"getObject" );
	def_visible_generic				( setObject,		"setObject" );
	def_visible_generic				( appendObject,		"appendObject" );
	def_visible_generic				( removeObject,		"removeObject" );
	def_visible_generic				( deleteObjects,	"deleteObjects" );
	def_visible_generic				( reparameterize, "reparameterize" );

	def_visible_generic_debug_ok	( getParent,		"getParent" );
	def_visible_generic_debug_ok	( getParentID,		"getParentID" );
	def_visible_generic				( setParent,		"setParent" );
	def_visible_generic				( setParentID,		"setParentID" );
	def_visible_generic_debug_ok	( getEdge,			"getEdge" );
	def_visible_generic				( setEdge,			"setEdge" );
	def_visible_generic				( appendUCurve,		"appendUCurve" );
	def_visible_generic				( appendUCurveByID,	"appendUCurveByID" );
	def_visible_generic_debug_ok	( getUCurve,		"getUCurve" );
	def_visible_generic_debug_ok	( getUCurveID,		"getUCurveID" );
	def_visible_generic				( setUCurve,		"setUCurve" );
	def_visible_generic				( setUCurveByID,	"setUCurveByID" );
	def_visible_generic				( appendVCurve,		"appendVCurve" );
	def_visible_generic				( appendVCurveByID,	"appendVCurveByID" );
	def_visible_generic_debug_ok	( getVCurve,		"getVCurve" );
	def_visible_generic_debug_ok	( getVCurveID,		"getVCurveID" );
	def_visible_generic				( setVCurve,		"setVCurve" );
	def_visible_generic				( setVCurveByID,	"setVCurveByID" );
	def_visible_generic				( disconnect,		"disconnect" );
	def_visible_generic				( setSeed, 	"setSeed" );
	def_visible_generic_debug_ok	( getSeed, 	"getSeed" );
	def_visible_generic_debug_ok	( getRadius, "getRadius" );
	def_visible_generic				( setRadius, "setRadius" );
	def_visible_generic_debug_ok	( getTrimSurface,	"getTrimSurface" );
	def_visible_generic				( setTrimSurface,	"setTrimSurface" );
	def_visible_generic_debug_ok	( getFlipTrim, "getFlipTrim" );
	def_visible_generic				( setFlipTrim, "setFlipTrim" );

	def_visible_generic				( setTextureSurface, "setTextureSurface" );
	def_visible_generic_debug_ok	( getTextureSurface, "getTextureSurface" );

	def_visible_generic_debug_ok	( getProdTess,		"getProdTess" );
	def_visible_generic				( setProdTess,		"setProdTess" );
	def_visible_generic_debug_ok	( getViewTess,		"getViewTess" );
	def_visible_generic				( setViewTess,		"setViewTess" );
	def_visible_generic				( clearViewTess,	"clearViewTess" );
	def_visible_generic				( clearProdTess,	"clearProdTess" );

	def_visible_generic				( setCurveStartPoint,	"setCurveStartPoint" );
	def_visible_generic_debug_ok	( getCurveStartPoint,	"getCurveStartPoint" );

	def_visible_primitive			( NURBSLatheSurface,		"MakeNURBSLatheSurface" );
	def_visible_primitive			( NURBSSphereSurface,		"MakeNURBSSphereSurface" );
	def_visible_primitive			( NURBSCylinderSurface,	"MakeNURBSCylinderSurface" );
	def_visible_primitive			( NURBSConeSurface,		"MakeNURBSConeSurface" );
	def_visible_primitive			( NURBSTorusSurface,		"MakeNURBSTorusSurface" );

	def_visible_primitive			( NURBSNode,				"NURBSNode" );
	def_visible_primitive			( NURBSLatheNode,			"NURBSLatheNode" );
	def_visible_primitive			( NURBSExtrudeNode,		"NURBSExtrudeNode" );

	def_visible_primitive			( updateSurfaceMapper,	"updateSurfaceMapper" );

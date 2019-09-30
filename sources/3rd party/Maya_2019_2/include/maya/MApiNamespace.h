#ifndef _MApiNamespace
#define _MApiNamespace

//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// FILE:    MApiNamespace.h
//
// ****************************************************************************
//
// DESCRIPTION (MApiNamespace)
//
//		This file contains the definitions of the Open Maya namespace
//
// ****************************************************************************

#if defined __cplusplus


// ****************************************************************************

// DECLARATIONS

// ****************************************************************************


#ifdef _WIN32

#ifndef FND_EXPORT
#define FND_EXPORT _declspec( dllimport ) 
#endif // FND_EXPORT

#ifndef OPENMAYA_EXPORT
#define OPENMAYA_EXPORT _declspec( dllimport )
#endif // OPENMAYA_EXPORT

#ifndef OPENMAYAUI_EXPORT
#define OPENMAYAUI_EXPORT _declspec( dllimport )
#endif // OPENMAYAUI_EXPORT

#ifndef OPENMAYAANIM_EXPORT
#define OPENMAYAANIM_EXPORT _declspec( dllimport )
#endif // OPENMAYAANIM_EXPORT

#ifndef OPENMAYAFX_EXPORT
#define OPENMAYAFX_EXPORT _declspec( dllimport )
#endif // OPENMAYAFX_EXPORT

#ifndef OPENMAYARENDER_EXPORT
#define OPENMAYARENDER_EXPORT _declspec( dllimport )
#endif // OPENMAYARENDER_EXPORT

#endif // _WIN32



#ifndef SWIG
// ****************************************************************************

// NAMESPACE MACRO DEFINITIONS

// ****************************************************************************

#define AUTODESK_NAMESPACE Autodesk
#define MAYA_NAMESPACE Maya
#define OPENMAYA_NAMESPACE_OPEN(API_VERSION) \
namespace AUTODESK_NAMESPACE{ \
	namespace MAYA_NAMESPACE{ \
		namespace API_VERSION{
#define OPENMAYA_NAMESPACE_CLOSE }}}
#define OPENMAYA_NAMESPACE_SCHEMA(API_VERSION) \
AUTODESK_NAMESPACE::MAYA_NAMESPACE::API_VERSION

// Namespace for Major release

#define OPENMAYA_MAJOR_NAMESPACE OpenMaya20190000
#define OPENMAYA_MAJOR_NAMESPACE_OPEN OPENMAYA_NAMESPACE_OPEN(OPENMAYA_MAJOR_NAMESPACE)
#define OPENMAYA_MAJOR_NAMESPACE_SCHEMA OPENMAYA_NAMESPACE_SCHEMA(OPENMAYA_MAJOR_NAMESPACE)

#else /* SWIG */

#define AUTODESK_NAMESPACE
#define MAYA_NAMESPACE
#define OPENMAYA_NAMESPACE_OPEN(API_VERSION)
#define OPENMAYA_NAMESPACE_CLOSE
#define OPENMAYA_NAMESPACE_SCHEMA(API_VERSION)

#define OPENMAYA_MAJOR_NAMESPACE
#define OPENMAYA_MAJOR_NAMESPACE_OPEN
#define OPENMAYA_MAJOR_NAMESPACE_SCHEMA

#endif /* SWIG */

// ****************************************************************************

// CLASS DEFINITIONS - FORWARD DECLARATIONS

// ****************************************************************************

// Forward declarations for Major release

OPENMAYA_MAJOR_NAMESPACE_OPEN

	class MFn;
	class MObject;
	class MPxEditData;
	class MSpace;
    class MAnimControl;
    class MAnimCurveChange;
    class MAnimCurveClipboard;
    class MAnimCurveClipboardItem;
    class MAnimCurveClipboardItemArray;
    class MAnimMessage;
    class MAnimUtil;
    class MFnAnimCurve;
    class MFnBlendShapeDeformer;
    class MFnCharacter;
    class MFnClip;
    class MFnGeometryFilter;
    class MFnHikEffector;
    class MFnIkEffector;
    class MFnIkHandle;
    class MFnIkJoint;
    class MFnIkSolver;
    class MFnKeyframeDelta;
    class MFnKeyframeDeltaAddRemove;
    class MFnKeyframeDeltaBlockAddRemove;
    class MFnKeyframeDeltaBreakdown;
    class MFnKeyframeDeltaInfType;
    class MFnKeyframeDeltaMove;
    class MFnKeyframeDeltaScale;
    class MFnKeyframeDeltaTangent;
    class MFnKeyframeDeltaWeighted;
    class MFnLattice;
    class MFnLatticeDeformer;
    class MFnMotionPath;
    class MFnSkinCluster;
    class MFnWeightGeometryFilter;
    class MFnWireDeformer;
    class MGPUDeformerRegistrationInfo;
    class MGPUDeformerRegistry;
    class MIkHandleGroup;
    class MIkSystem;
    class MItKeyframe;
    class MPxAnimCurveInterpolator;
    class MPxBlendShape;
    class MPxCacheConfigRuleFilter;
    class MPxConstraint;
    class MPxConstraintCommand;
    class MPxDeformerNode;
    class MPxGeometryFilter;
    class MGPUDeformerBuffer;
    class MGPUDeformerData;
    class MPxGPUDeformer;
    class MPxIkSolver;
    class MPxIkSolverNode;
    class MPxMotionPathNode;
    class MPxSkinCluster;
    class MDynamicsUtil;
    class MDynSweptLine;
    class MDynSweptTriangle;
    class MFnAirField;
    class MFnDragField;
    class MFnDynSweptGeometryData;
    class MFnField;
    class MFnFluid;
    class MFnGravityField;
    class MFnInstancer;
    class MFnNewtonField;
    class MFnNIdData;
    class MFnNObjectData;
    class MFnParticleSystem;
    class MFnPfxGeometry;
    class MFnRadialField;
    class MFnTurbulenceField;
    class MFnUniformField;
    class MFnVolumeAxisField;
    class MFnVortexField;
    class MHairSystem;
    class MnCloth;
    class MnObject;
    class MnParticle;
    class MnRigid;
    class MnSolver;
    class MPxEmitterNode;
    class MPxFieldNode;
    class MPxFluidEmitterNode;
    class MPxParticleAttributeMapperNode;
    class MPxSpringNode;
    class MRenderLine;
    class MRenderLineArray;
    class MAddRemoveAttrEdit;
    class MAngle;
    class MArgDatabase;
    class MArgList;
    class MArgParser;
    class MArrayDataBuilder;
    class MArrayDataHandle;
    class MAtomic;
    class MAttributeIndex;
    class MAttributePattern;
    class MAttributePatternArray;
    class MAttributeSpec;
    class MAttributeSpecArray;
    class MBoundingBox;
    class MCacheConfigRuleRegistry;
    class MCacheFormatDescription;
    class MCallbackIdArray;
    class MCameraSetMessage;
    class MColor;
    class MColorArray;
    class MCommandMessage;
    class MCommandResult;
    class MCommonSystemUtils;
    class MComputation;
    class MConditionMessage;
    class MConnectDisconnectAttrEdit;
    class MContainerMessage;
    class MCurveAttribute;
    class MCustomEvaluatorClusterNode;
    class MDagMessage;
    class MDagModifier;
    class MDAGDrawOverrideInfo;
    class MDagPath;
    class MDagPathArray;
    class MDataBlock;
    class MDataHandle;
    class MDGContext;
    class MDGContextGuard;
    class MDGMessage;
    class MDGModifier;
    class MDistance;
    class MDoubleArray;
    class MEdit;
    class MEulerRotation;
    class MEvaluationGraph;
    class MEvaluationManager;
    class MEvaluationNode;
    class MEvaluationNodeIterator;
    class MEventMessage;
    class MExternalContentInfoTable;
    class MExternalContentLocationTable;
    class MFcurveEdit;
    class MFileIO;
    class MFileObject;
    class MFloatArray;
    class MFloatMatrix;
    class MFloatPoint;
    class MFloatPointArray;
    class MFloatVector;
    class MFloatVectorArray;
    class MFnAmbientLight;
    class MFnAnisotropyShader;
    class MFnAreaLight;
    class MFnArrayAttrsData;
    class MFnAssembly;
    class MFnAttribute;
    class MFnBase;
    class MFnBlinnShader;
    class MFnCamera;
    class MFnCameraSet;
    class MFnComponent;
    class MFnComponentListData;
    class MFnCompoundAttribute;
    class MFnContainerNode;
    class MFnDagNode;
    class MFnData;
    class MFnDependencyNode;
    class MFnDirectionalLight;
    class MFnDoubleArrayData;
    class MFnDoubleIndexedComponent;
    class MFnEnumAttribute;
    class MFnExpression;
    class MFnFloatArrayData;
    class MFnGenericAttribute;
    class MFnGeometryData;
    class MFnIntArrayData;
    class MFnLambertShader;
    class MFnLatticeData;
    class MFnLayeredShader;
    class MFnLight;
    class MFnLightDataAttribute;
    class MFnMatrixArrayData;
    class MFnMatrixAttribute;
    class MFnMatrixData;
    class MFnMesh;
    class MMeshIsectAccelParams;
    class MFnMeshData;
    class MFnMeshOperation;
    class MFnMessageAttribute;
    class MFnNonAmbientLight;
    class MFnNonExtendedLight;
    class MFnNumericAttribute;
    class MFnNumericData;
    class MFnNurbsCurve;
    class MFnNurbsCurveData;
    class MFnNurbsSurface;
    class MFnNurbsSurfaceData;
    class MFnPartition;
    class MFnPhongEShader;
    class MFnPhongShader;
    class MFnPlugin;
    class MFnPluginData;
    class MFnPointArrayData;
    class MFnPointLight;
    class MFnReference;
    class MFnReflectShader;
    class MFnScPlugin;
    class MFnSet;
    class MFnSingleIndexedComponent;
    class MFnSphereData;
    class MFnSpotLight;
    class MFnStringArrayData;
    class MFnStringData;
    class MFnSubd;
    class MFnSubdData;
    class MFnSubdNames;
    class MFnTransform;
    class MFnTripleIndexedComponent;
    class MFnTypedAttribute;
    class MFnUInt64ArrayData;
    class MFnUint64SingleIndexedComponent;
    class MFnUnitAttribute;
    class MFnVectorArrayData;
    class MFnVolumeLight;
    class MGlobal;
    class MGraphNodeIterator;
    class MIffFile;
    class MIffTag;
    class MImage;
    class MImageFileInfo;
    class MInt64Array;
    class MIntArray;
    class MItCurveCV;
    class MItDag;
    class MItDependencyGraph;
    class MItDependencyNodes;
    class MItEdits;
    class MIteratorType;
    class MItGeometry;
    class MItInstancer;
    class MItMeshEdge;
    class MItMeshFaceVertex;
    class MItMeshPolygon;
    class MItMeshVertex;
    class MItSelectionList;
    class MItSubdEdge;
    class MItSubdFace;
    class MItSubdVertex;
    class MItSurfaceCV;
    class MLibrary;
    class MLockMessage;
    class MMatrix;
    class MMatrixArray;
    class MMeshIntersector;
    class MPointOnMesh;
    class MMeshSmoothOptions;
    class MMessage;
    struct MMessageNode;
    class MModelMessage;
    class MMutexLock;
    class MNamespace;
    class MNodeClass;
    class MNodeMessage;
    class MNurbsIntersector;
    class MPointOnNurbs;
    class MObjectArray;
    class MObjectHandle;
    class MObjectSetMessage;
    class MAutoCLKernel;
    class MAutoCLMem;
    class MAutoCLEvent;
    class MOpenCLBuffer;
    class MOpenCLInfo;
    class MParentingEdit;
    class MPlane;
    class MPlug;
    class MPlugArray;
    class MPoint;
    class MPointArray;
    class MPolyMessage;
    class MProfiler;
    class MProfilingScope;
    class MPxAssembly;
    class MPxAttributePatternFactory;
    class MPxCacheFormat;
    class MPxCameraSet;
    class MPxCommand;
    class MPxComponentShape;
    class MPxCustomEvaluator;
    class MPxData;
    class MPxFileResolver;
    class MPxFileTranslator;
    class MPxGeometryData;
    class MPxGeometryIterator;
    class MPxImageFile;
    class MPxImagePlane;
    class MPxMayaAsciiFilterOutput;
    class MPxMayaAsciiFilter;
    class MPxMultiPolyTweakUVCommand;
    class MPxNode;
    class MPxObjectSet;
    class MPxPolyTrg;
    class MPxPolyTweakUVCommand;
    class MPxRepresentation;
    class MPxSurfaceShape;
    class MPxTopologyEvaluator;
    class MPxTransform;
	class MPxTransform_BoundingBox;
    class MPxTransformationMatrix;
    class MQuaternion;
    class MRampAttribute;
    class MRichSelection;
    class MSceneMessage;
    class MScriptUtil;
    class MSelectionList;
    class MSelectionMask;
    class MSetAttrEdit;
    class MSpinLock;
    class MStatus;
    class MStreamUtils;
    class MString;
    class MStringArray;
    class MStringResource;
    class MStringResourceId;
    class MSyntax;
    class MTesselationParams;
    class MThreadAsync;
    class MThreadPool;
    class MThreadUtils;
    class MTime;
    class MTimeArray;
    class MTimer;
    class MTimerMessage;
    class MTopologyClusterNode;
    class MTransformationMatrix;
    class MTrimBoundaryArray;
    class MTypeId;
    class MUint64Array;
    class MUintArray;
    class MUniqueString;
    class MURI;
    class MUserData;
    class MUserEventMessage;
    class MUuid;
    class MVector;
    class MVectorArray;
    class MWeight;
    class MaterialInputData;
    class MPxBakeEngine;
    class MPxMaterialInformation;
    class MPxRenderPassImpl;
    class MRenderPassDef;
    class MRenderPassRegistry;
	class M3dView;
	class MAnimStream;
	class MCursor;
	class MDeviceChannel;
	class MDeviceState;
	class MDrawData;
	class MDrawInfo;
	class MDrawRequest;
	class MDrawRequestQueue;
	class MDrawTraversal;
	class MEvent;
	class MExternalDropCallback;
	class MExternalDropData;
	class MFeedbackLine;
	class MFnCircleSweepManip;
	class MFnCurveSegmentManip;
	class MFnDirectionManip;
	class MFnDiscManip;
	class MFnDistanceManip;
	class MFnFreePointTriadManip;
	class MFnManip3D;
	class MFnPointOnCurveManip;
	class MFnPointOnSurfaceManip;
	class MFnRotateManip;
	class MFnScaleManip;
	class MFnStateManip;
	class MFnToggleManip;
	class MFnToolContext;
	class MGraphEditorInfo;
	class MHWShaderSwatchGenerator;
	class MManipData;
	class MMaterial;
	class MMaterialArray;
	class MObjectListFilter;
	class MPaintMessage;
	class MProgressWindow;
	class MPx3dModelView;
	class MPxClientDeviceNode;
	class MPxContext;
	class MPxContextCommand;
	class MPxControlCommand;
	class MPxDragAndDropBehavior;
	class MPxFileDialog;
	class MPxGlBuffer;
	class MPxHardwareShader;
	class MPxHwShaderNode;
	class MPxLocatorNode;
	class MPxManipContainer;
	class MPxManipulatorNode;
	class MPxMidiInputDevice;
	class MPxModelEditorCommand;
	class MPxPolyTweakUVInteractiveCommand;
	class MPxSelectionContext;
	class MPxSurfaceShapeUI;
	class MPxTexContext;
	class MPxThreadedDeviceNode;
	class MCharBuffer;
	class MPxToolCommand;
	class MPxUIControl;
	class MPxUITableControl;
	class MQtUtil;
	class MSelectInfo;
	class MTextureEditorDrawInfo;
	class MToolsInfo;
	class MUiMessage;
	class MColorManagementUtilities;
    class MColorManagementNodes;
    class MColorPickerUtilities;
    class MColorPickerCallback;
	class MCommonRenderSettingsData;
	class MD3D9Renderer;
	class MDrawProcedureBase;
	class MFnAdskMaterial;
	class MFnImageSource;
	class MFnRenderLayer;
	class MFnRenderPass;
	class MGeometryLegacy;
	class MGeometryData;
	class MGeometryList;
	class MGeometryManager;
	class MGeometryPrimitive;
	class MGeometryRequirementsLegacy;
	class MGLFunctionTable;
	class MHardwareRenderer;
	class MHwrCallback;
	class MHwTextureManager;
	class MLightLinks;
	class MTestGLvoidPtr;
	class MPxRenderer;
	class MRenderCallback;
	class MRenderData;
	class MRenderingInfo;
	class MRenderProfile;
	class MRenderSetup;
	class MRenderSetupPrivate;
	class MRenderShadowData;
	class MRenderTargetLegacy;
	class MRenderUtil;
	class MRenderView;
	class MSwatchRenderBase;
	class MSwatchRenderRegister;
	class MUniformParameter;
	class MUniformParameterList;
	class MVaryingParameter;
	class MVaryingParameterList;
	class MViewportRenderer;

	namespace MHWRender
	{
		class MComponentDataIndexing;
		class MComponentDataIndexingList;
		class MDrawContext;
		class MPassContext;
		class MLightParameterInformation;
		class MDrawRegistry;
		class MFragmentManager;
		class MFrameContext;
		class MGeometryExtractor;
		class MVertexBufferDescriptor;
		class MVertexBufferDescriptorList;
		class MVertexBuffer;
		class MVertexBufferArray;
		class MIndexBufferDescriptor;
		class MIndexBufferDescriptorList;
		class MIndexBuffer;
		class MGeometryIndexMapping;
		class MRenderItem;
		class MRenderItemList;
		class MGeometry;
		class MGeometryRequirements;
		class MGeometryUtilities;
		class MPxComponentConverter;
		class MPxDrawOverride;
		class MPxGeometryOverride;
		class MPxIndexBufferMutator;
		class MPxImagePlaneOverride;
		class MPxPrimitiveGenerator;
		class MPxShaderOverride;
		class MAttributeParameterMapping;
		class MAttributeParameterMappingList;
		class MPxShadingNodeOverride;
		class MSubSceneContainer;
		class MPxSubSceneOverride;
		class MPxSurfaceShadingNodeOverride;
		class MPxVertexBufferGenerator;
		class MPxVertexBufferMutator;
		class MRenderParameters;
		class MRenderTargetDescription;
		class MRenderTarget;
		class MRenderTargetManager;
		class MRenderUtilities;
		class MSelectionContext;
		class MIntersection;
		class MSelectionInfo;
		class MShaderInstance;
		class MShaderManager;
		struct MTextureAssignment;
		struct MRenderTargetAssignment;
		struct MShaderCompileMacro;
		class MStateManager;
		class MBlendState;
		class MTargetBlendDesc;
		class MBlendStateDesc;
		class MRasterizerState;
		class MRasterizerStateDesc;
		class MDepthStencilState;
		class MStencilOpDesc;
		class MDepthStencilStateDesc;
		class MSamplerState;
		class MSamplerStateDesc;
		class MTextureDescription;
		class MDepthNormalizationDescription;
		class MTextureUpdateRegion;
		class MTexture;
		class MTextureArguments;
		class MTextureManager;
		class MUIDrawManager;
		class MCameraOverride;
		class MRenderOperation;
		class MRenderOperationList;
		class MUserRenderOperation;
		class MHUDRender;
		class MPresentTarget;
		class MClearOperation;
		class MSceneRender;
		class MQuadRender;
		class MRenderOverride;
		class MRenderScriptCallback;
		class MRenderer;
	}

	//MNativeWindowHdl : MNativeWindowHdl.h does not contain any api class
	//MGLdefinitions : MGLdefinitions.h does not contain any api class

OPENMAYA_NAMESPACE_CLOSE

#ifndef SWIG

		// ****************************************************************************

		// BUILDING API VERSIONS

		// ****************************************************************************

#define USING_MAJOR_ONLY_API \
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFn;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MObject;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxEditData;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSpace;								\
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimControl;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimCurveChange;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimCurveClipboard;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimCurveClipboardItem;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimCurveClipboardItemArray;       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimMessage;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimUtil;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAnimCurve;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnBlendShapeDeformer;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCharacter;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnClip;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnGeometryFilter;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnHikEffector;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnIkEffector;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnIkHandle;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnIkJoint;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnIkSolver;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDelta;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaAddRemove;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaBlockAddRemove;     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaBreakdown;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaInfType;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaMove;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaScale;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaTangent;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnKeyframeDeltaWeighted;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLattice;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLatticeDeformer;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMotionPath;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSkinCluster;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnWeightGeometryFilter;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnWireDeformer;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGPUDeformerRegistrationInfo;       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGPUDeformerRegistry;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIkHandleGroup;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIkSystem;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItKeyframe;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxAnimCurveInterpolator;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxBlendShape;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxCacheConfigRuleFilter;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxConstraint;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxConstraintCommand;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxDeformerNode;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxGeometryFilter;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGPUDeformerBuffer;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGPUDeformerData;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxGPUDeformer;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxIkSolver;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxIkSolverNode;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMotionPathNode;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxSkinCluster;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDynamicsUtil;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDynSweptLine;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDynSweptTriangle;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAirField;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDragField;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDynSweptGeometryData;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnField;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnFluid;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnGravityField;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnInstancer;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNewtonField;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNIdData;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNObjectData;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnParticleSystem;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPfxGeometry;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnRadialField;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnTurbulenceField;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnUniformField;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnVolumeAxisField;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnVortexField;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHairSystem;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MnCloth;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MnObject;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MnParticle;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MnRigid;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MnSolver;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxEmitterNode;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxFieldNode;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxFluidEmitterNode;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxParticleAttributeMapperNode;     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxSpringNode;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderLine;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderLineArray;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAddRemoveAttrEdit;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAngle;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MArgDatabase;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MArgList;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MArgParser;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MArrayDataBuilder;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MArrayDataHandle;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAtomic;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAttributeIndex;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAttributePattern;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAttributePatternArray;             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAttributeSpec;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAttributeSpecArray;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MBoundingBox;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCacheConfigRuleRegistry;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCacheFormatDescription;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCallbackIdArray;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCameraSetMessage;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColor;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColorArray;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCommandMessage;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCommandResult;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCommonSystemUtils;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MComputation;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MConditionMessage;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MConnectDisconnectAttrEdit;         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MContainerMessage;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCurveAttribute;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCustomEvaluatorClusterNode;        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDagMessage;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDagModifier;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDAGDrawOverrideInfo;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDagPath;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDagPathArray;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDataBlock;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDataHandle;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDGContext;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDGContextGuard;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDGMessage;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDGModifier;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDistance;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDoubleArray;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEdit;                              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEulerRotation;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEvaluationGraph;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEvaluationManager;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEvaluationNode;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEvaluationNodeIterator;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEventMessage;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MExternalContentInfoTable;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MExternalContentLocationTable;      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFcurveEdit;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFileIO;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFileObject;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatArray;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatMatrix;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatPoint;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatPointArray;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatVector;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFloatVectorArray;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAmbientLight;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAnisotropyShader;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAreaLight;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnArrayAttrsData;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAssembly;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAttribute;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnBase;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnBlinnShader;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCamera;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCameraSet;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnComponent;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnComponentListData;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCompoundAttribute;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnContainerNode;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDagNode;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnData;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDependencyNode;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDirectionalLight;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDoubleArrayData;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDoubleIndexedComponent;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnEnumAttribute;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnExpression;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnFloatArrayData;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnGenericAttribute;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnGeometryData;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnIntArrayData;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLambertShader;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLatticeData;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLayeredShader;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLight;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnLightDataAttribute;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMatrixArrayData;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMatrixAttribute;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMatrixData;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMesh;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMeshIsectAccelParams;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMeshData;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMeshOperation;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnMessageAttribute;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNonAmbientLight;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNonExtendedLight;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNumericAttribute;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNumericData;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNurbsCurve;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNurbsCurveData;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNurbsSurface;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnNurbsSurfaceData;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPartition;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPhongEShader;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPhongShader;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPlugin;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPluginData;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPointArrayData;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPointLight;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnReference;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnReflectShader;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnScPlugin;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSet;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSingleIndexedComponent;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSphereData;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSpotLight;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnStringArrayData;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnStringData;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSubd;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSubdData;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnSubdNames;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnTransform;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnTripleIndexedComponent;          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnTypedAttribute;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnUInt64ArrayData;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnUint64SingleIndexedComponent;    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnUnitAttribute;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnVectorArrayData;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnVolumeLight;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGlobal;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGraphNodeIterator;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIffFile;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIffTag;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MImage;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MImageFileInfo;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MInt64Array;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIntArray;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItCurveCV;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItDag;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItDependencyGraph;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItDependencyNodes;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItEdits;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MIteratorType;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItGeometry;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItInstancer;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItMeshEdge;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItMeshFaceVertex;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItMeshPolygon;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItMeshVertex;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItSelectionList;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItSubdEdge;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItSubdFace;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItSubdVertex;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MItSurfaceCV;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MLibrary;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MLockMessage;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMatrix;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMatrixArray;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMeshIntersector;                   \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPointOnMesh;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMeshSmoothOptions;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMessage;                           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMessageNode;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MModelMessage;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMutexLock;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MNamespace;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MNodeClass;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MNodeMessage;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MNurbsIntersector;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPointOnNurbs;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MObjectArray;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MObjectHandle;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MObjectSetMessage;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAutoCLKernel;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAutoCLMem;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAutoCLEvent;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MOpenCLBuffer;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MOpenCLInfo;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MParentingEdit;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPlane;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPlug;                              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPlugArray;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPoint;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPointArray;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPolyMessage;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MProfiler;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MProfilingScope;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxAssembly;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxAttributePatternFactory;         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxCacheFormat;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxCameraSet;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxCommand;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxComponentShape;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxCustomEvaluator;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxData;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxFileResolver;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxFileTranslator;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxGeometryData;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxGeometryIterator;                \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxImageFile;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxImagePlane;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMayaAsciiFilterOutput;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMayaAsciiFilter;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMultiPolyTweakUVCommand;         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxNode;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxObjectSet;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxPolyTrg;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxPolyTweakUVCommand;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxRepresentation;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxSurfaceShape;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxTopologyEvaluator;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxTransform;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxTransform_BoundingBox;           \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxTransformationMatrix;            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MQuaternion;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRampAttribute;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRichSelection;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSceneMessage;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MScriptUtil;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSelectionList;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSelectionMask;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSetAttrEdit;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSpinLock;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MStatus;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MStreamUtils;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MString;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MStringArray;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MStringResource;                    \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MStringResourceId;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSyntax;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTesselationParams;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MThreadAsync;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MThreadPool;                        \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MThreadUtils;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTime;                              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTimeArray;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTimer;                             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTimerMessage;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTopologyClusterNode;               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTransformationMatrix;              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTrimBoundaryArray;                 \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTypeId;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUint64Array;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUintArray;                         \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUniqueString;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MURI;                               \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUserData;                          \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUserEventMessage;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUuid;                              \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MVector;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MVectorArray;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MWeight;                            \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MaterialInputData;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxBakeEngine;                      \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMaterialInformation;             \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxRenderPassImpl;                  \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderPassDef;                     \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderPassRegistry;                \
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::M3dView;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MAnimStream;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCursor;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDeviceChannel;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDeviceState;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawData;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawInfo;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawRequest;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawRequestQueue;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawTraversal;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MEvent;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MExternalDropCallback;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MExternalDropData;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFeedbackLine;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCircleSweepManip;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnCurveSegmentManip;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDirectionManip;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDiscManip;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnDistanceManip;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnFreePointTriadManip;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnManip3D;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPointOnCurveManip;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnPointOnSurfaceManip;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnRotateManip;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnScaleManip;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnStateManip;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnToggleManip;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnToolContext;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGraphEditorInfo;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWShaderSwatchGenerator;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MManipData;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMaterial;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MMaterialArray;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MObjectListFilter;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPaintMessage;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MProgressWindow;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPx3dModelView;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxClientDeviceNode;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxContext;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxContextCommand;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxControlCommand;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxDragAndDropBehavior;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxFileDialog;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxGlBuffer;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxHardwareShader;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxHwShaderNode;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxLocatorNode;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxManipContainer;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxManipulatorNode;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxMidiInputDevice;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxModelEditorCommand;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxPolyTweakUVInteractiveCommand;	\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxSelectionContext;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxSurfaceShapeUI;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxTexContext;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxThreadedDeviceNode;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCharBuffer;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxToolCommand;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxUIControl;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxUITableControl;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MQtUtil;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSelectInfo;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTextureEditorDrawInfo;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MToolsInfo;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUiMessage;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColorManagementUtilities;					\
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColorManagementNodes;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColorPickerUtilities;                       \
    using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MColorPickerCallback;                        \
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MCommonRenderSettingsData;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MD3D9Renderer;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MDrawProcedureBase;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnAdskMaterial;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnImageSource;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnRenderLayer;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MFnRenderPass;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryLegacy;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryData;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryList;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryManager;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryPrimitive;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGeometryRequirementsLegacy;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MGLFunctionTable;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHardwareRenderer;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHwrCallback;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHwTextureManager;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MLightLinks;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MTestGLvoidPtr;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MPxRenderer;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderCallback;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderData;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderingInfo;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderProfile;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderSetup;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderSetupPrivate;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderShadowData;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderTargetLegacy;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderUtil;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MRenderView;								\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSwatchRenderBase;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MSwatchRenderRegister;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUniformParameter;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MUniformParameterList;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MVaryingParameter;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MVaryingParameterList;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MViewportRenderer;							\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MComponentDataIndexing;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MComponentDataIndexingList;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDrawContext;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPassContext;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MLightParameterInformation;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDrawRegistry;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MFragmentManager;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MFrameContext;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryExtractor;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferDescriptor;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferDescriptorList;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBuffer;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferArray;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBufferDescriptor;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBufferDescriptorList;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBuffer;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryIndexMapping;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderItem;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderItemList;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometry;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryRequirements;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryUtilities;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxComponentConverter;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxDrawOverride;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxGeometryOverride;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxIndexBufferMutator;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxImagePlaneOverride;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxPrimitiveGenerator;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxShaderOverride;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MAttributeParameterMapping;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MAttributeParameterMappingList;	\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxShadingNodeOverride;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSubSceneContainer;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxSubSceneOverride;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxSurfaceShadingNodeOverride;	\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxVertexBufferGenerator;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxVertexBufferMutator;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderParameters;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetDescription;		\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTarget;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetManager;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderUtilities;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSelectionContext;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIntersection;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSelectionInfo;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderInstance;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderManager;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureAssignment;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetAssignment;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderCompileMacro;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MStateManager;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MBlendState;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTargetBlendDesc;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MBlendStateDesc;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRasterizerState;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRasterizerStateDesc;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthStencilState;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MStencilOpDesc;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthStencilStateDesc;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSamplerState;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSamplerStateDesc;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureDescription;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthNormalizationDescription;	\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureUpdateRegion;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTexture;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureArguments;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureManager;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MUIDrawManager;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MCameraOverride;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOperation;				\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOperationList;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MUserRenderOperation;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MHUDRender;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPresentTarget;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MClearOperation;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSceneRender;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MQuadRender;						\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOverride;					\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderScriptCallback;			\
	using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderer;						\
	namespace MHWRender {														\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MComponentDataIndexing;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MComponentDataIndexingList;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDrawContext;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPassContext;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MLightParameterInformation;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDrawRegistry;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MFragmentManager;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MFrameContext;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryExtractor;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferDescriptor;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferDescriptorList;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBuffer;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MVertexBufferArray;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBufferDescriptor;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBufferDescriptorList;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIndexBuffer;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryIndexMapping;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderItem;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderItemList;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometry;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryRequirements;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MGeometryUtilities;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxComponentConverter;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxDrawOverride;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxGeometryOverride;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxIndexBufferMutator;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxImagePlaneOverride;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxPrimitiveGenerator;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxShaderOverride;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MAttributeParameterMapping;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MAttributeParameterMappingList;	\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxShadingNodeOverride;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSubSceneContainer;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxSubSceneOverride;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxSurfaceShadingNodeOverride;	\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxVertexBufferGenerator;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPxVertexBufferMutator;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderParameters;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetDescription;		\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTarget;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetManager;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderUtilities;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSelectionContext;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MIntersection;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSelectionInfo;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderInstance;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderManager;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureAssignment;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderTargetAssignment;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MShaderCompileMacro;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MStateManager;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MBlendState;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTargetBlendDesc;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MBlendStateDesc;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRasterizerState;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRasterizerStateDesc;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthStencilState;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MStencilOpDesc;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthStencilStateDesc;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSamplerState;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSamplerStateDesc;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureDescription;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MDepthNormalizationDescription;	\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureUpdateRegion;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTexture;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureArguments;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MTextureManager;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MUIDrawManager;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MCameraOverride;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOperation;				\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOperationList;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MUserRenderOperation;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MHUDRender;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MPresentTarget;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MClearOperation;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MSceneRender;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MQuadRender;						\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderOverride;					\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderScriptCallback;			\
		using OPENMAYA_MAJOR_NAMESPACE_SCHEMA::MHWRender::MRenderer;						\
	}

#define USING_MAJOR_API \
	USING_MAJOR_ONLY_API

#define USING_LATEST_API USING_MAJOR_API
#define OPENMAYA_LATEST_NAMESPACE OPENMAYA_MAJOR_NAMESPACE

// ****************************************************************************

// USE LATEST API

// ****************************************************************************

USING_LATEST_API

#endif /* SWIG */

#endif /* __cplusplus */
#endif /* _MApiNamespace */

#ifndef _MQtUtil
#define _MQtUtil
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MQtUtil
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

// Handle name clashes between Qt and platform-specific headers.
#if defined(LINUX)
#	if defined(CursorShape)
#		define X_CursorShape CursorShape
#		undef CursorShape
#	endif
#endif

#if defined(OSMac_) || defined(_WIN32)
#	if defined(None)
#		define QtMaya_None None
#		undef None
#	endif
#endif

#include <QtCore/QList>
#include <QtCore/QString>

#if defined(LINUX)
#	if defined(X_CursorShape)
#		define CursorShape X_CursorShape
#		undef X_CursorShape
#	endif
#endif

#if defined(OSMac_) || defined(_WIN32)
#	if defined(QtMaya_None)
#		define None 0
#		undef QtMaya_None
#	endif
#endif

#include <maya/MNativeWindowHdl.h>
#include <maya/MString.h>
#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS

class QAction;
class QObject;
class QWidget;
class QPixmap;
class QGLContext;
class QCursor;
class QIcon;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MQtUtil)

//! \ingroup OpenMayaUI
//! \brief Qt Utility Methods
/*!
  This class provides some basic methods for accessing the Qt controls
  which underlie Maya's UI.

  The safest way to use the Qt API from within Maya is to create your own Qt
  window and populate it with your own controls.

  While it is possible to use the Qt API to modify existing Maya UI
  elements, such as those created using Maya commands from MEL or Python,
  such modifications are not supported by Autodesk and could lead to Maya
  becoming unstable or inoperable.

  In practice, you will likely find a number of modifications which
  appear safe, such as changing a control's text or reorganizing the items
  on a menu. However, Autodesk provides no guarantees that the underlying
  implementations of those UI elements won't change from one release of
  Maya to another, potentially in ways that may make formerly safe usage
  become unsafe. So if you choose to incorporate such actions into your plug-in
  be aware that you may have to modify your code in future versions of Maya.

  <b>Layouts In Maya Vs. Qt</b>

  In Qt a layout (i.e. a QLayout or something derived from it) is not a
  widget. It is attached to a widget to control the positioning of that
  widget's children but is not itself a child of the widget and does
  not appear in the widget hierarchy.

  In Maya, layouts are also widgets which are part of the hierarchy.
  For example, if you have a window named "myWin" which contains a layout
  named "myLayout" which in turn holds a button named "myButton", then the
  button's full hierarchical name would be "myWin|myLayout|myButton".

  To bridge this difference, each Maya layout is given its own placeholder
  QWidget to which the corresponding QLayout is applied and bearing the
  same name as the layout. The children of the layout are then parented
  beneath the placeholder widget. When you call the findLayout() method,
  it is the placeholder widget which is returned.

  The one exception to this use of placeholders is the top-level layout of
  a window. Rather than introduce a superfluous placeholder widget, the
  top-level QLayout is attached directly to the window. So a call to
  findLayout() for a top-level layout will return the window. Note, however,
  that the top-level layout's hierarchical name is still structured as if
  it had its own dedicated widget in the hierarchy. E.g. "myWin|myLayout".
  This can lead to the somewhat incongruous result that calling fullName()
  on the result of a findLayout() call for a top-level layout will return
  the name of the window rather than the layout. E.g:

  \code
  QWidget* layoutWidget = MQtUtil::findLayout("myWin|myLayout");
  MString  layoutName = MQtUtil::fullName(layoutWidget);
  \endcode

  In the example above, \c layoutName will contain "myWin" rather than the
  string we started out with, "myWin|myLayout". To get around this problem
  pass the widget's QLayout to fullName() instead:

  \code
  MString layoutName = MQtUtil::fullName(layoutWidget->layout());
  \endcode

  Now \c layoutName will contain "myWin|myLayout" as expected.

  Not all Maya layouts are implemented by a single QLayout attached to a
  single placeholder widget (or window). In some cases the layout is
  implemented by a small sub-tree of widgets and QLayouts. Despite this
  added complexity behind the scenes, Maya still treats the entire sub-tree
  as a single element in the hierarchy for which the findLayout() method
  will return the top-level widget of the sub-tree.

  To simplify the traversal of these complex layouts, the getLayoutChildren()
  and getParent() methods treat complex layouts as if they are a single
  layout widget with their children parented directly beneath, skipping over
  any internal widgets which the layout's sub-tree may contain. In this
  way, getLayoutChildren() returns the same set of children as the \c layout
  command's <tt>-query -childArray</tt> flags, while getParent() returns
  the same layout as a control's <tt>-query -parent</tt> flags.

  <b>Order of Include Files</b>

  Some Qt header files redefine certain standard C++ library symbols. As such
  it is important that the Qt headers be included before any other header files
  which might reference those symbols.

  To ensure proper compilation, your source files should always include any
  Qt header files they use (QWidget, QEvent, etc) before all other header
  files. If the source includes MQtUtil.h then it should come after the Qt
  headers but before any others.

	<b>Qt Licensing</b>

	Qt is made available under both open source and commercial licenses. The
	type of license required depends upon your intended use of Qt. For more
	information, see the Qt Licensing page at
	http://qt.nokia.com/about/licensing/frequently-asked-questions
*/
class OPENMAYAUI_EXPORT MQtUtil {
public:
	//! \brief Pointer to a widget creator function.
	/*!
	 \param[in] parent The function should use this parameter to set the parent
	                   of the created object.
	 \param[in] name The function should use this parameter to set the
	                 objectName of the created object.
	 \return
	 	A pointer to the created object. The type of the object should match
	 	the class name passed to ::registerUIType.
	*/
	typedef QWidget*(*UITypeCreatorFn)(QWidget* parent, const QString& name);

	static QWidget*	findControl(MString controlName, const QWidget* ancestor = NULL);
	static QWidget*	findLayout(MString layoutName, const QWidget* ancestor = NULL);
	static QAction*	findMenuItem(MString itemName);
	static QWidget*	findWindow(MString windowName);
	static MString	fullName(const QObject* uiElement);
	static QWidget* getCurrentParent();

	static QPixmap*	createPixmap(const MString& imageName, bool autoScale=true);
	static QCursor*	createCursor(const MString& cursorName, bool autoScale=true);
	static QIcon*	createIcon(const MString& iconName, bool autoScale=true);

	static QList<QObject*>	getLayoutChildren(const QWidget* layout);

	static QObject*	getParent(const QObject* uiElement);

	static MString addWidgetToMayaLayout(QWidget* control, QWidget* layout,
			const MString& uiType = MString());

	static bool registerUIType(const MString& className, UITypeCreatorFn, const MString& command);
	static bool deregisterUIType(const MString& className);

	static QWidget*	mainWindow();
	static QGLContext* resourceGLContext();

	static MNativeWindowHdl	nativeWindow(const QWidget* control);

	static MString	toMString(const QString& qstr);
	static QString	toQString(const MString& mstr);

	static int		dpiScale(int size);
	static float	dpiScale(float size);

	static const char *className();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MQtUtil */

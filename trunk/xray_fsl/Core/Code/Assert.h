/*
Данный файл является частью xrFSL
Модуль:					Assert
Назначение:				Модуль обработки ассёртов.
Автор:					Neo][
Последнее обновление:	12.6.2008 by Neo][ - created
ToDo:

*/	
#pragma once

//---------------------------------------------
/// Assert macros
//---------------------------------------------
/**
*	@brief Проверяет, что condition true и продолжает выполнение. Фатал версия, не позволяет продолжить выполнение, 
*	если condition false.
*	Если condition false, тогда выдаст диалог, в котором будет информация об имени файла и номере строки, где сработал
*	ассёрт и сообщение msg.
*	@code
*		ASSERT_F(expression, ("This is formatted :%s", "message"));
*	@endcode
*/
#define ASSERT_F(condition, msg)																	\
	do																								\
	{																								\
	static bool s_bIgnoreAssert = false;														\
	if(!s_bIgnoreAssert && !(condition))														\
	{																							\
	xrFSL::processMessage msg;																\
	if (xrFSL::processingAssert(#condition, __FILE__, __LINE__, &s_bIgnoreAssert, true))	\
	{																						\
	_asm { int 3 };																		\
	}																						\
	}																							\
	} while(0)

/**
*	@brief Проверяет, что condition true и продолжает выполнение. Варнинг версия, позволяет продолжить выполнение, 
*	если condition false.
*	Если condition false, тогда выдаст диалог, в котором будет информация об имени файла и номере строки, где сработал
*	ассёрт и сообщение msg.
*	@code
*		ASSERT_W(expression, ("This is formatted :%s", "message"));
*	@endcode
*/
#define ASSERT_W(condition, msg)																	\
	do																								\
	{																								\
	static bool s_bIgnoreAssert = false;														\
	if(!s_bIgnoreAssert && !(condition))														\
	{																							\
	xrFSL::processMessage msg;																\
	if (xrFSL::processingAssert(#condition, __FILE__, __LINE__, &s_bIgnoreAssert))			\
	{																						\
	_asm { int 3 };																		\
	}																						\
	}																							\
	} while(0)


#include "Prerequisites.h"

namespace xrFSL
{

#	define IDD_DIALOG_ASSERT		101
#	define IDC_EDIT_LINE			1000
#	define IDC_EDIT_FILE			1001
#	define IDC_EDIT_CONDITION		1002
#	define IDC_BUTTON_CONTINUE		1003
#	define IDC_EDIT_REASON			1004
#	define IDC_BUTTON_IGNORE		1005
//#	define IDC_BUTTON_REPORT		1006
#	define IDC_BUTTON_STOP			1007
#	define IDC_BUTTON_BREAK			1008
#	define IDC_BUTTON_IGNORE_ALL	1009

#	define IDC_STATIC_TEXT			0

#	define DLG_TITLE			L"Assertion Failed"
#	define DLG_FONT				L"MS Sans Serif"
#	define DLG_ITEM_TEXT_0		L"Continue"
#	define DLG_ITEM_TEXT_1		L"Stop"
#	define DLG_ITEM_TEXT_2		L"Info"
#	define DLG_ITEM_TEXT_3		L""
#	define DLG_ITEM_TEXT_4		L"Line"
#	define DLG_ITEM_TEXT_5		L""
#	define DLG_ITEM_TEXT_6		L"File"
#	define DLG_ITEM_TEXT_7		L"Condition"
#	define DLG_ITEM_TEXT_8		L""
#	define DLG_ITEM_TEXT_9		L"failed"
#	define DLG_ITEM_TEXT_10		L""
#	define DLG_ITEM_TEXT_11		L"Reason"
#	define DLG_ITEM_TEXT_12		L"Ignore"
//#	define DLG_ITEM_TEXT_13		L"Report as Bug"
#	define DLG_ITEM_TEXT_14		L"Break"
#	define DLG_ITEM_TEXT_15		L"Ignore All"

//#	define DLG_NB_ITEM			16
#	define DLG_NB_ITEM			15


	template<int iTitleSize>
	struct SDlgItem
	{
		// If use my struct instead of DLGTEMPLATE, or else (for some strange reason) it is not DWORD aligned !!
		DWORD style;
		DWORD dwExtendedStyle;
		short x;
		short y;
		short cx;
		short cy;
		WORD id;
		WORD ch;
		WORD c;
		WCHAR t[iTitleSize];
		WORD dummy;
	};
#	define SDLGITEM(TEXT, V) SDlgItem<sizeof(TEXT) / 2> V;

	struct SDlgData
	{
		DLGTEMPLATE dlt;
		WORD _menu;
		WORD _class;
		WCHAR _title[sizeof(DLG_TITLE) / 2];
		WORD pointSize;
		WCHAR _font[sizeof(DLG_FONT) / 2];

		SDLGITEM(DLG_ITEM_TEXT_0,i0);
		SDLGITEM(DLG_ITEM_TEXT_12,i12);
		SDLGITEM(DLG_ITEM_TEXT_15,i15);
		SDLGITEM(DLG_ITEM_TEXT_14,i14);
		SDLGITEM(DLG_ITEM_TEXT_1,i1);
		SDLGITEM(DLG_ITEM_TEXT_2,i2);
		SDLGITEM(DLG_ITEM_TEXT_3,i3);
		SDLGITEM(DLG_ITEM_TEXT_4,i4);
		SDLGITEM(DLG_ITEM_TEXT_5,i5);
		SDLGITEM(DLG_ITEM_TEXT_6,i6);
		SDLGITEM(DLG_ITEM_TEXT_7,i7);
		SDLGITEM(DLG_ITEM_TEXT_8,i8);
		SDLGITEM(DLG_ITEM_TEXT_9,i9);
		SDLGITEM(DLG_ITEM_TEXT_10,i10);
		SDLGITEM(DLG_ITEM_TEXT_11,i11);
		//SDLGITEM(DLG_ITEM_TEXT_13,i13);
	};

	static SDlgData gs_warnDialogRC =
	{
		{DS_MODALFRAME|DS_3DLOOK|DS_SETFONT|DS_CENTER|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_VISIBLE,0,DLG_NB_ITEM,0,0,330,134},0,0,DLG_TITLE,8,DLG_FONT,
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	12,	113,50,	14,	IDC_BUTTON_CONTINUE,	0xFFFF,0x0080,DLG_ITEM_TEXT_0,	0},
		{BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,									0,	66,	113,50,	14,	IDC_BUTTON_IGNORE,		0xFFFF,0x0080,DLG_ITEM_TEXT_12,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	120,113,50,	14,	IDC_BUTTON_IGNORE_ALL,0xFFFF,0x0080,DLG_ITEM_TEXT_15,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	214,113,50,	14,	IDC_BUTTON_BREAK,			0xFFFF,0x0080,DLG_ITEM_TEXT_14,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	268,113,50,	14,	IDC_BUTTON_STOP,			0xFFFF,0x0080,DLG_ITEM_TEXT_1,	0},
		{BS_GROUPBOX|WS_CHILD|WS_VISIBLE,																	0,	7,	7,	316,100,IDC_STATIC_TEXT,			0xFFFF,0x0080,DLG_ITEM_TEXT_2,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	48,	25,	13,	IDC_EDIT_LINE,				0xFFFF,0x0081,DLG_ITEM_TEXT_3,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	14,	50,	14,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_4,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	32,	240,13,	IDC_EDIT_FILE,				0xFFFF,0x0081,DLG_ITEM_TEXT_5,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	14,	34,	12,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_6,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	13,	18,	30,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_7,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	16,	240,13,	IDC_EDIT_CONDITION,		0xFFFF,0x0081,DLG_ITEM_TEXT_8,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	298,19,	18,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_9,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	67,	240,13,	IDC_EDIT_REASON,			0xFFFF,0x0081,DLG_ITEM_TEXT_10,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	15,	69,	26,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_11,	0},
		//{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_DISABLED,										0,	127,88,	72,	12,	IDC_BUTTON_REPORT,		0xFFFF,0x0080,DLG_ITEM_TEXT_13,	0},
	};

	static SDlgData gs_fatalDialogRC =
	{
		{DS_MODALFRAME|DS_3DLOOK|DS_SETFONT|DS_CENTER|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_VISIBLE,0,DLG_NB_ITEM,0,0,330,134},0,0,DLG_TITLE,8,DLG_FONT,
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_DISABLED,										0,	12,	113,50,	14,	IDC_BUTTON_CONTINUE,	0xFFFF,0x0080,DLG_ITEM_TEXT_0,	0},
		{BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_DISABLED,									0,	66,	113,50,	14,	IDC_BUTTON_IGNORE,		0xFFFF,0x0080,DLG_ITEM_TEXT_12,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_DISABLED,										0,	120,113,50,	14,	IDC_BUTTON_IGNORE_ALL,0xFFFF,0x0080,DLG_ITEM_TEXT_15,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	214,113,50,	14,	IDC_BUTTON_BREAK,			0xFFFF,0x0080,DLG_ITEM_TEXT_14,	0},
		{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP,										0,	268,113,50,	14,	IDC_BUTTON_STOP,			0xFFFF,0x0080,DLG_ITEM_TEXT_1,	0},
		{BS_GROUPBOX|WS_CHILD|WS_VISIBLE,																	0,	7,	7,	316,100,IDC_STATIC_TEXT,			0xFFFF,0x0080,DLG_ITEM_TEXT_2,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	48,	25,	13,	IDC_EDIT_LINE,				0xFFFF,0x0081,DLG_ITEM_TEXT_3,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	14,	50,	14,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_4,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	32,	240,13,	IDC_EDIT_FILE,				0xFFFF,0x0081,DLG_ITEM_TEXT_5,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	14,	34,	12,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_6,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	13,	18,	30,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_7,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	16,	240,13,	IDC_EDIT_CONDITION,		0xFFFF,0x0081,DLG_ITEM_TEXT_8,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	298,19,	18,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_9,	0},
		{ES_LEFT|ES_AUTOHSCROLL|ES_READONLY|WS_BORDER|WS_CHILD|WS_VISIBLE,0,	50,	67,	240,13,	IDC_EDIT_REASON,			0xFFFF,0x0081,DLG_ITEM_TEXT_10,	0},
		{WS_CHILD|WS_VISIBLE,																							0,	15,	69,	26,	8,	IDC_STATIC_TEXT,			0xFFFF,0x0082,DLG_ITEM_TEXT_11,	0},
		//{BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_DISABLED,										0,	127,88,	72,	12,	IDC_BUTTON_REPORT,		0xFFFF,0x0080,DLG_ITEM_TEXT_13,	0},
	};

	/// Тип ассёрта
	enum EType
	{
		Fatal		= 1,
		Warning		= 2
	};

	//-----------------------------------------------------------------------
	/// Структура с информацией об ассёрте
	//-----------------------------------------------------------------------
	struct SAssertInfo
	{
		String sCondition;	///< Выражение вызвавшее ассёрт
		String sFile;		///< Имя файла, где произошёл ассёрт
		uint32 uiLine;				///< Строка в файле, где произошёл ассёрт
		String sMessage;		///< Сообщение ассёрта		

		// кнопки в диалоге ассёрта
		enum
		{
			BUTTON_CONTINUE,		///< Продолжить
			BUTTON_IGNORE,			///< Игнорировать данный ассёрт
			BUTTON_IGNORE_ALL,		///< Игнорировать все ассёрты
			BUTTON_BREAK,			///< Уронить приложение
			BUTTON_STOP				///< Остановить выполнение
		} btnChosen;				///< Выбранная кнопка в диалоге ассёрта

		uint32 uiX;
		uint32 uiY;
	};
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	// Переменные
	//-----------------------------------------------------------------------
	static char gs_szMessage[MAX_PATH];	///< Сообщение ассёрта, заполняется в методе Assert::processMessage()

	//-----------------------------------------------------------------------
	// Функции
	//-----------------------------------------------------------------------
	/**
	*	
	*/
	xrFSL_CORE_API bool processingAssert(const String& sCondition, const String& sFile, uint32 uiLine, bool* pbIgnore, bool bFatal = false);

	/**
	*	
	*/
	static INT_PTR CALLBACK dlgProc(HWND hDlg, uint32 uiMsg, WPARAM wParam, LPARAM lParam);

	/**
	 *	
	 */
	xrFSL_CORE_API void processMessage(const char* pszFormatMsg,...);

	// class Assert
	// {
	// public:
	// 	
	// 
	// 	/**
	// 	*	
	// 	*/
	// 	
	// 
	// 
	// protected:
	// private:
	// 	bool m_bProcessing;			///<
	// 	
	//	static bool Asser();
	// 	/**
	// 	*	
	// 	*/
	// 	
	// 
	// 	Assert(void): m_bProcessing(false) {};
	// 	virtual ~Assert(void){};
	// };

} // end namespace xrFSL
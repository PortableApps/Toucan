/////////////////////////////////////////////////////////////////////////////////
// Author:      Jorgen Bodde & Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef __WXVIRTUALDIRTREECTRL_H__
#define __WXVIRTUALDIRTREECTRL_H__

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dynarray.h>
#include <wx/treectrl.h>
#include <wx/filesys.h>
#include <wx/imaglist.h>

#include "../rules.h"

enum {
	VDTC_TI_ROOT = 0,
	VDTC_TI_DIR,
	VDTC_TI_FILE
};

#ifndef LINUX
#define VDTC_DIR_FILESPEC wxT("*.*")
#else
#define VDTC_DIR_FILESPEC wxT("*")
#endif

/// Icon number for root
#define VDTC_ICON_ROOT     0

/// Icon number for a folder
#define VDTC_ICON_DIR      1

/// Icon number for file
#define VDTC_ICON_FILE     2

/** \class VdtcTreeItemBase
	This class is used to link the information of a file/root or folder node to a wxTreeItem
	in the wxTreeCtrl.

	To use more information in this class, inherit this with your own copy, and in the handler
	wxVirtualDirTreeCtrl::OnCreateTreeItem, return your own class. This allows you to store more
	information in this class.

	For example; to return a different caption instead of the standard filename / directory, inherit this, and
	redefine the VdtcTreeItemBase::GetCaption() member. The same goes for the VdtcTreeItemBase::GetIconId()
	and VdtcTreeItemBase::GetSelectedIconId() functions if you redefined the bitmaps in the imagelist.

*/
class VdtcTreeItemBase : public wxTreeItemData
{
protected:
	int _type;
	wxString _name;
	wxColour _colour;


public:

	VdtcTreeItemBase(int type, const wxString &name)
			: _type(type)
			, _name(name) {
		_colour = wxColour(wxT("Black"));
	};

	~VdtcTreeItemBase() {};

/** Virtual function to return the icon ID this node should get. Per default it gets the ID of the
	    default image list. If you assigned more bitmaps (or different bitmaps) to the image list, return
		the proper indices based upon the class it refers to. The ID's returned are:

		- VDTC_ICON_ROOT: For root
		- VDTC_ICON_DIR: For a directory
		- VDTC_ICON_FILE: For a file
	*/
	virtual int GetIconId() const {
		switch (_type) {
		case VDTC_TI_ROOT:
			return VDTC_ICON_ROOT;
		case VDTC_TI_DIR:
			return VDTC_ICON_DIR;
		case VDTC_TI_FILE:
			return VDTC_ICON_FILE;
		}
		return -1;
	};

	wxColour &GetColour() {
		return _colour;
	}
	void SetColour(wxColour colour) {
		_colour = colour;
		return;
	}

	/** Virtual function to return the selected icon ID this node should get. Per default there is no icon
	    associated with a selection. If you would like a selection, inherit this class and redefine this function
		to return a proper id.
	*/
	virtual int GetSelectedIconId() const {
		return -1;
	};

	/** Gets this name. The name of the root is the base path of the whole directory, the
	   name of a file node is the filename, and from a dir node the directory name.
	   \sa IsDir, IsFile, IsRoot */
	const wxString &GetName() const {
		return _name;
	};

	/** Returns true if this is of type VDTC_TI_DIR */
	bool IsDir() const {
		return _type == VDTC_TI_DIR;
	};

	/** Returns true if this is of type VDTC_TI_ROOT */
	bool IsRoot() const {
		return _type == VDTC_TI_ROOT;
	};

	/** Returns true if this is of type VDTC_TI_FILE */
	bool IsFile() const {
		return _type == VDTC_TI_FILE;
	};

};

// the formal definition of the array of pointers for tree item base
WX_DEFINE_ARRAY(VdtcTreeItemBase *, VdtcTreeItemBaseArray);

/**
	\class wxVirtualDirTreeCtrl
	A class which allows the user to browse through a 'virtual' treectrl, fully customizable when it's derived.

	The wxVirtualDirTreeCtrl can be used for displaying the directory structure in a tree ctrl just like the
	wxDirCtrl, but has much more advantages and flexibility for the developer. A list of some of the functionality
	embedded:

	See the main page to read more about this control.
*/

class wxVirtualDirTreeCtrl : public wxTreeCtrl
{
private:
	/** Icons image list */
	wxImageList *_iconList;
	/** Extra flags */
	int _flags;

	bool _Preview;

	Rules* _Rules;

	bool _IsSync;

	wxString _Root;

	wxString _RootOpp;

	wxString _Mode;
	
	int _ScanDepth;

	/** Scans from given dir, for 'level' depth and with present extensions. This will
	    reload the directory on that level. If there are tree items associated with the 'reloaded'
		items they will be deleted first. */
	int ScanFromDir(VdtcTreeItemBase *item, const wxFileName &path, int level);

	/** Get files for current dir */
	void GetFiles(VdtcTreeItemBase *parent, VdtcTreeItemBaseArray &items, const wxFileName &path);

	/** get dirs in current dir */
	void GetDirectories(VdtcTreeItemBase *parent, VdtcTreeItemBaseArray &items, const wxFileName &path);

	void AppendPathRecursively(VdtcTreeItemBase *b, wxFileName &dir, bool useRoot = true);

	/** Adds items on this level to the parent dir associated wxTreeCtrl item */
	void AddItemsToTreeCtrl(VdtcTreeItemBase *item, VdtcTreeItemBaseArray &items);

	/** Sorts items with qsort algorithm */
	void SortItems(VdtcTreeItemBaseArray &items, int left, int right);

	/** Built in swap algoritm */
	void SwapItem(VdtcTreeItemBaseArray &items, int a, int b);

	// -- event handlers --

	void OnExpanding(wxTreeEvent &event);

protected:

	wxBitmap *CreateRootBitmap();
	wxBitmap *CreateFolderBitmap();
	wxBitmap *CreateFileBitmap();

	/** Inherited virtual function for SortChildren */
	int OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2);

public:
	/** Default constructor of this control. It is similar to the wxTreeCtrl */
	wxVirtualDirTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	                     const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS | wxTR_FULL_ROW_HIGHLIGHT,
	                     const wxValidator& validator = wxDefaultValidator,
	                     const wxString& name = wxT("wxVirtualDirTreeCtrl"));
	virtual ~wxVirtualDirTreeCtrl();

	/** Returns the extra flags currently set set for wxVirtualDirTreeCtrl.
		\sa SetRootPath */
	int GetExtraFlags() const {
		return _flags;
	};

	/** Sets the root path of the wxVirtualDirTreeCtrl. This will reset the view and restart the
	    process of notification, and all events that need to be called. Returns false when this
		root path does not lead to any valid filesystem or directory. Set extra flags for
		wxVirtualDirTreeCtrl when needed. The following flags are possible
		- wxVDTC_DEFAULT
		- wxVDTC_RELOAD_ALL
		- wxVDTC_NO_EXPAND
		- wxVDTC_SHOW_BUSYDLG
		- wxVDTC_NO_SORT
		- wxVDTC_NO_FILES */
	bool AddNewPath(const wxString &root, int flags = 0);

	/** Return full path of given node. This can also include the name of the file. This will be returned as
	    a wxFileName (with path and possible name) to be used further. Be warned, these functions can take some time because the whole tree
		structure is iterated from the top node down to the root. */
	wxFileName GetFullPath(const wxTreeItemId &id);

	/** Adds a file item. Be aware that this call does not add it to the wxTreeCtrl. This only creates an instance
	    of a VtdcTreeItemBase file node. The handler OnCreateTreeItem is called to allow the proper initialisation
		of every newly created instance of the (inherited) VdtcTreeItemBase class. */
	VdtcTreeItemBase *AddFileItem(const wxString &name);

	/** Adds a directory item. Be aware that this call does not add it to the wxTreeCtrl. This only creates an instance
	    of a VtdcTreeItemBase directory node. The handler OnCreateTreeItem is called to allow the proper initialisation
		of every newly created instance of the (inherited) VdtcTreeItemBase class. */
	VdtcTreeItemBase *AddDirItem(const wxString &name);

	/** Returns parent of the passed VdtcItemBase object. It will fetch the wxTreeItemId of this parent,
	    and return the VdtcTreeItemBase parent associated with it. If the associated item is nil, there is no
		parent, this is most likely the root else an assertion failure occurs */
	VdtcTreeItemBase *GetParent(VdtcTreeItemBase *item) const {
		wxCHECK(item, 0);

		wxTreeItemId p = GetItemParent(item->GetId());
		return (VdtcTreeItemBase *)GetItemData(p);
	};

	// --- handlers ---

	/** This virtual handler is used to allow the developer to assign custom icons to the
	    image list. Override this method to assign your own icons to the wxTreeCtrl. The default method
		will assign a root bitmap, a folder bitmap and a file bitmap. The icons assigned can be
		refered to in the OnFileName and OnDirectory handlers, to specify what icon should be used.

		The default images can be used by these constants:
		- VDTC_ICON_ROOT
		- VDTC_ICON_DIR
		- VDTC_ICON_FILE

		The icon list contains icons which are 16x16 in size.
	*/
	virtual void OnAssignIcons(wxImageList &icons);

	/** This handler provides must provide a VdtcTreeItemBase* instance to be used upon creation. If you
	    override this handler, you can return your own class, which might embed more information.

		The type tells you what kind of element is created, a root item (VDTC_TI_ROOT), a dir item (VDTC_TI_DIR)
		or a file item (VDTC_TI_FILE). Dependent upon the type you can return a different class based upon
		VdtcTreeItemBase.

		For example, this is the standard implementation:

		\code
			return new VdtcTreeItemBase(type, name);
		\endcode

		You could derive the wxVirtualDirTreeCtrl and VdtcTreeItemBase object and make this:

		\code

			switch(type)
			{
				case VDTC_TI_ROOT:
					return new MyRootItem(type, name);	// MyRootItem = class derived VdtcTreeItemBase
				case VDTC_TI_DIR:
					return new MyDirItem(type, name);	// MyDirItem = class derived VdtcTreeItemBase
				case VDTC_TI_FILE:
					return new MyFileItem(type, name);	// MyFileItem = class derived VdtcTreeItemBase
			}

			return 0;

		\endcode

		\sa VdtcTreeItemBase
	*/
	virtual VdtcTreeItemBase *OnCreateTreeItem(int type, const wxString &name);

	/** This handler is called before the directory specified by 'path' is scanned. You can veto this scan by
	    returning 'false'. This way the scanning of all files and directories in this path is skipped. After this
		handler, subsequent calls to OnAddFile, OnAddDirectory will be made for every file and directory
		encountered in this level to be scanned. NOTE: When this scan is veto'd there will be no call
		to OnDirectoryScanEnd because there was no scan. Also OnAddedItems is not called */

	virtual void OnDirectoryScanEnd(VdtcTreeItemBaseArray &items, const wxFileName &path);

	/** This handler is called the very moment after all items are added to the tree control. The parent parameter
	    contains the tree node to which the items are added. They are already sorted and ready. E.g. when you don't
		want to use the internal sort algoritm this is a nice place to call SortChildren on the parent node */

	virtual void OnAddedItems(const wxTreeItemId &parent);

	/** This handler is called during the sorting of the tree control items. Return < 0 when a < b, > 0 when a > b
	    and 0 when a == b.
	*/
	virtual int OnCompareItems(const VdtcTreeItemBase *a, const VdtcTreeItemBase *b);

	void SetPreview(bool preview) {
		_Preview = preview;
	}
	void SetRules(Rules *rules) {
		_Rules = rules;
	}
	void SetSync(bool sync) {
		_IsSync = sync;
	}

	void SetScanDepth(const int scandepth) {this->_ScanDepth = scandepth;}
	const int& GetScanDepth() const {return _ScanDepth;}
	
	//Neat expand all
	void NeatExpandAll(wxWindow* topwindow);

private:
	DECLARE_EVENT_TABLE()
};

#endif

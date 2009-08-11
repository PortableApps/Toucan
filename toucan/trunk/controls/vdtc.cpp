/////////////////////////////////////////////////////////////////////////////////
// Author:      Jorgen Bodde & Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/object.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/artprov.h>
#include "vdtc.h"
#include "../toucan.h"
#include "../variables.h"
#include "../data/syncdata.h"
#include "../sync/syncpreview.h"

#define ID_SYNC_SOURCE_TREE 10060

// WDR: class implementations

//----------------------------------------------------------------------------
// wxVirtualDirTreeCtrl
//----------------------------------------------------------------------------

// WDR: event table for wxVirtualDirTreeCtrl

BEGIN_EVENT_TABLE(wxVirtualDirTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_EXPANDING(-1, wxVirtualDirTreeCtrl::OnExpanding)
END_EVENT_TABLE()

wxVirtualDirTreeCtrl::wxVirtualDirTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
		: wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
	// create an icon list for the tree ctrl
	_iconList = new wxImageList(16,16);
	_ScanDepth = 2;
	_Preview = false;
	_IsSync = false;
	_Rules = new Rules(wxT("Internal"));
	this->AddRoot(wxT("Hidden - you wont see this"));
}

wxVirtualDirTreeCtrl::~wxVirtualDirTreeCtrl()
{
	// first delete all VdtcTreeItemBase items (client data)
	DeleteAllItems();

	// delete the icons
	delete _iconList;
	delete _Rules;
}

bool wxVirtualDirTreeCtrl::AddNewPath(const wxString &root, int flags)
{
	bool value;
	wxLogNull log;

	// set flags to adopt new behaviour
	_flags = flags;

	// delete all items plus root first
	//DeleteAllItems();
	//this->AddRoot(wxT("Hidden root"));
	VdtcTreeItemBase *start = 0;

	// now call for icons management, the virtual
	// handler so the derived class can assign icons

	_iconList->RemoveAll();
	OnAssignIcons(*_iconList);

	SetImageList(_iconList);

	//This happens backwards asif a file is added it *must* exists, whereas directories might not (i.e. due to variables)
	value = ::wxFileExists(root);
	if (!value) {

		// create a root item
		start = OnCreateTreeItem(VDTC_TI_ROOT, root);
		if (start) {
			wxFileName path;
			path.AssignDir(root);

			// add this item to the tree, with info of the developer
			wxTreeItemId id = this->AppendItem(this->GetRootItem(), start->GetName(), start->GetIconId(), start->GetSelectedIconId(), start);
				
			// scan directory, either the smart way or not at all
			ScanFromDir(start, path, GetScanDepth());

			Expand(id);
		}
	} 
	else{
		if(wxFileExists(root)){
			wxFileName path(root);
			start = OnCreateTreeItem(VDTC_TI_FILE, root);
			// add this item to the tree, with info of the developer
			wxTreeItemId id = this->AppendItem(this->GetRootItem(), start->GetName(), start->GetIconId(), start->GetSelectedIconId(), start);
		}
	}

	return value;
}

int wxVirtualDirTreeCtrl::ScanFromDir(VdtcTreeItemBase *item, const wxFileName &path, int level)
{
	int value = 0;
	wxCHECK(item, -1);
	wxCHECK(item->IsDir() || item->IsRoot(), -1);

	wxLogNull log;

	// when we can still scan, do so
	if (level == -1 || level > 0) {
		// if no items, then go iterate and get everything in this branch
		if (GetChildrenCount(item->GetId()) == 0) {
			//wxMessageBox(_("Creatin array"));
			VdtcTreeItemBaseArray addedItems;

			// get directories
			GetDirectories(item, addedItems, path);

			// get files
			GetFiles(item, addedItems, path);

			// call handler that can do a last thing
			// before sort and anything else
			OnDirectoryScanEnd(addedItems, path);

			// sort items
			if (addedItems.GetCount() > 0 )
				SortItems(addedItems, 0, addedItems.GetCount()-1);

			AddItemsToTreeCtrl(item, addedItems);
		}

		value = GetChildrenCount(item->GetId());

		// go through all children of this node, pick out all
		// the dir classes, and descend as long as the level allows it
		// NOTE: Don't use the addedItems array, because some new can
		// be added or some can be deleted.

		wxTreeItemIdValue cookie = 0;
		VdtcTreeItemBase *b;
		//wxMessageBox(_("Further"));
		wxTreeItemId child = GetFirstChild(item->GetId(), cookie);
		while (child.IsOk()) {
			b = (VdtcTreeItemBase *)GetItemData(child);
			if (b && b->IsDir()) {
				wxFileName tp = path;
				tp.AppendDir(b->GetName());
				value += ScanFromDir(b, tp, (level == -1 ? -1 : level-1));
			}

			child = GetNextChild(item->GetId(), cookie);
		}
	}

	return value;
}

void wxVirtualDirTreeCtrl::GetFiles(VdtcTreeItemBase *WXUNUSED(parent), VdtcTreeItemBaseArray &items, const wxFileName &path)
{
	wxFileName fpath;
	wxString fname;
	VdtcTreeItemBase *item;

	fpath = path;

	wxDir fdir(path.GetFullPath());

	if (fdir.IsOpened()) {
		bool bOk = fdir.GetFirst(&fname, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
		while (bOk) {
			item = AddFileItem(fname);
			if (item) {
				// fill it in, and marshall it by the user for info
				fpath.SetFullName(fname);
				items.Add(item);
			}

			bOk = fdir.GetNext(&fname);
		}
	}
}

void wxVirtualDirTreeCtrl::GetDirectories(VdtcTreeItemBase *WXUNUSED(parent), VdtcTreeItemBaseArray &items, const wxFileName &path)
{
	wxFileName fpath;
	wxString fname;
	VdtcTreeItemBase *item;

	// no nodes present yet, we should start scanning this dir
	// scan files first in this directory, with all extensions in this array

	wxDir fdir(path.GetFullPath());
	if (fdir.IsOpened()) {
		bool bOk = fdir.GetFirst(&fname, VDTC_DIR_FILESPEC, wxDIR_DIRS | wxDIR_HIDDEN);
		while (bOk) {
			// TODO: Flag for double items
			item = AddDirItem(fname);
			if (item) {
				// fill it in, and marshall it by the user for info
				fpath = path;
				fpath.AppendDir(fname);

				items.Add(item);
			}

			bOk = fdir.GetNext(&fname);
		}
	}
}

int wxVirtualDirTreeCtrl::OnCompareItems(const wxTreeItemId& item1, const wxTreeItemId& item2)
{
	// used for SortChildren, reroute to our sort routine
	VdtcTreeItemBase *a = (VdtcTreeItemBase *)GetItemData(item1),
	                      *b = (VdtcTreeItemBase *)GetItemData(item2);
	if (a && b)
		return OnCompareItems(a,b);

	return 0;
}

int wxVirtualDirTreeCtrl::OnCompareItems(const VdtcTreeItemBase *a, const VdtcTreeItemBase *b)
{
	// if dir and other is not, dir has preference
	if (a->IsDir() && b->IsFile())
		return -1;
	else if (a->IsFile() && b->IsDir())
		return 1;

	// else let ascii fight it out
	return a->GetName().CmpNoCase(b->GetName());
}

void wxVirtualDirTreeCtrl::SwapItem(VdtcTreeItemBaseArray &items, int a, int b)
{
	VdtcTreeItemBase *t = items[b];
	items[b] = items[a];
	items[a] = t;
}

void wxVirtualDirTreeCtrl::SortItems(VdtcTreeItemBaseArray &items, int left, int right)
{
	VdtcTreeItemBase *a, *b;
	int i, last;

	if (left >= right)
		return;

	SwapItem(items, left, (left + right)/2);

	last = left;
	for (i = left+1; i <= right; i++) {
		a = items[i];
		b = items[left];
		if (a && b) {
			if (OnCompareItems(a, b) < 0)
				SwapItem(items, ++last, i);
		}
	}

	SwapItem(items, left, last);
	SortItems(items, left, last-1);
	SortItems(items, last+1, right);
}


void wxVirtualDirTreeCtrl::AddItemsToTreeCtrl(VdtcTreeItemBase *item, VdtcTreeItemBaseArray &items)
{
	wxCHECK2(item, return);

	// now loop through all elements on this level and add them
	// to the tree ctrl pointed out by 'id'

	VdtcTreeItemBase *t;
	wxTreeItemId id = item->GetId();
	for (size_t i = 0; i < items.GetCount(); i++) {
		t = items[i];
		if (t) {
			wxTreeItemId idTemp = AppendItem(id, t->GetName(), t->GetIconId(), t->GetSelectedIconId(), t);
			OnAddedItems(idTemp);
		}

	}
}

wxFileName wxVirtualDirTreeCtrl::GetFullPath(const wxTreeItemId &id)
{
	wxFileName value;
	wxCHECK(id.IsOk(), value);

	VdtcTreeItemBase *b = (VdtcTreeItemBase *)GetItemData(id);
	wxCHECK(b, value);

	AppendPathRecursively(b, value, true);

	return value;
}

/** Appends subdirs up until root. This is done by finding the root first and
    going back down to the original caller. This is faster because no copying takes place */
void wxVirtualDirTreeCtrl::AppendPathRecursively(VdtcTreeItemBase *b, wxFileName &dir, bool useRoot)
{
	wxCHECK2(b, return);

	VdtcTreeItemBase *parent = GetParent(b);
	if (parent)
		AppendPathRecursively(parent, dir, useRoot);
	else {
		// no parent assume top node
		if (b->IsRoot() && useRoot)
			dir.AssignDir(b->GetName());
		return;
	}

	// now we are unwinding the other way around
	if (b->IsDir())
		dir.AppendDir(b->GetName());
	else if (b->IsFile())
		dir.SetFullName(b->GetName());
};

// -- event handlers --

void wxVirtualDirTreeCtrl::OnExpanding(wxTreeEvent &event)
{
	// check for collapsing item, and scan from there
	wxTreeItemId id = event.GetItem();
	if (id.IsOk()) {
		VdtcTreeItemBase *t = (VdtcTreeItemBase *)GetItemData(id);
		if (t && t->IsDir()) {
			// extract data element belonging to it, and scan.
			ScanFromDir(t, GetFullPath(id), GetScanDepth());
		}
	}

	// be kind, and let someone else also handle this event
	event.Skip();
}

wxBitmap *wxVirtualDirTreeCtrl::CreateRootBitmap()
{
	// create root and return
	return new wxBitmap(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER));
}

wxBitmap *wxVirtualDirTreeCtrl::CreateFolderBitmap()
{
	// create folder and return
	return new wxBitmap(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER));
}

wxBitmap *wxVirtualDirTreeCtrl::CreateFileBitmap()
{
	// create file and return
	return new wxBitmap(wxArtProvider::GetBitmap(wxART_NORMAL_FILE , wxART_OTHER));
}

VdtcTreeItemBase *wxVirtualDirTreeCtrl::AddFileItem(const wxString &name)
{
	// call the file item node create method
	return OnCreateTreeItem(VDTC_TI_FILE, name);
}

VdtcTreeItemBase *wxVirtualDirTreeCtrl::AddDirItem(const wxString &name)
{
	// call the dir item node create method
	return OnCreateTreeItem(VDTC_TI_DIR, name);
}


// --- virtual handlers ----

void wxVirtualDirTreeCtrl::OnAssignIcons(wxImageList &icons)
{
	wxBitmap *bmp;
	// default behaviour, assign three bitmaps

	bmp = CreateRootBitmap();
	icons.Add(*bmp);
	delete bmp;

	// 1 = folder
	bmp = CreateFolderBitmap();
	icons.Add(*bmp);
	delete bmp;

	// 2 = file
	bmp = CreateFileBitmap();
	icons.Add(*bmp);
	delete bmp;
}

VdtcTreeItemBase *wxVirtualDirTreeCtrl::OnCreateTreeItem(int type, const wxString &name)
{
	// return a default instance, no extra info needed in this item
	return new VdtcTreeItemBase(type, name);
}

void wxVirtualDirTreeCtrl::OnAddedItems(const wxTreeItemId &parent)
{
	VdtcTreeItemBase *t = (VdtcTreeItemBase *)GetItemData(parent);
	this->SetItemTextColour(parent, t->GetColour());
	return;
}

void wxVirtualDirTreeCtrl::OnDirectoryScanEnd(VdtcTreeItemBaseArray &items, const wxFileName &path)
{
	//If we are previewing and are in backup or secure
	if(!_IsSync && _Preview){
		//If the files should be excluded then set the correct colour, the actuall colour wil be set on the item later
		for (unsigned int i = 0; i < items.GetCount(); i++) {
			wxString strComplete = path.GetPath() + wxFILE_SEP_PATH + items.Item(i)->GetName();
			bool isdir = false;
			if(wxDirExists(strComplete)){
				isdir = true;
			}
			if (_Rules->ShouldExclude(strComplete, isdir)) {
				items.Item(i)->SetColour(wxColour(wxT("Red")));
			} 
		}
	}
	else if(_IsSync && _Preview){
		SyncData data(wxT("PreviewJob"));
		data.TransferFromForm(wxGetApp().MainWindow);
		data.SetSource(Normalise(data.GetSource()));
		data.SetDest(Normalise(data.GetDest()));
		wxString end;
		bool issource;
		if(this->GetId() == ID_SYNC_SOURCE_TREE){
			end = path.GetPath().Right(path.GetPath().Length() - data.GetSource().Length());
			issource = true;
		}
		else{
			end = path.GetPath().Right(path.GetPath().Length() - data.GetDest().Length());
			issource = false;
		}
		SyncPreview preview(data.GetSource() + end, data.GetDest() + end, &data, issource);
		items = preview.Execute();
	}
	return;
}

void wxVirtualDirTreeCtrl::NeatExpandAll(wxWindow* topwindow){
	wxBusyCursor cursor;
	topwindow->Freeze();
	this->ExpandAll();
	topwindow->Thaw();
}

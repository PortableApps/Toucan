/////////////////////////////////////////////////////////////////////////////////
// Author:      Jorgen Bodde & Steven Lamerton
// Copyright:   Copyright (C) 2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/object.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/artprov.h>
#include "virtualdirtreectrl.h"

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
		, _flags(wxVDTC_DEFAULT)
{
	// create an icon list for the tree ctrl
	_iconList = new wxImageList(16,16);

	_Preview = false;
	this->AddRoot(wxT("Hidden - you wont see this"));

	// reset to default extension list
	ResetExtensions();
}

wxVirtualDirTreeCtrl::~wxVirtualDirTreeCtrl()
{
	// first delete all VdtcTreeItemBase items (client data)
	DeleteAllItems();

	// delete the icons
	delete _iconList;
}

bool wxVirtualDirTreeCtrl::AddNewPath(const wxString &root, int flags)
{
	bool value;
	wxBusyInfo *bsy = 0;
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

	value = ::wxDirExists(root);
	if (value) {
		// call virtual handler to notify the derived class
		OnAddNewPath(root);

		// create a root item
		start = OnCreateTreeItem(VDTC_TI_ROOT, root);
		if (start) {
			wxFileName path;
			path.AssignDir(root);

			// call the add callback and find out if this root
			// may be added (later on)

			if (OnAddRoot(*start, path)) {
				// add this item to the tree, with info of the developer
				wxTreeItemId id = this->AppendItem(this->GetRootItem(), start->GetCaption(), start->GetIconId(), start->GetSelectedIconId(), start);
				//wxMessageBox(_("Appending"));
				// show a busy dialog
				if (_flags & (wxVDTC_RELOAD_ALL | wxVDTC_SHOW_BUSYDLG))
					bsy = new wxBusyInfo(wxT("Please wait, scanning directory..."), 0);

				// scan directory, either the smart way or not at all
				ScanFromDir(start, path, (wxVDTC_RELOAD_ALL & _flags ? -1 : VDTC_MIN_SCANDEPTH));
				//wxMessageBox(_("Finished Scan"));

				// expand root when allowed
				if (!(_flags & wxVDTC_NO_EXPAND))
					Expand(id);
			} else
				delete start; // sorry not succeeded
		}
	} 
	else{
		if(wxFileExists(root)){
			OnAddNewPath(root);
			wxFileName path(root);
			start = OnCreateTreeItem(VDTC_TI_FILE, root);
			if (OnAddFile(*start, path)) {
				// add this item to the tree, with info of the developer
				wxTreeItemId id = this->AppendItem(this->GetRootItem(), start->GetCaption(), start->GetIconId(), start->GetSelectedIconId(), start);
			}
		}
	}
	// delete busy info if present
	if (bsy)
		delete bsy;

	//wxMessageBox(this->GetItemText(this->GetRootItem()));
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
		// TODO: Maybe when a reload is issued, delete all items that are no longer present
		// in the tree (on disk) and check if all new items are present, else add them

		// if no items, then go iterate and get everything in this branch
		if (GetChildrenCount(item->GetId()) == 0) {
			//wxMessageBox(_("Creatin array"));
			VdtcTreeItemBaseArray addedItems;

			// now call handler, if allowed, scan this dir
			if (OnDirectoryScanBegin(path)) {
				// get directories
				GetDirectories(item, addedItems, path);

				// get files
				if (!(_flags & wxVDTC_NO_FILES))
					GetFiles(item, addedItems, path);

				// call handler that can do a last thing
				// before sort and anything else
				OnDirectoryScanEnd(addedItems, path);
				//wxMessageBox(_("Sortin"));
				// sort items
				if (addedItems.GetCount() > 0 && (_flags & wxVDTC_NO_SORT) == 0)
					SortItems(addedItems, 0, addedItems.GetCount()-1);
				//wxMessageBox(_("About to add"));
				AddItemsToTreeCtrl(item, addedItems);
				//wxMessageBox(_("Finished adding"));

				// call handler to tell that the items are on the tree ctrl
				//OnAddedItems(item->GetId());
			}
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

void wxVirtualDirTreeCtrl::GetFiles(VdtcTreeItemBase *parent, VdtcTreeItemBaseArray &items, const wxFileName &path)
{
	wxFileName fpath;
	wxString fname;
	VdtcTreeItemBase *item;

	fpath = path;

	// no nodes present yet, we should start scanning this dir
	// scan files first in this directory, with all extensions in this array

	for (size_t i = 0; i < _extensions.Count(); i++) {
		wxDir fdir(path.GetFullPath());

		if (fdir.IsOpened()) {
			bool bOk = fdir.GetFirst(&fname, _extensions[i], wxDIR_FILES | wxDIR_HIDDEN);
			while (bOk) {
				// TODO: Flag for double items

				item = AddFileItem(fname);
				if (item) {
					// fill it in, and marshall it by the user for info
					fpath.SetFullName(fname);
					if (OnAddFile(*item, fpath))
						items.Add(item);
					else
						delete item;
				}

				bOk = fdir.GetNext(&fname);
			}
		}
	}
}

void wxVirtualDirTreeCtrl::GetDirectories(VdtcTreeItemBase *parent, VdtcTreeItemBaseArray &items, const wxFileName &path)
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

				if (OnAddDirectory(*item, fpath))
					items.Add(item);
				else
					delete item;
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
	return a->GetCaption().CmpNoCase(b->GetCaption());
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
			wxTreeItemId idTemp = AppendItem(id, t->GetCaption(), t->GetIconId(), t->GetSelectedIconId(), t);
			OnAddedItems(idTemp);
		}

	}
}

wxFileName wxVirtualDirTreeCtrl::GetRelativePath(const wxTreeItemId &id)
{
	wxFileName value;
	wxCHECK(id.IsOk(), value);

	VdtcTreeItemBase *b = (VdtcTreeItemBase *)GetItemData(id);
	wxCHECK(b, value);

	AppendPathRecursively(b, value, false);

	return value;
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

wxTreeItemId wxVirtualDirTreeCtrl::ExpandToPath(const wxFileName &path)
{
	wxTreeItemId value((void *)0);
	wxFileName seekpath;
	wxArrayString paths;
	VdtcTreeItemBase *ptr;

	paths = path.GetDirs();

	// start in root section, and find the path sections that
	// match the sequence

	wxTreeItemId root = GetRootItem();
	if (root.IsOk()) {
		wxTreeItemId curr = root, id;
		for (size_t i = 0; i < paths.GetCount(); i++) {
			// scan for name on this level of children
			wxString currpath = paths[i];
			bool not_found = true;
			wxTreeItemIdValue cookie;

			id = GetFirstChild(curr, cookie);
			while (not_found && id.IsOk()) {
				ptr = (VdtcTreeItemBase *)GetItemData(id);
				not_found = !ptr->GetName().IsSameAs(currpath, false);

				// prevent overwriting id
				if (!not_found) {
					// we found the name, now to ensure there are more
					// names loaded from disk, we call ScanFromDir (it will abort anywayz
					// when there are items in the dir)

					if (ptr->IsDir()) {
						// TODO: This getfullpath might be a too high load, we can also
						// walk along with the path, but that is a bit more tricky.
						seekpath = GetFullPath(id);
						ScanFromDir(ptr, seekpath, VDTC_MIN_SCANDEPTH);
					}

					curr = id;
				} else
					id = GetNextChild(curr, cookie);
			}

			// now, if not found we break out
			if (not_found)
				return value;
		}

		// when we are here we are at the final node

		Expand(curr);
		value = curr;
	}

	return value;
}

bool wxVirtualDirTreeCtrl::IsRootNode(const wxTreeItemId &id)
{
	bool value = false;
	wxCHECK(id.IsOk(), value);

	VdtcTreeItemBase *b = (VdtcTreeItemBase *)GetItemData(id);
	if (b)
		value = b->IsRoot();

	return value;
}

bool wxVirtualDirTreeCtrl::IsDirNode(const wxTreeItemId &id)
{
	bool value = false;
	wxCHECK(id.IsOk(), value);

	VdtcTreeItemBase *b = (VdtcTreeItemBase *)GetItemData(id);
	if (b)
		value = b->IsDir();

	return value;
}

bool wxVirtualDirTreeCtrl::IsFileNode(const wxTreeItemId &id)
{
	bool value = false;
	wxCHECK(id.IsOk(), value);

	VdtcTreeItemBase *b = (VdtcTreeItemBase *)GetItemData(id);
	if (b)
		value = b->IsFile();

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
			ScanFromDir(t, GetFullPath(id), VDTC_MIN_SCANDEPTH);

			// TODO: When this scan gives no nodes, delete all children
			// and conclude that the scan could not be performed upon expansion
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

bool wxVirtualDirTreeCtrl::OnAddRoot(VdtcTreeItemBase &item, const wxFileName &name)
{
	// allow adding
	return true;
}

bool wxVirtualDirTreeCtrl::OnDirectoryScanBegin(const wxFileName &path)
{
	// allow all paths
	return true;
}

bool wxVirtualDirTreeCtrl::OnAddFile(VdtcTreeItemBase &item, const wxFileName &name)
{
	return true;
}

bool wxVirtualDirTreeCtrl::OnAddDirectory(VdtcTreeItemBase &item, const wxFileName &name)
{
	return true;
}

void wxVirtualDirTreeCtrl::OnAddNewPath(const wxString &root)
{
	// do nothing here, but it can be used to start initialisation
	// based upon the setting of the root (which means a renewal from the tree)
}

void wxVirtualDirTreeCtrl::OnAddedItems(const wxTreeItemId &parent)
{
	VdtcTreeItemBase *t = (VdtcTreeItemBase *)GetItemData(parent);
	this->SetItemTextColour(parent, t->GetColour());
	return;
}

void wxVirtualDirTreeCtrl::OnDirectoryScanEnd(VdtcTreeItemBaseArray &items, const wxFileName &path)
{
	//If the files should be excluded then set the correct colour, the actuall colour wil be set on the item later
	for (unsigned int i = 0; i < items.GetCount(); i++) {
		wxString strComplete = path.GetPath() + items.Item(i)->GetName();
		if (_Rules.ShouldExclude(strComplete, false)) {
			items.Item(i)->SetColour(wxColour(wxT("Red")));
		} else {
			items.Item(i)->SetColour(wxColour(wxT("Black")));
		}
	}
	//Make sure that we a in sync and that we are actually supposed to preview!
	if (_IsSync) {
		if (_Preview) {
			//Grab the path that we are in without the root
			wxString strPathNoRoot = path.GetPath().Right(path.GetPath().Length() - _Root.Length());
			//Create a wxFilename ut of the path we are in, but the opposite root
			wxFileName name(_RootOpp + strPathNoRoot);
			//Check if it is actually a directory
			if (wxDirExists(name.GetFullPath())) {
				wxString strFilename;
				wxDir dir(name.GetFullPath());
				bool blDir = dir.GetFirst(&strFilename);
				//If the path is ok, it should because because it was earlier, but you never can tell if you are going to get hit by a pesky neutrino!
				if (blDir) {
					//Loop through all of the files and folders in the directory
					do {
						//Set the path to the array items name, ensuring that it has a trailing slash
						wxString strPath = name.GetFullPath();
						if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
							strPath += wxFILE_SEP_PATH;       
						}
						//If the item in the array is a folder
						if (wxDirExists(strPath + strFilename)) {
							bool blExists = false;
							unsigned int j = 0;
							//Loop through the whole array, seeing if any of them are the same as in the folder in the opp dir
							for(unsigned int i = 0; i < items.GetCount(); i++){
								if(items.Item(i)->GetName() == strFilename){
									blExists = true;
									j = i;
								}
							}
							//If there is a match
							if(blExists){
								//If we shouldn't exclude it then set the item colour red to show it has been overwritten
								if(!_Rules.ShouldExclude(strPath + strFilename, true)){
									items.Item(j)->SetColour(wxColour(wxT("Red")));
								}
							}
							//There isnt a match so we need to add the new folder
							else{
								if(!_Rules.ShouldExclude(strPath + strFilename, true)){
									VdtcTreeItemBase *t = this->AddDirItem(strFilename);
									t->SetColour(wxColour(wxT("Blue")));
									items.Add(t);	
								}
							}
								
						}
						//If it is a file
						else {
							//Check to see if there is already a file with that name
							bool blExists = false;
							unsigned int j = 0;
							for(unsigned int i = 0; i < items.GetCount(); i++){
								if(items.Item(i)->GetName() == strFilename){
									blExists = true;
									j = i;
								}
							}
							//If there is
							if(blExists){
								//Make sure it shouldnt be excluded
								if(!_Rules.ShouldExclude(strPath + strFilename, false)){
									if(_Mode == _("Complete") || _Mode == _("Mirror (Copy)")){
										items.Item(j)->SetColour(wxColour(wxT("Red")));
									}
									else if(_Mode == _("Update") || _Mode == _("Mirror (Update)")){
										wxDateTime tmTo = wxFileModificationTime(_Root + items.Item(j)->GetName());
										wxDateTime tmFrom = wxFileModificationTime(strPath + strFilename);
										//Need to put in code to account for timezone settings
										//if(data.GetIgnoreDLS()){
										//	tmFrom.MakeTimezone(wxDateTime::Local, true);
										//}
										//I.E. strFrom is newer
										if(tmFrom.IsLaterThan(tmTo)){
											items.Item(j)->SetColour(wxColour(wxT("Green")));
										}
									}
								}
							}
							//If there isnt
							else{
								//Make sure it shouldnt be excluded
								if(!_Rules.ShouldExclude(strPath + strFilename, false)){
									VdtcTreeItemBase *t = this->AddFileItem(strFilename);
									t->SetColour(wxColour(wxT("Blue")));
									items.Add(t);	
								}
								else{
									VdtcTreeItemBase *t = this->AddFileItem(strFilename);
									t->SetColour(wxColour(wxT("Red")));
									items.Add(t);	
								}
							}
						}
					} while (dir.GetNext(&strFilename) );
				}
			}
		}
	}

	if(_Mode == _("Mirror (Copy)") || _Mode == _("Mirror (Update)")){
		//If the files should be excluded then set the correct colour, the actuall colour wil be set on the item later
		for (unsigned int i = 0; i < items.GetCount(); i++) {
		wxString strComplete = path.GetPath() + items.Item(i)->GetName();
			if(items.Item(i)->GetColour() == wxColour(wxT("Black"))){
				items.Item(i)->SetColour(wxColour(wxT("Grey")));
				//Set colour to grey
			}
		}
	}
	return;
}

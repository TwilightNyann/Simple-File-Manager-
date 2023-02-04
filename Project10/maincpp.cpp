#include <wx/wx.h>
#include <wx/dirctrl.h>
#include <wx/filename.h>
#include <wx/icon.h>
#include <wx/clipbrd.h>


class FileManagerFrame : public wxFrame {
public:
    FileManagerFrame() : wxFrame(NULL, wxID_ANY, "File Manager") {
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        wxGenericDirCtrl* dirCtrl = new wxGenericDirCtrl(this, wxID_ANY);
        sizer->Add(dirCtrl, 3, wxEXPAND);


        wxMenuBar* menuBar = new wxMenuBar();
        wxMenu* fileMenu = new wxMenu();
        wxMenu* aboutMenu = new wxMenu();
        fileMenu->Append(wxID_OPEN, "Open");
        fileMenu->Append(wxID_DELETE, "Delete");
        fileMenu->AppendSeparator();
        fileMenu->Append(wxID_EXIT, "Exit");
        aboutMenu->Append(wxID_ABOUT, "About");
        menuBar->Append(fileMenu, "File");
        menuBar->Append(aboutMenu, "Help");
        SetMenuBar(menuBar);
        CreateStatusBar();
        SetStatusText("Welcome to file manager, by Yura Yudin!");
        
        wxTextCtrl* pathTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_LEFT);
        sizer->Add(pathTextCtrl, 2, wxEXPAND);

        wxButton* button = new wxButton(this, wxID_ANY, "Rename");
        sizer->Add(button, 0, wxDOWN | wxALL, 5); 

        wxButton* button2 = new wxButton(this, wxID_ANY, "Copy");
        sizer->Add(button2, 0, wxDOWN | wxALL, 5);

        FileManagerFrame::SetBackgroundColour(wxColour(240, 240, 240));

        button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
            wxString oldPath = pathTextCtrl->GetValue();
        wxString newPath = wxGetTextFromUser("Enter new name:", "Rename", oldPath);

        if (newPath.empty()) return;

        if (!wxRenameFile(oldPath, newPath)) {
            wxMessageBox("Failed to rename the file or folder!", "Error", wxOK | wxICON_ERROR);
        }
            });
        button2->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
            wxString Path = pathTextCtrl->GetValue();
        wxTheClipboard->Clear();
        if (wxTheClipboard->Open()) {
            wxTheClipboard->SetData(new wxTextDataObject(Path));
            wxTheClipboard->Close();
        }

            });


        Bind(wxEVT_COMMAND_MENU_SELECTED, [=](wxCommandEvent& event) {
            if (event.GetId() == wxID_OPEN) {
                wxFileName filePath = dirCtrl->GetFilePath();
                if (wxFileExists(filePath.GetFullPath())) {
                    wxLaunchDefaultApplication(filePath.GetFullPath());
                }
                else {
                    wxMessageBox("File not found!", "Error", wxOK | wxICON_INFORMATION);
                }
            }
            else if (event.GetId() == wxID_EXIT) {
                Close();
            }
        }, wxID_OPEN, wxID_EXIT);


        Bind(wxEVT_COMMAND_MENU_SELECTED, [=](wxCommandEvent& event) {
            if (event.GetId() == wxID_DELETE) {
                wxFileName filePath = dirCtrl->GetPath();
                if (wxDirExists(filePath.GetFullPath())) {
                    wxRmdir(filePath.GetFullPath());
                    dirCtrl->Refresh();
                    wxMessageBox("File/Directory has been successfully deleted!", "Success");
                }
                else if (wxFileExists(filePath.GetFullPath())) {
                    wxRemoveFile(filePath.GetFullPath());
                    dirCtrl->Refresh();
                    wxMessageBox("File/Directory has been successfully deleted!", "Success");
                }
                else {
                    wxMessageBox("File/Directory not found", "Error");
                }
            }
            else if (event.GetId() == wxID_ABOUT) {
                wxMessageBox("This is a wxWidgets File Manager smple",
                    "About File Manager", wxOK | wxICON_INFORMATION);
            }
            }, wxID_ABOUT, wxID_DELETE);


        Bind(wxEVT_TREE_SEL_CHANGED, [=](wxTreeEvent& event) {
            wxFileName filePath = dirCtrl->GetPath();
        if (wxDirExists(filePath.GetFullPath())) {
            pathTextCtrl->SetValue(filePath.GetFullPath());
        }
        else {
            pathTextCtrl->SetValue(filePath.GetFullPath());
        }
            });

        SetSizer(sizer);
        SetSize(800, 600);
    }
    

};

class FileManagerApp : public wxApp {
public:
    bool OnInit() {
        FileManagerFrame* frame = new FileManagerFrame();
        wxIcon icon("C:/Users/ura21/Downloads/cat.ico", wxBITMAP_TYPE_ICO);
        frame->SetIcon(icon);
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(FileManagerApp);

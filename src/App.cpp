#include "App.h"
#include <fstream>
#include <cstdio>
#include <chrono>
#include <thread>

/*-------------------------------------------------------------------------------------------------------------------*/
//////Standard WxWidgets AppInitialization and Declaration of Main Frame -> MyFrame and Events/////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
//Define custom WxEvents for MyFrame
wxDEFINE_EVENT(IMAGEREADY, wxCommandEvent);
wxDEFINE_EVENT(CAMERACLOSED, wxCommandEvent);
wxDEFINE_EVENT(CAMERAOPEN, wxCommandEvent);

//Eventtable for Gui stuff
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::OnClose)
EVT_MENU(App_Quit, MyFrame::OnQuit)
EVT_MENU(App_About, MyFrame::OnAbout)
EVT_MENU(App_Save, MyFrame::OnSave)
EVT_MENU(App_Load, MyFrame::OnLoad)
EVT_MENU(App_StartCamera, MyFrame::OnStartCamera)
EVT_MENU(App_CloseCamera, MyFrame::OnCloseCamera)
EVT_MENU(App_DetectFace, MyFrame::OnDetectFace)
EVT_MENU(App_MakeRobotFace, MyFrame::OnMakeRobotFace)
EVT_BUTTON(App_StartCamera, MyFrame::OnStartCamera)
EVT_BUTTON(App_CloseCamera, MyFrame::OnCloseCamera)
EVT_BUTTON(App_Snap, MyFrame::OnSnap)
EVT_BUTTON(App_MakeRobotFaceBtn, MyFrame::OnMakeRobotFaceBtn)
EVT_COMMAND(wxID_ANY, IMAGEREADY, MyFrame::ImageReady)
EVT_COMMAND(wxID_ANY, CAMERACLOSED, MyFrame::CameraClosedEvent)
EVT_COMMAND(wxID_ANY, CAMERAOPEN, MyFrame::CameraOpenEvent)

wxEND_EVENT_TABLE()


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxInitAllImageHandlers();
    MyFrame* pMyFrame = new MyFrame("Webcam App");
    pMyFrame->Show(true);
    return true;
}

//Implementation of the Main Frame Class
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    //SetIcon(wxICON(_T("../files/RobotBtn.bmp")));

    // create a menu bar
    wxMenu* fileMenu = new wxMenu;
    m_pOpenCVMenu = new wxMenu;
    wxMenu* helpMenu = new wxMenu;

    helpMenu->Append(App_About, "&About\tF1", "Show about dialog");

    fileMenu->Append(App_Save, "&Save\tCtrl-S", "Save File");
    fileMenu->Append(App_Load, "&Load\tCtrl-L", "Load File");
    fileMenu->AppendSeparator();
    fileMenu->Append(App_Quit, "&Exit\tCtrl-Q", "Quit this program");

    m_pOpenCVMenu->Append(App_StartCamera, "&Start Camera\tCtrl-T", "Start Camera");
    m_pOpenCVMenu->Append(App_CloseCamera, "&Close Camera\tCtrl-R", "Close Camera");
    m_pOpenCVMenu->AppendSeparator();
    m_pOpenCVMenu->Append(App_DetectFace, "&Detect Face \tCtrl-J", "Detect Face");
    m_pOpenCVMenu->Append(App_MakeRobotFace, "&Make Robot Face \tCtrl-I", "Make Robot Face");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(m_pOpenCVMenu, "&OpenCV");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);
    
    m_pCamera = new Camera(this);
    m_pImgProcessor = new Imageprocessor();
    setLayout();
}

void MyFrame::setLayout() {
    wxPanel* pLiveImageViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    pLiveImageViewPanel->SetBackgroundColour(wxColour(0, 0, 0));

    wxPanel* pSnapImageViewPanel = nullptr;
    //Check if there is a file
    bool exist = MyFrame::checkExistance("../files/image.jpg");
    //If there is a file, add show image, else show empty panel
    if (exist) {
        pSnapImageViewPanel = new wxImagePanel(this, wxT("../files/image.jpg"), wxBITMAP_TYPE_JPEG);
    }
    else {
        pSnapImageViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        pLiveImageViewPanel->SetBackgroundColour(wxColour(128, 128, 128));
    }

    wxBoxSizer* pMainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* pImageSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* pButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    pMainSizer->Add(pImageSizer, 10, wxEXPAND | wxALL, 5);
    pMainSizer->Add(pButtonSizer, 1, wxEXPAND | wxALL, 5);

    pImageSizer->Add(pLiveImageViewPanel, 1, wxEXPAND | wxALL, 5);
    pImageSizer->Add(pSnapImageViewPanel, 1, wxEXPAND | wxALL, 5);
    pLiveImageViewPanel->Hide();

    //Add Button layout
    m_pButtonStartCamera = new wxButton(this, App_StartCamera, _T("Start Camera"), wxDefaultPosition, wxDefaultSize, 0);
    m_pButtonCloseCamera = new wxButton(this, App_CloseCamera, _T("Close Camera"), wxDefaultPosition, wxDefaultSize, 0);
    m_pButtonSnap = new wxButton(this, App_Snap, _T("Snap!"), wxDefaultPosition, wxDefaultSize, 0);
    
    wxBitmap bitmap;
    bitmap.LoadFile("../files/RobotBtn.bmp", wxBITMAP_TYPE_BMP);
    m_pButtonMakeRobot = new wxBitmapButton(this, App_MakeRobotFaceBtn, bitmap , wxDefaultPosition, wxDefaultSize,0);

    this -> refreshButtonLayout(m_bButtonFlag);

    pButtonSizer->Add(m_pButtonStartCamera, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonCloseCamera, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonSnap, 1, wxEXPAND | wxALL, 5);
    pButtonSizer->Add(m_pButtonMakeRobot, 1, wxEXPAND | wxALL, 5);
    this->SetSizerAndFit(pMainSizer);
    Layout();
}
/*-------------------------------------------------------------------------------------------------------------------*/
////////////////////////////////WXImagePanel Constructor, Functions Events/////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
//Define WxEvents for ImagePanel
BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(wxImagePanel::paintEvent)
EVT_SIZE(wxImagePanel::OnSize)
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) : wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = -1;
    h = -1;
}

void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC& dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    if (neww != w || newh != h)
    {
        resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
        w = neww;
        h = newh;
        dc.DrawBitmap(resized, 0, 0, false);
    }
    else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
}

void wxImagePanel::OnSize(wxSizeEvent& event) 
{
    Refresh();
    //skip the event.
    event.Skip();
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Implementation of Functions called by Gui Events//////////////////////////////////////////
//Currently the snaps are saved in a file called Image.jpg, this will be replaced with a pointer/buffer in the futer///
/*-------------------------------------------------------------------------------------------------------------------*/
void MyFrame::OnClose(wxCloseEvent& event)
{
    if ( !m_bFileSaved && checkExistance("../files/image.jpg"))
    {
        if ( wxMessageBox("The file has not been saved... continue closing?",
                          "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            return;
        }
    }
    std::remove("../files/image.jpg");
    Destroy();

}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    if ( !m_bFileSaved && checkExistance("../files/image.jpg"))
    {
        if ( wxMessageBox("The file has not been saved... continue closing?",
                          "Please confirm",
                          wxICON_QUESTION | wxYES_NO) == wxNO )
        {
            return;
        }
    }
    std::remove("../files/image.jpg");
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
    (
        "Welcome to my first App!\n"
        "\n"
        "This is my OpenCV Test App\n"
        "Have Fun!",
        wxVERSION_STRING,
        wxGetOsDescription()
    ),
        "About my OpenCv Test App ",
        wxOK | wxICON_INFORMATION,
        this);
}

void MyFrame::OnSave(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save File As _?"), wxEmptyString, wxEmptyString,
		_("JPEG (*.jpg)|*.jpg"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	if (SaveDialog->ShowModal() == wxID_OK) 
	{
		wxString CurrentDocPath = SaveDialog->GetPath();
        if (checkExistance("../files/image.jpg")){
            std::ifstream input( "../files/image.jpg", std::ios::binary );
            std::ofstream output(CurrentDocPath.ToStdString(), std::ios::binary );
            std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>( ), std::ostreambuf_iterator<char>(output));
            m_bFileSaved = true;
        }
	}

	SaveDialog->Destroy();
}

void MyFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog *OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
		_("JPEG (*.jpg)|*.jpg"),
		wxFD_OPEN, wxDefaultPosition);

	if (OpenDialog->ShowModal() == wxID_OK) 
	{
		wxString CurrentDocPath = OpenDialog->GetPath();
        std::string sName = CurrentDocPath.ToStdString();
        std::cout << sName << std::endl;
        std::ifstream input(sName, std::ios::binary );
        std::ofstream output("../files/image.jpg", std::ios::binary);
        std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>( ), std::ostreambuf_iterator<char>(output));
        m_bFileSaved = false;
	}
    OpenDialog->Destroy();
    refreshLayout();
}
/*-------------------------------------------------------------------------------------------------------------------*/
////////////////////Implementation of Functions called by Gui Events for Camera interaction////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
void MyFrame::OnStartCamera(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }
    
    bool result;

    if ((m_pCamera->isCameraClosed())) {
        result = m_pCamera->startCameraThread();
        if(!result) {
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("No Camera Available!"), wxString("Error"), wxOK|wxCENTRE);  
            errorDlg -> ShowModal();
        }
    }

}

void MyFrame::OnCloseCamera(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }
    if (!(m_pCamera->isCameraClosed())) {
        m_pCamera->closeCamera();
    }
}

void MyFrame::OnSnap(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera) {
        return;
    }
    if (!(m_pCamera->isCameraClosed())) {
        m_pCamera->snap(true);
        m_bFileSaved = false;
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////Implementation of Functions called by Gui Events for Imageprocessing//////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
//Testfunctions if Facedetection works, can be called on every Image in jpg. format -> will not be saved

void MyFrame::OnDetectFace(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pImgProcessor) {
        return;
    }

    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please type the name of the file, you want to load"), wxString("Load"));

    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxFName =  nameDlg -> GetValue();
        std::string fileName= "../files/" + std::string(wxFName.mb_str()) + ".jpg";
        if (MyFrame::checkExistance(fileName)){
            m_pImgProcessor -> detectFace(fileName,true);
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("File does not exist!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
    }
}

//Testfunctions if overlaying of images works works, can be called on every Image in jpg. format
void MyFrame::OnMakeRobotFace(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pImgProcessor) {
        return;
    }

    wxTextEntryDialog* nameDlg = new wxTextEntryDialog(this, wxString("Please type the name of the file, you want to load"), wxString("Load"));

    if ( nameDlg -> ShowModal() == wxID_OK )
    {
        wxString wxFName =  nameDlg -> GetValue();
        std::string fileName = "../files/" + std::string(wxFName.mb_str()) + ".jpg";
        if (MyFrame::checkExistance(fileName)){
            m_pImgProcessor -> makeRobotFace(fileName, true, false);
        }
        else{
            wxMessageDialog* errorDlg = new wxMessageDialog(this, wxString("File does not exist!"), wxString("Error"), wxOK|wxCENTRE);
            errorDlg -> ShowModal();
        }
    }
}

//Overlayed image will be saved
void MyFrame::OnMakeRobotFaceBtn(wxCommandEvent& WXUNUSED(event))
{
    if (!m_pCamera || !m_pImgProcessor) {
        return;
    }

    if (!(m_pCamera->isCameraClosed())) 
    {
        m_pCamera->snap(true);
        m_bImgProcess = true;
        m_bFileSaved = false;
    }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/////////////////////////////Implementation of Auxiliary Functions/////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------------------------*/
//Events
void MyFrame::CameraClosedEvent(wxCommandEvent& event)
{
    setButtonLayout();
    m_pOpenCVMenu->Enable(App_DetectFace, true);
    m_pOpenCVMenu->Enable(App_MakeRobotFace, true);
}

void MyFrame::CameraOpenEvent(wxCommandEvent& event)
{
    setButtonLayout();
    m_pOpenCVMenu->Enable(App_DetectFace, false);
    m_pOpenCVMenu->Enable(App_MakeRobotFace, false);
}

void MyFrame::ImageReady(wxCommandEvent& event)
{
    if(m_bImgProcess){
       m_pImgProcessor -> makeRobotFace("../files/Image.jpg", false, true);
       m_bImgProcess = false;
    }
    refreshLayout();
}

//Layout refresh
void MyFrame::refreshLayout()
{
    this-> DestroyChildren();
    setLayout();
}

void MyFrame::setButtonLayout()
{
    m_bButtonFlag = (m_bButtonFlag == true) ? false : true;
    refreshButtonLayout(m_bButtonFlag);
}

void MyFrame::refreshButtonLayout(bool state)
{
    if (m_bButtonFlag) {
        m_pButtonStartCamera->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonCloseCamera->SetBackgroundColour(wxColour(*wxRED));
        m_pButtonSnap->SetBackgroundColour(wxColour(*wxGREEN));
        m_pButtonMakeRobot->SetBackgroundColour(wxColour(*wxGREEN));
    }
    else {
        m_pButtonStartCamera->SetBackgroundColour(wxColour(*wxGREEN));
        m_pButtonCloseCamera->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonSnap->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
        m_pButtonMakeRobot->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
    }
}

//Filehandling
bool MyFrame::checkExistance(const std::string& name) 
{
    std::ifstream f(name.c_str());
    return f.good();
}

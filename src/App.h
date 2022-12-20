#ifndef APP_H_
#define APP_H_

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/fswatcher.h>
#include <wx/textdlg.h>
#include "Camera.h"
#include "Imageprocessor.h"
#include <thread>

//Forward Declaration
class Camera;

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;

private:
  
};

class wxImagePanel : public wxPanel
{
    wxImage image;
    wxBitmap resized;
    int w, h;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

private:
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    DECLARE_EVENT_TABLE()
};


class MyFrame : public wxFrame
{
public: /*Constructor*/
    MyFrame(const wxString& title);

public: /*Event Handling by Camera*/
    void ImageReady(wxCommandEvent& event);
    void CameraClosedEvent(wxCommandEvent& event);
    void CameraOpenEvent(wxCommandEvent& event);

public: /*Gui Functions*/
    void setButtonLayout();
    static bool checkExistance(const std::string& name);

private: /*Gui Functions*/
    void close(); 
    void setLayout();
    void refreshLayout();
    void refreshButtonLayout(bool state);

private:/*Non Gui Members*/
    Camera* m_pCamera{ nullptr };
    Imageprocessor* m_pImgProcessor{ nullptr };
    bool m_bButtonFlag{ false };
    bool m_bFileSaved{ false };
    bool m_bImgProcess{ false };

private:/*Gui Members*/
    wxImagePanel* m_pSnapImageViewPanel{ nullptr };
    wxButton* m_pButtonStartCamera{ nullptr };
    wxButton* m_pButtonCloseCamera{ nullptr };
    wxButton* m_pButtonSnap{ nullptr };
    wxBitmapButton* m_pButtonMakeRobot{ nullptr };
    wxMenu* m_pOpenCVMenu{ nullptr };

private:/*Event handling*/
    void OnClose(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnLoad(wxCommandEvent& event);
    void OnStartCamera(wxCommandEvent& event);
    void OnCloseCamera(wxCommandEvent& event);
    void OnDetectFace(wxCommandEvent& event);
    void OnMakeRobotFace(wxCommandEvent& event);
    void OnMakeRobotFaceBtn(wxCommandEvent& event);
    void OnSnap(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};
    //Event Declaration
    wxDECLARE_EVENT(IMAGEREADY, wxCommandEvent);
    wxDECLARE_EVENT(CAMERACLOSED, wxCommandEvent);
    wxDECLARE_EVENT(CAMERAOPEN, wxCommandEvent);

enum
{
    App_Quit = wxID_EXIT,
    App_About = wxID_ABOUT,
    App_Save = 1,
    App_Load,
    App_StartCamera = 10,
    App_CloseCamera,
    App_DetectFace,
    App_MakeRobotFace,
    App_Snap,
    App_MakeRobotFaceBtn
};

#endif /* APP_H_ */

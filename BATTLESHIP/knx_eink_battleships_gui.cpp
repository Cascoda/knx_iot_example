/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2023 Cascoda Ltd
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Cascoda Limited.
 *    integrated circuit in a product or a software update for such product, must
 *    reproduce the above copyright notice, this list of  conditions and the following
 *    disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Cascoda Limited nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * 4. This software, whether provided in binary or any other form must not be decompiled,
 *    disassembled, reverse engineered or otherwise modified.
 *
 *  5. This software, in whole or in part, must only be used with a Cascoda Limited circuit.
 *
 * THIS SOFTWARE IS PROVIDED BY CASCODA LIMITED "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CASCODA LIMITED OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/
// 2024-06-17 16:19:21.624451

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/cmdline.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <inttypes.h>

#ifdef WIN32
#include <winsock.h>
#endif

#define NO_MAIN
#include "knx_eink_battleships.h"
#include "api/oc_knx_dev.h"
#include "api/oc_knx_sec.h"
#include "api/oc_knx_fp.h"
#include "port/dns-sd.h"

enum
{
  RESET = wxID_HIGHEST + 1,   // ID for reset button in the menu
  RESET_TABLE = RESET + 1,    // ID for clear table button in the menu
  IA_TEXT = RESET_TABLE + 1,  // ID for internal address text 
  IID_TEXT = IA_TEXT + 1,     // ID for installation id text 
  PM_TEXT = IID_TEXT + 1,     // ID for programming mode text 
  LS_TEXT = PM_TEXT + 1,      // ID for load status text 
  HOSTNAME_TEXT = LS_TEXT + 1, // ID for hostname text 
  GOT_TABLE_ID = HOSTNAME_TEXT + 1, // ID for the Group object window
  PUB_TABLE_ID = GOT_TABLE_ID + 1, // ID for the publisher table window
  REC_TABLE_ID = PUB_TABLE_ID + 1, // ID for the recipient table window
  PARAMETER_LIST_ID = REC_TABLE_ID + 1, // ID for the parameter window
  AT_TABLE_ID = PARAMETER_LIST_ID + 1, // ID for the auth/at window
  CHECK_GA_DISPLAY = AT_TABLE_ID + 1 , // ga display check
  CHECK_IID_DISPLAY = CHECK_GA_DISPLAY + 1, // iid display check
  CHECK_GRPID_DISPLAY = CHECK_IID_DISPLAY + 1, // grpid display check
  CHECK_SLEEPY = CHECK_GRPID_DISPLAY + 1 , // sleepy check
  CHECK_PM = CHECK_SLEEPY + 1 , // programming mode check in menu bar
  DP_IDSENDSHOT = CHECK_PM + 1, // SendShot for /p/o_1_1
  DP_IDRECEIVESHOT = CHECK_PM + 2, // ReceiveShot for /p/o_1_2
  DP_IDSENDSHOTSTATUS = CHECK_PM + 3, // SendShotStatus for /p/o_1_3
  DP_IDRECEIVESHOTSTATUS = CHECK_PM + 4, // ReceiveShotStatus for /p/o_1_4
  DP_IDSENDREADY = CHECK_PM + 5, // SendReady for /p/o_1_5
  DP_IDRECEIVEREADY = CHECK_PM + 6 // ReceiveReady for /p/o_1_6
};

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
  { wxCMD_LINE_OPTION, "s", "serialnumber", "serial number", wxCMD_LINE_VAL_STRING },
  { wxCMD_LINE_NONE }
};

wxCmdLineParser* g_cmd;


//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

class CustomDialog : public wxDialog
{
public:
  CustomDialog(const wxString& title, const wxString& text);

private:
  void on_close(wxCommandEvent& event);
};

void CustomDialog::on_close(wxCommandEvent& event)
{
  this->Destroy();
}

CustomDialog::CustomDialog(const wxString& title, const wxString& text)
  : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(550, 300))
{
  int size_x = 520;
  int size_y = 300;

  wxPanel* panel = new wxPanel(this, -1);

  wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

  wxTextCtrl* tc = new wxTextCtrl(panel, -1, text, wxPoint(10, 10),
    wxSize(size_x, size_y), wxTE_MULTILINE | wxTE_READONLY);

  wxButton* closeButton = new wxButton(this, -1, wxT("Close"),
    wxDefaultPosition, wxDefaultSize);
  closeButton->Bind(wxEVT_BUTTON, &CustomDialog::on_close, this);

  hbox->Add(closeButton, 1, wxLEFT, 5);
  vbox->Add(panel, 1);
  vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

  SetSizerAndFit(vbox);
  Centre();
  ShowModal();
  Destroy();
}

//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
};

//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

class ScrolledWidgetsPane : public  wxScrolledWindow
{
public:
  wxFrame* m_parentFrame= NULL;

  ScrolledWidgetsPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id)
  {
    // the sizer will take care of determining the needed scroll size
    // (if you don't use sizers you will need to manually set the viewport size)
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    int x_width = 230; /* width of the widgets */
    int x_height = 25; /* height of the widgets */
    int border_size = 1; /* border size, size between the widgets */  
  // complex data type, as string
  char text_SENDSHOT[100];
  memset(text_SENDSHOT, 0, 100);
  app_str_expected_DPT_Uint_XY(2, text_SENDSHOT);
  mSENDSHOT = new wxTextCtrl(this, DP_IDSENDSHOT, text_SENDSHOT, wxPoint(0, 0), wxSize(x_width, x_height), wxTE_PROCESS_ENTER);
  char text_tt_SENDSHOT[100];
  memset(text_tt_SENDSHOT, 0, 100);
  app_str_expected_DPT_Uint_XY(1, text_tt_SENDSHOT);
  mSENDSHOT->SetToolTip(text_tt_SENDSHOT);
  mSENDSHOT->Enable(true);
  {
   
    wxTextCtrl* lSENDSHOT_text = new wxTextCtrl(this, -1, _T("SendShot ('/p/o_1_1') if.o "), wxPoint(0, 0), wxSize(x_width, x_height));
    lSENDSHOT_text->SetEditable(false);
    lSENDSHOT_text->SetToolTip("SendShot urn:knx:dpt.uint_XY ['urn:knx:dpa.65500.101'] ");
    // add sizer for the group.
    wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(lSENDSHOT_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    vsizer->Add(mSENDSHOT, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    // add the sizer
    sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  }
  // add callback
  mSENDSHOT->Bind(wxEVT_TEXT_ENTER, &ScrolledWidgetsPane::OnTextSENDSHOT, this);   
  // complex data type, as string
  char text_RECEIVESHOT[100];
  memset(text_RECEIVESHOT, 0, 100);
  app_str_expected_DPT_Uint_XY(2, text_RECEIVESHOT);
  mRECEIVESHOT = new wxTextCtrl(this, DP_IDRECEIVESHOT, text_RECEIVESHOT, wxPoint(0, 0), wxSize(x_width, x_height), wxTE_PROCESS_ENTER);
  char text_tt_RECEIVESHOT[100];
  memset(text_tt_RECEIVESHOT, 0, 100);
  app_str_expected_DPT_Uint_XY(1, text_tt_RECEIVESHOT);
  mRECEIVESHOT->SetToolTip(text_tt_RECEIVESHOT);
  mRECEIVESHOT->Enable(true);
  {
   
    wxTextCtrl* lRECEIVESHOT_text = new wxTextCtrl(this, -1, _T("ReceiveShot ('/p/o_1_2') if.i "), wxPoint(0, 0), wxSize(x_width, x_height));
    lRECEIVESHOT_text->SetEditable(false);
    lRECEIVESHOT_text->SetToolTip("ReceiveShot urn:knx:dpt.uint_XY ['urn:knx:dpa.65501.111'] ");
    // add sizer for the group.
    wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(lRECEIVESHOT_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    vsizer->Add(mRECEIVESHOT, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    // add the sizer
    sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  }   
  // complex data type, as string
  char text_SENDSHOTSTATUS[100];
  memset(text_SENDSHOTSTATUS, 0, 100);
  app_str_expected_DPT_Shot_Status(2, text_SENDSHOTSTATUS);
  mSENDSHOTSTATUS = new wxTextCtrl(this, DP_IDSENDSHOTSTATUS, text_SENDSHOTSTATUS, wxPoint(0, 0), wxSize(x_width, x_height), wxTE_PROCESS_ENTER);
  char text_tt_SENDSHOTSTATUS[100];
  memset(text_tt_SENDSHOTSTATUS, 0, 100);
  app_str_expected_DPT_Shot_Status(1, text_tt_SENDSHOTSTATUS);
  mSENDSHOTSTATUS->SetToolTip(text_tt_SENDSHOTSTATUS);
  mSENDSHOTSTATUS->Enable(true);
  {
   
    wxTextCtrl* lSENDSHOTSTATUS_text = new wxTextCtrl(this, -1, _T("SendShotStatus ('/p/o_1_3') if.o "), wxPoint(0, 0), wxSize(x_width, x_height));
    lSENDSHOTSTATUS_text->SetEditable(false);
    lSENDSHOTSTATUS_text->SetToolTip("SendShotStatus urn:knx:dpt.shot_Status ['urn:knx:dpa.65501.102'] ");
    // add sizer for the group.
    wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(lSENDSHOTSTATUS_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    vsizer->Add(mSENDSHOTSTATUS, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    // add the sizer
    sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  }
  // add callback
  mSENDSHOTSTATUS->Bind(wxEVT_TEXT_ENTER, &ScrolledWidgetsPane::OnTextSENDSHOTSTATUS, this);   
  // complex data type, as string
  char text_RECEIVESHOTSTATUS[100];
  memset(text_RECEIVESHOTSTATUS, 0, 100);
  app_str_expected_DPT_Shot_Status(2, text_RECEIVESHOTSTATUS);
  mRECEIVESHOTSTATUS = new wxTextCtrl(this, DP_IDRECEIVESHOTSTATUS, text_RECEIVESHOTSTATUS, wxPoint(0, 0), wxSize(x_width, x_height), wxTE_PROCESS_ENTER);
  char text_tt_RECEIVESHOTSTATUS[100];
  memset(text_tt_RECEIVESHOTSTATUS, 0, 100);
  app_str_expected_DPT_Shot_Status(1, text_tt_RECEIVESHOTSTATUS);
  mRECEIVESHOTSTATUS->SetToolTip(text_tt_RECEIVESHOTSTATUS);
  mRECEIVESHOTSTATUS->Enable(true);
  {
   
    wxTextCtrl* lRECEIVESHOTSTATUS_text = new wxTextCtrl(this, -1, _T("ReceiveShotStatus ('/p/o_1_4') if.i "), wxPoint(0, 0), wxSize(x_width, x_height));
    lRECEIVESHOTSTATUS_text->SetEditable(false);
    lRECEIVESHOTSTATUS_text->SetToolTip("ReceiveShotStatus urn:knx:dpt.shot_Status ['urn:knx:dpa.65500.112'] ");
    // add sizer for the group.
    wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
    vsizer->Add(lRECEIVESHOTSTATUS_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    vsizer->Add(mRECEIVESHOTSTATUS, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
    // add the sizer
    sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  } 
  //DP_IDSENDREADY bool if.o
  // if.o  ==> sensor == possible to change value in UI
  mSENDREADY = new wxButton(this, DP_IDSENDREADY, _T("SendReady"), wxPoint(0, 0), wxSize(x_width, x_height)); 
  mSENDREADY->Bind(wxEVT_BUTTON, &ScrolledWidgetsPane::OnPressedSendReady, this);
  {
    wxTextCtrl* lSENDREADY_text = new wxTextCtrl(this, -1, _T("SendReady ('/p/o_1_5')"), wxPoint(0, 0), wxSize(x_width, x_height));
    lSENDREADY_text->SetEditable(false);
    lSENDREADY_text->SetToolTip("SendReady urn:knx:dpt.start ['urn:knx:dpa.65500.103']  ");
    // add sizer for the group.
     wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
     vsizer->Add(lSENDREADY_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
     vsizer->Add(mSENDREADY, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
     // add the sizer
     sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  }           
  //DP_IDRECEIVEREADY bool if.i 
  mRECEIVEREADY = new wxCheckBox(this, DP_IDRECEIVEREADY, _T("ReceiveReady "), wxPoint(0, 0), wxSize(x_width, x_height)); 
  mRECEIVEREADY->Enable(false);
  {
    wxTextCtrl* lRECEIVEREADY_text = new wxTextCtrl(this, -1, _T("ReceiveReady ('/p/o_1_6') "), wxPoint(0, 0), wxSize(x_width, x_height));
    lRECEIVEREADY_text->SetEditable(false);
    lRECEIVEREADY_text->SetToolTip("ReceiveReady urn:knx:dpt.start ['urn:knx:dpa.65501.113']  ");
    // add sizer for the group.
     wxBoxSizer* vsizer = new wxBoxSizer(wxHORIZONTAL);
     vsizer->Add(lRECEIVEREADY_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
     vsizer->Add(mRECEIVEREADY, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
     // add the sizer
     sizer->Add(vsizer, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  }          

    this->SetSizer(sizer);
    // this part makes the scrollbars show up
    this->FitInside(); // ask the sizer about the needed size
    this->SetScrollRate(5, 5);
  }

  //--------------------------
  //--------------------------
  // complex datatype
  void OnTextSENDSHOT(wxCommandEvent& event);    
  // complex datatype
  void OnTextSENDSHOTSTATUS(wxCommandEvent& event);        
  void OnPressedSendReady(wxCommandEvent& event); 
  //DP_IDSENDSHOT 
  //type is complex! 
  wxTextCtrl* mSENDSHOT; 
  //DP_IDRECEIVESHOT 
  //type is complex! 
  wxTextCtrl* mRECEIVESHOT; 
  //DP_IDSENDSHOTSTATUS 
  //type is complex! 
  wxTextCtrl* mSENDSHOTSTATUS; 
  //DP_IDRECEIVESHOTSTATUS 
  //type is complex! 
  wxTextCtrl* mRECEIVESHOTSTATUS; 
  //DP_IDSENDREADY bool
  wxButton* mSENDREADY; // SendReady if.o  
  //DP_IDRECEIVEREADY bool
  wxCheckBox* mRECEIVEREADY ; // ReceiveReady if.i  
};    
void ScrolledWidgetsPane::OnPressedSendReady(wxCommandEvent& event)
{
  char url[] = "/p/o_1_5";
  char my_text[100];
  bool p = (bool)*app_get_DPT_Start_variable(url, NULL);
  if (p == true) {
    p = false;
  }
  else {
    p = true;
  }
  app_set_DPT_Start_variable(url, (DPT_Start*)&p);
  oc_do_s_mode_with_scope(2, url, "w");
  oc_do_s_mode_with_scope(5, url, "w");
  sprintf(my_text, "SendReady ('%s') pressed: %d", url, (int)p);
  this->m_parentFrame->SetStatusText(my_text);
}   
// Complex datatype for SendShot
void ScrolledWidgetsPane::OnTextSENDSHOT(wxCommandEvent& event)
{
  char url[] = "/p/o_1_1";
  char my_text[200];
  bool input_ok = true;
  DPT_Uint_XY dt_converted;
  
  sprintf(my_text, "invalid text ");
  wxString entered_str = mSENDSHOT->GetValue();
  wxCharBuffer buffer = entered_str.ToUTF8();
  char* text_as_char = buffer.data();
  int error = app_sscanf_DPT_Uint_XY(&dt_converted, text_as_char);
  if ( error == 0 ){
    if (input_ok) {
      // send out the message
      //dt_converted = (DPT_Uint_XY)converted;
      app_set_DPT_Uint_XY_variable(url, &dt_converted);
      oc_do_s_mode_with_scope(2, url, "w");
      oc_do_s_mode_with_scope(5, url, "w");
      sprintf(my_text, "Sensor SendShot (/p/o_1_1) ::  %s ", text_as_char);
    }
  } else {
    char txt[100];
    memset(txt, 0, 100);
    app_str_expected_DPT_Uint_XY(1, txt);
    strcat(my_text, " expected format: ");
    strcat(my_text, txt);
  }
  this->m_parentFrame->SetStatusText(my_text);
}
  
   
 
// Complex datatype for SendShotStatus
void ScrolledWidgetsPane::OnTextSENDSHOTSTATUS(wxCommandEvent& event)
{
  char url[] = "/p/o_1_3";
  char my_text[200];
  bool input_ok = true;
  DPT_Shot_Status dt_converted;
  
  sprintf(my_text, "invalid text ");
  wxString entered_str = mSENDSHOTSTATUS->GetValue();
  wxCharBuffer buffer = entered_str.ToUTF8();
  char* text_as_char = buffer.data();
  int error = app_sscanf_DPT_Shot_Status(&dt_converted, text_as_char);
  if ( error == 0 ){
    if (input_ok) {
      // send out the message
      //dt_converted = (DPT_Shot_Status)converted;
      app_set_DPT_Shot_Status_variable(url, &dt_converted);
      oc_do_s_mode_with_scope(2, url, "w");
      oc_do_s_mode_with_scope(5, url, "w");
      sprintf(my_text, "Sensor SendShotStatus (/p/o_1_3) ::  %s ", text_as_char);
    }
  } else {
    char txt[100];
    memset(txt, 0, 100);
    app_str_expected_DPT_Shot_Status(1, txt);
    strcat(my_text, " expected format: ");
    strcat(my_text, txt);
  }
  this->m_parentFrame->SetStatusText(my_text);
}
  
   
   

//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------
//----------------------------------------------------

class MyFrame : public wxFrame
{
public:
  MyFrame(char* serial_number);
private:
  void OnGroupObjectTable(wxCommandEvent& event);
  void OnPublisherTable(wxCommandEvent& event);
  void OnRecipientTable(wxCommandEvent& event);
  void OnParameterList(wxCommandEvent& event);
  void OnAuthTable(wxCommandEvent& event);
  void OnProgrammingMode(wxCommandEvent& event);
  void OnSleepyMode(wxCommandEvent& event);
  void OnReset(wxCommandEvent& event);
  void OnClearTables(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnTimer(wxTimerEvent& event);

  void updateInfoCheckBoxes();
  void updateInfoButtons();
  void updateTextButtons();
  void bool2text(bool on_off, char* text);
  void int2text(int value, char* text);
  void int2gatext(uint32_t value, char* text, bool as_ets=false);
  void int2grpidtext(uint64_t value, char* text, bool as_ets);
  void int2scopetext(uint32_t value, char* text);
  void double2text(double value, char* text);

  wxMenu* m_menuFile;
  wxMenu* m_menuDisplay;
  wxMenu* m_menuOptions;
  wxTimer m_timer;
  ScrolledWidgetsPane* m_scrolledwindow;
  
  // sleepy information
  int m_sleep_counter = 0;
  int m_sleep_seconds = 20;

  wxTextCtrl* m_ia_text;  // text control for internal address
  wxTextCtrl* m_iid_text; // text control for installation id
  wxTextCtrl* m_pm_text;  // text control for programming mode
  wxTextCtrl* m_ls_text;  // text control for load state
  wxTextCtrl* m_hostname_text; // text control for host name
  wxTextCtrl* m_secured_text; // text secure/not secure
};

wxIMPLEMENT_APP(MyApp);

/**
 * @brief initialization of the application
 * 
 * @return true 
 * @return false 
 */
bool MyApp::OnInit()
{
  int argc = wxAppConsole::argc;
  wxChar** argv = wxAppConsole::argv;

  g_cmd = new wxCmdLineParser(argc, argv);
  g_cmd->SetDesc(g_cmdLineDesc);
  g_cmd->Parse(true);

  wxString serial_number;
  if (g_cmd->Found("s", &serial_number)) {
  }
  MyFrame* frame = new MyFrame((char*)(serial_number.c_str()).AsChar());
  frame->SetVirtualSize(650, 300);
  frame->SetSize(700, 400);
  frame->Show(true);
  return true;
}

/**
 * @brief Construct a new My Frame:: My Frame object
 * 
 * @param str_serial_number 
 */
MyFrame::MyFrame(char* str_serial_number)
    : wxFrame(NULL, wxID_ANY, "KNX Battleships Demo")
{
  m_menuFile = new wxMenu;
  m_menuFile->Append(GOT_TABLE_ID, "List Group Object Table", "List the Group object table", false);
  m_menuFile->Append(PUB_TABLE_ID, "List Publisher Table", "List the Publisher table", false);
  m_menuFile->Append(REC_TABLE_ID, "List Recipient Table", "List the Recipient table", false);
  m_menuFile->Append(PARAMETER_LIST_ID, "List Parameters", "List the parameters of the device", false);
  m_menuFile->Append(AT_TABLE_ID, "List Auth/AT Table", "List the security data of the device", false);
  m_menuFile->Append(CHECK_PM, "Programming Mode", "Sets the application in programming mode", true);
  m_menuFile->Append(RESET_TABLE, "Reset (7) (Tables)", "Reset 7 (Reset to default without IA).", false);
  m_menuFile->Append(RESET, "Reset (2)(ex-factory)", "Reset 2 (Reset to default state)", false);
  m_menuFile->AppendSeparator();
  m_menuFile->Append(wxID_EXIT);
  // display menu
   // display menu
  m_menuDisplay = new wxMenu;
  m_menuDisplay->Append(CHECK_GA_DISPLAY, "GA 3-level (ETS)", "Displays the group addresses as GA 3-Level or as integer", true);
  m_menuDisplay->Check(CHECK_GA_DISPLAY, true);
  m_menuDisplay->Append(CHECK_GRPID_DISPLAY, "GRPID as partial ipv6 address (ETS)", "Displays the grpid as integer", true);
  m_menuDisplay->Check(CHECK_GRPID_DISPLAY, true);
  m_menuDisplay->Append(CHECK_IID_DISPLAY, "IID as partial ipv6 address (ETS)", "Displays the iid as integer", true);
  m_menuDisplay->Check(CHECK_IID_DISPLAY, true);
  // Option menu
  m_menuOptions = new wxMenu;
  m_menuOptions->Append(CHECK_SLEEPY, "Act as Sleepy Device", "Sleeps for 20 seconds", true);
  m_menuOptions->Check(CHECK_SLEEPY, false);
  // help menu
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  // full menu bar
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(m_menuFile, "&File");
  menuBar->Append(m_menuDisplay, "&Display");
  menuBar->Append(m_menuOptions, "&Options");
  menuBar->Append(menuHelp, "&Help");
  SetMenuBar( menuBar );
  CreateStatusBar();
  SetStatusText("Welcome to KNX Battleships Demo!");
  Bind(wxEVT_MENU, &MyFrame::OnReset, this, RESET);
  Bind(wxEVT_MENU, &MyFrame::OnClearTables, this, RESET_TABLE);
  Bind(wxEVT_MENU, &MyFrame::OnGroupObjectTable, this, GOT_TABLE_ID);
  Bind(wxEVT_MENU, &MyFrame::OnPublisherTable, this, PUB_TABLE_ID);
  Bind(wxEVT_MENU, &MyFrame::OnRecipientTable, this, REC_TABLE_ID);
  Bind(wxEVT_MENU, &MyFrame::OnParameterList, this, PARAMETER_LIST_ID);
  Bind(wxEVT_MENU, &MyFrame::OnAuthTable, this, AT_TABLE_ID);
  Bind(wxEVT_MENU, &MyFrame::OnProgrammingMode, this, CHECK_PM);
  Bind(wxEVT_MENU, &MyFrame::OnSleepyMode, this, CHECK_SLEEPY);
  Bind(wxEVT_MENU, &MyFrame::OnReset, this, RESET);
  Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
  int x_width = 350; /* width of the widgets */
  int x_height = 25; /* height of the widgets */
  int max_instances = 6;
  int max_dp_count = 6;
  int border_size = 1;
  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  
  m_scrolledwindow = new ScrolledWidgetsPane(this, wxID_ANY);
  sizer->Add(m_scrolledwindow, 1, wxEXPAND);
  this->SetSizer(sizer);
  m_scrolledwindow->m_parentFrame = this;
  
  if (strlen(str_serial_number) > 1) {
    app_set_serial_number(str_serial_number);
  }
  // initialize the KNX stack
  app_initialize_stack(); 
  // grid for the info of the device
  wxGridSizer* gridsizer = new wxGridSizer( 4, 2, border_size, border_size);
  // serial number
  char text[500];
  strcpy(text, "Device Serial Number: -sn ");
  oc_device_info_t* device = oc_core_get_device_info(0);
  strcat(text, oc_string(device->serialnumber));
  wxTextCtrl* Statictext;
  Statictext = new wxTextCtrl(this, wxID_ANY, text, wxPoint(0, 0), wxSize(x_width, x_height));
  Statictext->SetEditable(false);
  Statictext->SetToolTip("Serial number of this instance, can be changed on the commandline");
  gridsizer->Add(Statictext, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  
  /* QR code
    KNX:S:serno;P:password
    where:
    KNX: is a fixed prefix
    S: means a KNX serial number follows, serno itself is encoded as 
       12 upper-case hexadecimal characters
    P: means a password follows, password itself is just 
       the KNX IoT Point API password; 
       this works as the allowed password characters do not interfere 
       with the separator characters colon and semicolon and are in the Alphanumeric range.
  */
  char qrtext[500];
  strcpy(qrtext, "QR (ISO): 41S");
  strcat(qrtext, oc_string(device->serialnumber));
  strcat(qrtext, "+3ZPA:");
  strcat(qrtext, app_get_password());
  app_str_to_upper(qrtext);
  wxTextCtrl* Statictext2;
  Statictext2 = new wxTextCtrl(this, wxID_ANY, qrtext, wxPoint(0, 0), wxSize(x_width, x_height));
  Statictext2->SetEditable(false);
  Statictext2->SetToolTip("QR info in ISO format, can be copied paste into ETS");
  gridsizer->Add(Statictext2, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  // individual address
  sprintf(text, "IA: %d", device->ia);
  m_ia_text = new wxTextCtrl(this, IA_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  m_ia_text->SetEditable(false);
  m_ia_text->SetToolTip("Individual Address");
  gridsizer->Add(m_ia_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM  , FromDIP(border_size));
  // installation id
  sprintf(text, "IID: %lld", device->iid);
  m_iid_text = new wxTextCtrl(this, IID_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  m_iid_text->SetEditable(false);
  m_iid_text->SetToolTip("Installation IDentifier");
  gridsizer->Add(m_iid_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  // programming mode
  sprintf(text, "Programming Mode: %d", device->pm);
  m_pm_text = new wxTextCtrl(this, PM_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  m_pm_text->SetEditable(false);
  //m_pm_text->SetToolTip("Programming Mode");
  gridsizer->Add(m_pm_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  // installation id
  sprintf(text, "LoadState: %s", oc_core_get_lsm_state_as_string(device->lsm_s));
  m_ls_text = new wxTextCtrl(this, LS_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  m_ls_text->SetEditable(false);
  //m_ls_text->SetToolTip("LoadState, 'loaded' means in running state");
  gridsizer->Add(m_ls_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  // host name
#ifdef WIN32

  char hostname_str[50];
  int error = gethostname(hostname_str, 50);
  oc_core_set_device_hostname(0, hostname_str);
#endif
  sprintf(text, "host name: %s", oc_string(device->hostname));
  m_hostname_text = new wxTextCtrl(this, LS_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  m_hostname_text->SetEditable(false);
#ifdef WIN32
m_hostname_text->SetToolTip("Hostname of the PC");
#else
  m_hostname_text->SetToolTip("Hostname not supported");
#endif
  gridsizer->Add(m_hostname_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  if (app_is_secure()) {
    strcpy(text, app_get_password());
  }
  else {
    strcpy(text, "unsecured");
  }
  // this one should be rich text wxTE_RICH
  m_secured_text = new wxTextCtrl(this, LS_TEXT, text, wxPoint(0, 0), wxSize(x_width, x_height));
  gridsizer->Add(m_secured_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(border_size));
  m_secured_text->SetEditable(false);
  m_secured_text->SetToolTip("Password if the device is secure");
  if (app_is_secure() == false) {
    m_secured_text->SetStyle(0, 100, (wxTextAttr(*wxRED)));
  }
  // update the UI
  sizer->Add(gridsizer, 0, wxEXPAND | wxALL, FromDIP(border_size));
  this->SetSizer(sizer);
  // initialize the UI with values from the stack
  this->updateTextButtons();
  this->updateInfoButtons();
  this->updateInfoCheckBoxes();
  // start the timer for UI updates and stack polls
  m_timer.Bind(wxEVT_TIMER, &MyFrame::OnTimer, this);
  m_timer.Start(1, wxTIMER_CONTINUOUS);  // 1 millisecond interval
}

/**
 * @brief exit the application
 * 
 * @param event command triggered by the framework
 */
void MyFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

/**
 * @brief checks/unchecks the programming mode
 * 
 * @param event command triggered by the menu button
 */
void MyFrame::OnProgrammingMode(wxCommandEvent& event)
{
  int device_index = 0;
  SetStatusText("Changing programming mode");

  bool my_val = m_menuFile->IsChecked(CHECK_PM);
  oc_device_info_t* device = oc_core_get_device_info(0);
  device->pm = my_val;

  // update the UI
  this->updateTextButtons();
  // update mdns
  knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);
}


/**
 * @brief checks/unchecks the sleepy mode
 * 
 * @param event command triggered by the menu button
 */
void MyFrame::OnSleepyMode(wxCommandEvent& event)
{
 int device_index = 0;
  SetStatusText("Changing sleepy mode");

  bool my_sleepy = m_menuOptions->IsChecked(CHECK_SLEEPY);
  oc_device_info_t* device = oc_core_get_device_info(0);
  
  if (my_sleepy) {
    knx_service_sleep_period(20);
  } else {
    knx_service_sleep_period(0);
  }
  // update mdns
  knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);
}

/**
 * @brief update the text buttons
 * - IA
 * - Loadstate
 * - programming mode
 * - IID
 * - Hostname
 */
void MyFrame::updateTextButtons()
{

  char text[500];
  size_t device_index = 0;
  bool iid_conversion = m_menuDisplay->IsChecked(CHECK_IID_DISPLAY);

  // get the device data structure
  oc_device_info_t* device = oc_core_get_device_info(device_index);
  // update the text labels
  // ia_0 == AAxxxxxx = AA
  // ia_1 == xxAAxxxx = AA
  // ia_2 == xxxxAAAA = AAAA
  uint32_t ia = device->ia;
  uint32_t ia_o = (ia >> 12);
  uint32_t ia_1 = (ia >> 8) & 0xF;
  uint32_t ia_2 = (ia & 0x000000FF);
  sprintf(text, "IA: %d.%d.%d   [%d]", ia_o, ia_1, ia_2, device->ia);
  m_ia_text->SetLabelText(text);
  sprintf(text, "LoadState: %s", oc_core_get_lsm_state_as_string(device->lsm_s));
  m_pm_text->SetLabelText(text);
  sprintf(text, "Programming Mode : % d", device->pm);
  m_ls_text->SetLabelText(text);
  strcpy(text, "IID: ");
  this->int2grpidtext(device->iid, text, iid_conversion);
  m_iid_text->SetLabelText(text);
  sprintf(text, "host name: %s", oc_string(device->hostname));
  m_hostname_text->SetLabelText(text);
  // reset the programming mode to what the device has
  m_menuFile->Check(CHECK_PM, device->pm);
}

/**
 * @brief clear the tables of the device
 * 
 * @param event command triggered by button in the menu
 */
void MyFrame::OnClearTables(wxCommandEvent& event)
{
  int device_index = 0;
  SetStatusText("Clear Tables");
  // reset the device
  oc_knx_device_storage_reset(device_index, 7);
  // update the UI
  this->updateTextButtons();
}

/**
 * @brief reset the device
 * 
 * @param event command triggered by button in the menu
 */
void MyFrame::OnReset(wxCommandEvent& event)
{
  int device_index = 0;
  SetStatusText("Device Reset");
  // reset the device
  oc_knx_device_storage_reset(device_index, 2);
  // update the UI
  this->updateTextButtons();
}

/**
 * @brief shows the group object table in a window
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnGroupObjectTable(wxCommandEvent& event)
{
  int device_index = 0;
  char text[1024 * 5];
  char line[200];
  char windowtext[200];
  bool ga_conversion = m_menuDisplay->IsChecked(CHECK_GA_DISPLAY);

  strcpy(text, "");
  oc_device_info_t* device = oc_core_get_device_info(device_index);
  if (device == NULL) {
    return;
  }
  int total = oc_core_get_group_object_table_total_size();
  for (int index = 0; index < total; index++) {
    oc_group_object_table_t* entry = oc_core_get_group_object_table_entry(index);

    if (entry && entry->ga_len > 0) {
      sprintf(line, "Index %d \n", index);
      strcat(text, line);
      sprintf(line, "  id: '%d'  ", entry->id);
      strcat(text, line);
      sprintf(line, "  url: '%s' ", oc_string(entry->href));
      strcat(text, line);
      sprintf(line, "  cflags : '%d' ", (int)entry->cflags);
      oc_cflags_as_string(line, entry->cflags);
      strcat(text, line);
      strcpy(line,"  ga : [");
      for (int i = 0; i < entry->ga_len; i++) {
        this->int2gatext(entry->ga[i], line, ga_conversion);
      }
      strcat(line," ]\n");
      strcat(text, line);
    }
  }
  strcpy(windowtext, "Group Object Table  ");
  strcat(windowtext, oc_string(device->serialnumber));
  CustomDialog(windowtext, text);
  SetStatusText("List Group Object Table");
}

/**
 * @brief shows the Publisher table in a window
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnPublisherTable(wxCommandEvent& event)
{
  int device_index = 0;
  char text[1024 * 5];
  char line[200];
  char windowtext[200];
  bool ga_conversion = m_menuDisplay->IsChecked(CHECK_GA_DISPLAY);
  bool grpid_conversion = m_menuDisplay->IsChecked(CHECK_GRPID_DISPLAY);
  bool iid_conversion = m_menuDisplay->IsChecked(CHECK_IID_DISPLAY);

  strcpy(text, "");
  oc_device_info_t* device = oc_core_get_device_info(device_index);
  if (device == NULL) {
    return;
  }
  
  int total =  oc_core_get_publisher_table_size();
  for (int index = 0; index < total; index++) {
    oc_group_rp_table_t* entry = oc_core_get_publisher_table_entry(index);

    if (entry && entry->id >= 0) {
      sprintf(line, "Index %d \n", index);
      strcat(text, line);
      sprintf(line, "  id: '%d'  ", entry->id);
      strcat(text, line);
       if ( entry->ia >= 0) {
        sprintf(line, "  ia: '%d' ", entry->ia);
        strcat(text, line);
      }
      if ( entry->iid >= 0) {
        strcpy(line, "  iid: ");
        this->int2grpidtext(entry->iid, line, iid_conversion);
        strcat(text, line);
      }
      if ( entry->fid >= 0) {
        sprintf(line, "  fid: '%lld' ", entry->fid);
        strcat(text, line);
      }
      if ( entry->grpid > 0) {
        //sprintf(line, "  grpid: '%u' ", entry->grpid);
        strcpy(line, "  grpid: ");
        this->int2grpidtext(entry->grpid, line, grpid_conversion);
        strcat(text, line);
      }
      if (oc_string_len(entry->url) > 0) {
        sprintf(line, "  url: '%s' ", oc_string(entry->url));
        strcat(text, line);
      }
      if (oc_string_len(entry->path) > 0) {
        sprintf(line, "  path: '%s' ", oc_string(entry->path));
        strcat(text, line);
      }
      if (oc_string_len(entry->at) > 0){
        sprintf(line, "  at: '%s' ", oc_string(entry->at));
        strcat(text, line);
      }
      if ( entry->ga_len > 0) {
        strcpy(line,"  ga : [");
        for (int i = 0; i < entry->ga_len; i++) {
          this->int2gatext(entry->ga[i], line, ga_conversion);
        }
        strcat(line," ]\n");
        strcat(text, line);
      }
    }
  }
  strcpy(windowtext, "Publisher Table  ");
  strcat(windowtext, oc_string(device->serialnumber));
  CustomDialog(windowtext, text);
  SetStatusText("List Publisher Table");
}

/**
 * @brief shows the Recipient table in a window
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnRecipientTable(wxCommandEvent& event)
{
  int device_index = 0;
  char text[1024 * 5];
  char line[200];
  char windowtext[200];
  bool ga_conversion = m_menuDisplay->IsChecked(CHECK_GA_DISPLAY);
  bool grpid_conversion = m_menuDisplay->IsChecked(CHECK_GRPID_DISPLAY);
  bool iid_conversion = m_menuDisplay->IsChecked(CHECK_IID_DISPLAY);

  strcpy(text, "");
  oc_device_info_t* device = oc_core_get_device_info(device_index);
  if (device == NULL) {
    return;
  }

  int total =  oc_core_get_recipient_table_size();
  for (int index = 0; index < total; index++) {
    oc_group_rp_table_t* entry = oc_core_get_recipient_table_entry(index);

    if (entry && entry->id >= 0) {
      sprintf(line, "Index %d \n", index);
      strcat(text, line);
      sprintf(line, "  id: '%d'  ", entry->id);
      strcat(text, line);
      if ( entry->ia >= 0) {
        sprintf(line, "  ia: '%d' ", entry->ia);
        strcat(text, line);
      }
      if ( entry->iid >= 0) {
        strcpy(line, "  iid: ");
        this->int2grpidtext(entry->iid, line, iid_conversion);
        strcat(text, line);
      }
      if ( entry->fid >= 0) {
        sprintf(line, "  fid: '%lld' ", entry->fid);
        strcat(text, line);
      }
      if ( entry->grpid >= 0) {
        strcpy(line, "  grpid: ");
        this->int2grpidtext(entry->grpid, line, grpid_conversion);
        strcat(text, line);
      }
      if (oc_string_len(entry->url) > 0) {
        sprintf(line, "  url: '%s' ", oc_string(entry->url));
        strcat(text, line);
      }
      if (oc_string_len(entry->path) > 0){
        sprintf(line, "  path: '%s' ", oc_string(entry->path));
        strcat(text, line);
      }
      if (oc_string_len(entry->at) > 0){
        sprintf(line, "  at: '%s' ", oc_string(entry->at));
        strcat(text, line);
      }
      if ( entry->ga_len > 0) {
        strcpy(line,"  ga : [");
        for (int i = 0; i < entry->ga_len; i++) {
          this->int2gatext(entry->ga[i], line, ga_conversion);
        }
        strcat(line," ]\n");
        strcat(text, line);
      }
      sprintf(line, "  non: %d  mt: %d\n", entry->non, entry->mt);
      strcat(text, line);
    }
  }
  strcpy(windowtext, "Recipient Table  ");
  strcat(windowtext, oc_string(device->serialnumber));
  CustomDialog(windowtext, text);
  SetStatusText("List Recipient Table");
}
/**
 * @brief shows a window containing the parameters and current values of the application
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnParameterList(wxCommandEvent& event)
{
  int device_index = 0;
  char text[1024 + (200*1)];
  char line[200];
  char windowtext[200];

  strcpy(text, "");

  oc_device_info_t* device = oc_core_get_device_info(device_index);
  if (device == NULL) {
    return;
  }
  strcpy(windowtext, "Parameter List ");
  strcat(windowtext, oc_string(device->serialnumber));

  // start looping at 0.
  int index = 0;
  const char* url = app_get_parameter_url(index);
  if (url == NULL) {
    strcat(text, "no parameters in this device");
  }
  bool added = false;
  
  while (url) {
    sprintf(line, "\nIndex %02d ", index);
    strcat(text, line);
    sprintf(line, "  url : '%s'  ", url);
    strcat(text, line);
    const char* name = app_get_parameter_name(index);
    if (name) {
      sprintf(line, "  name: '%s'  ", app_get_parameter_name(index));
      strcat(text, line);
    }
    added = false;
    if (app_is_DPT_Param_Bool_url(url)) {
      DPT_Param_Bool param_value;
      memset(&param_value, 0, sizeof(param_value));
      app_get_DPT_Param_Bool_variable(url, &param_value);
      added = true;

      // Enumeration
      sprintf(line, "  value : '%d'  ", param_value);
      strcat(text, line);
    }
    if (app_is_DPT_Shot_Status_url(url)) {
      DPT_Shot_Status param_value;
      memset(&param_value, 0, sizeof(param_value));
      app_get_DPT_Shot_Status_variable(url, &param_value);
      added = true;


      // param_value.DPST_60004_1_F_1 of bool type
      sprintf(line, "  %s : '%d'  ", "DPST-60004-1_F-1", param_value.DPST_60004_1_F_1);
      strcat(text, line);
      if (!param_value.DPST_60004_1_F_1) {
        sprintf(line, " (%s) ", "Miss");
        strcat(text, line);
      }
      if (param_value.DPST_60004_1_F_1) {
        sprintf(line, " (%s) ", "Hit");
        strcat(text, line);
      }

      // param_value.DPST_60004_1_F_2 of bool type
      sprintf(line, "  %s : '%d'  ", "DPST-60004-1_F-2", param_value.DPST_60004_1_F_2);
      strcat(text, line);
      if (!param_value.DPST_60004_1_F_2) {
        sprintf(line, " (%s) ", "Floating");
        strcat(text, line);
      }
      if (param_value.DPST_60004_1_F_2) {
        sprintf(line, " (%s) ", "Sunk");
        strcat(text, line);
      }
      strcat(text, "\n");

      // param_value.DPST_60004_1_F_3 of Enumeration type
    }
    if (app_is_DPT_Start_url(url)) {
      DPT_Start param_value;
      memset(&param_value, 0, sizeof(param_value));
      app_get_DPT_Start_variable(url, &param_value);
      added = true;

      // bool
      sprintf(line, "  value : '%d'  ", param_value);
      strcat(text, line);
      if (!param_value) {
        sprintf(line, " (%s) ", "Stop");
        strcat(text, line);
      }
      if (param_value) {
        sprintf(line, " (%s) ", "Start");
        strcat(text, line);
      }
    }
    if (app_is_DPT_Uint_XY_url(url)) {
      DPT_Uint_XY param_value;
      memset(&param_value, 0, sizeof(param_value));
      app_get_DPT_Uint_XY_variable(url, &param_value);
      added = true;

      // param_value.DPST_60009_1_F_1 of unsigned int type
      sprintf(line, "  %s : '%d'  ", "DPST-60009-1_F-1", param_value.DPST_60009_1_F_1);
      strcat(text, line);
      strcat(text, "\n");

      // param_value.DPST_60009_1_F_2 of unsigned int type
      sprintf(line, "  %s : '%d'  ", "DPST-60009-1_F-2", param_value.DPST_60009_1_F_2);
      strcat(text, line);
    }

    if (added == false) {
      // type is boolean
      sprintf(line, " (%d) ", app_retrieve_bool_variable(url));
      strcat(text, line);
      {
        // type is int
        int value;
        if (app_retrieve_int_variable(url, &value)) {
          sprintf(line, " (%d) ", value);
          strcat(text, line);
        }
      }
    }

    //if (app_is_string_url(url)) {
    //  sprintf(line, "  value : '%s'  ", app_retrieve_string_variable(url));
    //  strcat(text, line);
    //}
    index++;
    url = app_get_parameter_url(index);
  }

  //wxMessageBox(text, windowtext,
  //  wxOK | wxICON_NONE);
  CustomDialog(windowtext, text);
  SetStatusText("List Parameters and their current set values");
}

/**
 * @brief shows the (loaded) auth/at table
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnAuthTable(wxCommandEvent& event)
{
  int device_index = 0;
  char text[1024 * 10];
  char line[500];
  bool ga_conversion = m_menuDisplay->IsChecked(CHECK_GA_DISPLAY);
  char windowtext[200];
  int max_entries = oc_core_get_at_table_size();
  int index = 1;

  oc_device_info_t* device = oc_core_get_device_info(device_index);
  if (device == NULL) {
    return;
  }

  strcpy(text, "");
  for (index = 0; index < max_entries; index++) {

    oc_auth_at_t* my_entry = oc_get_auth_at_entry(device_index, index);
    if (my_entry != NULL) {
      if (oc_string_len(my_entry->id)) {
        sprintf(line, "index : '%d' id = '%s' \n", index, oc_string(my_entry->id));
        strcat(text, line);
        sprintf(line, "  profile : %d (%s)\n", my_entry->profile,
          oc_at_profile_to_string(my_entry->profile));
        strcat(text, line);
        if (my_entry->profile == OC_PROFILE_COAP_DTLS) {
          if (oc_string_len(my_entry->sub) > 0) {
            sprintf(line, "    sub           : %s\n", oc_string(my_entry->sub));
            strcat(text, line);
          }
          if (oc_string_len(my_entry->kid) > 0) {
            sprintf(line, "  kid : %s\n", oc_string(my_entry->kid));
            strcat(text, line);
          }
        }
        if (my_entry->profile == OC_PROFILE_COAP_OSCORE ||my_entry->profile == OC_PROFILE_COAP_PASE) {
          if (oc_byte_string_len(my_entry->osc_id) > 0) {
            sprintf(line, "  osc_id [%d]: ", (int)oc_byte_string_len(my_entry->osc_id));
            strcat(text, line);
            char* ms = oc_string(my_entry->osc_id);
            int length = (int)oc_byte_string_len(my_entry->osc_id);
            for (int i = 0; i < length; i++) {
              sprintf(line, "%02x", (unsigned char)ms[i]);
              strcat(text, line);
            }
            sprintf(line, "\n");
            strcat(text, line);
          }
          if (oc_byte_string_len(my_entry->osc_ms) > 0) {
            sprintf(line, "  osc_ms [%d]: ",(int)oc_byte_string_len(my_entry->osc_ms));
            strcat(text, line);
            int length = (int)oc_byte_string_len(my_entry->osc_ms);
            char* ms = oc_string(my_entry->osc_ms);
            for (int i = 0; i < length; i++) {
              sprintf(line, "%02x", (unsigned char)ms[i]);
              strcat(text, line);
            }
            sprintf(line, "\n");
            strcat(text, line);
          }
          if (oc_byte_string_len(my_entry->osc_salt) > 0) {
            sprintf(line, "  osc_salt [%d]: ",(int)oc_byte_string_len(my_entry->osc_salt));
            strcat(text, line);
            int length = (int)oc_byte_string_len(my_entry->osc_salt);
            char* salt = oc_string(my_entry->osc_salt);
            for (int i = 0; i < length; i++) {
              sprintf(line, "%02x", (unsigned char)salt[i]);
              strcat(text, line);
            }
            sprintf(line, "\n");
            strcat(text, line);
          }
          if (oc_byte_string_len(my_entry->osc_contextid) > 0) {
            sprintf(line, "  osc_contextid (o)[%d]: ", (int)oc_byte_string_len(my_entry->osc_contextid));
            strcat(text, line);
            char* ms = oc_string(my_entry->osc_contextid);
            int length = (int)oc_byte_string_len(my_entry->osc_contextid);
            for (int i = 0; i < length; i++) {
              sprintf(line, "%02x", (unsigned char)ms[i]);
              strcat(text, line);
            }
            sprintf(line, "\n");
            strcat(text, line);
          }
          if (oc_string_len(my_entry->sub) > 0) {
            sprintf(line, "  sub : %s\n", oc_string(my_entry->sub));
            strcat(text, line);
          }
          if (my_entry->ga_len > 0) {
            sprintf(line, "  osc_ga : [");
            strcat(text, line);
            for (int i = 0; i < my_entry->ga_len; i++) {
              this->int2gatext(my_entry->ga[i], text, ga_conversion);
            }
            sprintf(line, " ]\n");
            strcat(text, line);
          } else {
            sprintf(line, "  scope : ");
            this->int2scopetext(my_entry->scope, line);
            strcat(text, line);
            strcat(text, "\n");
          }
        }
      }
    }
  }

  strcpy(windowtext, "Auth AT Table ");
  strcat(windowtext, oc_string(device->serialnumber));
  CustomDialog(windowtext, text);
  SetStatusText("List security entries");
}

/**
 * @brief shows static info about the application
 * 
 * @param event command triggered by a menu button
 */
void MyFrame::OnAbout(wxCommandEvent& event)
{
  char text[500 + (200* 6)];
  strcpy(text, "KNX Battleships Demo\n");
  strcat(text, "\nDevice Serial Number: ");
  oc_device_info_t* device = oc_core_get_device_info(0);
  strcat(text, oc_string(device->serialnumber));
  strcat(text,"\n");
  strcat(text,"manufacturer     : cascoda\n");
  strcat(text,"model            : KNX Battleships Demo eink\n");
  strcat(text,"hardware type    : 000000000000\n");
  strcat(text,"hardware version : [0, 4, 0]\n");
  strcat(text,"firmware version : [0, 4, 0]\n\n");
  
  strcat(text, "data points:\n");
  strcat(text,"url:/p/o_1_1 rt:urn:knx:dpa.65500.101 if:if.o inst:1 name:SendShot\n");
  strcat(text,"url:/p/o_1_2 rt:urn:knx:dpa.65501.111 if:if.i inst:1 name:ReceiveShot\n");
  strcat(text,"url:/p/o_1_3 rt:urn:knx:dpa.65501.102 if:if.o inst:1 name:SendShotStatus\n");
  strcat(text,"url:/p/o_1_4 rt:urn:knx:dpa.65500.112 if:if.i inst:1 name:ReceiveShotStatus\n");
  strcat(text,"url:/p/o_1_5 rt:urn:knx:dpa.65500.103 if:if.o inst:1 name:SendReady\n");
  strcat(text,"url:/p/o_1_6 rt:urn:knx:dpa.65501.113 if:if.i inst:1 name:ReceiveReady\n");
  strcat(text, "\n");
  
  strcat(text, "(c) Cascoda Ltd\n");
  strcat(text, "2024-06-17 16:19:21.624451");
  CustomDialog("About", text);
}

/**
 * @brief update the UI on the timer ticks
 * updates:
 * - check boxes
 * - info buttons
 * - text buttons
 * does a oc_main_poll to give a tick to the stack
 * takes into account if the device is sleepy
 * e.g. then it only does an poll each 20 seconds
 * @param event triggered by a timer
 */
void MyFrame::OnTimer(wxTimerEvent& event)
{
  bool do_poll = true;
  bool sleepy = m_menuOptions->IsChecked(CHECK_SLEEPY);
  // do whatever you want to do every millisecond here
  if (sleepy)
  {
    do_poll = false;
    m_sleep_counter++;
    if ((m_sleep_counter / 1000) > m_sleep_seconds) {
      // only do a poll each x (20) seconds
      do_poll = true;
      m_sleep_counter = 0;
    }
    if (oc_knx_device_in_programming_mode(0)) {
      // make sure that the device is reactive in programming mode
      // e.g. keep on polling
      do_poll = true;
    }
  }

  if (do_poll) {
    oc_clock_time_t next_event;
   next_event = oc_main_poll();
  }
  this->updateInfoCheckBoxes();
  this->updateInfoButtons(); 
  this->updateTextButtons();
}


/**
 * @brief update the UI e.g. check boxes in the UI
 * updates:
 * does a oc_main_poll to give a tick to the stack
 * 
 * @param event triggered by a timer
 */
void  MyFrame::updateInfoCheckBoxes()
{
  bool p;     
  p = *app_get_DPT_Start_variable("/p/o_1_6", NULL); // set toggle of ReceiveReady
  m_scrolledwindow->mRECEIVEREADY->SetValue(p); 

}

/**
 * @brief convert the boolean to text for display
 * 
 * @param on_off the boolean
 * @param text the text to add the boolean as text
 */
void MyFrame::bool2text(bool on_off, char* text)
{
  if (on_off) {
    strcat(text, " On");
  }
  else {
    strcat(text, " Off");
  }
}

/**
 * @brief convert the integer to text for display
 * 
 * @param value the integer
 * @param text the text to add info to
 */
void MyFrame::int2text(int value, char* text)
{
  char value_text[50];

  sprintf(value_text, " %d", value);
  strcat(text, value_text);
}

/**
 * @brief convert the group address to text for display
 * 
 * @param value the integer
 * @param text the text to add info to
 * @param as_ets the text as terminology as used in ets
 */
void MyFrame::int2gatext(uint32_t value, char* text, bool as_ets)
{
  char value_text[50];

  if (as_ets) {
    /*
    The so called Group Address structure correlates with its representation style in ETS,
    see also the relevant ETS Professional article.
    The information about the ETS Group Address representation style itself is NOT included in the Group Address.
    '3-level' = main/middle/sub
    main = D7+D6+D5+D4+D3 of the first octet (high address)
    middle = D2+D1+D0 of the first octet (high address)
    sub = the entire second octet (low address)
    ranges: main = 0..31, middle = 0..7, sub = 0..255
    */
    uint32_t ga = value;
    uint32_t ga_main = (ga >> 11);
    uint32_t ga_middle = (ga >> 8) & 0x7;
    uint32_t ga_sub = (ga & 0x000000FF);
    sprintf(value_text, " %lu/%lu/%lu", ga_main, ga_middle, ga_sub);
    strcat(text, value_text);
  } else {
    sprintf(value_text, " %lu", value);
    strcat(text, value_text);
  }
}

/**
 * @brief convert the scope to text for display
 * 
 * @param value the scope
 * @param text the text to add info too
 */
void MyFrame::int2scopetext(uint32_t value, char* text)
{
  char value_text[150];

  sprintf(value_text, " [%d]", value);
  strcat(text, value_text);
  // should be the same as 
  if (value & (1 << 1)) strcat(text, " if.i");
  if (value & (1 << 2)) strcat(text, " if.o");
  if (value & (1 << 3)) strcat(text, " if.g.s");
  if (value & (1 << 4)) strcat(text, " if.c");
  if (value & (1 << 5)) strcat(text, " if.p");
  if (value & (1 << 6)) strcat(text, " if.d");
  if (value & (1 << 7)) strcat(text, " if.a");
  if (value & (1 << 8)) strcat(text, " if.s");
  if (value & (1 << 9)) strcat(text, " if.ll");
  if (value & (1 << 10)) strcat(text, " if.b");
  if (value & (1 << 11)) strcat(text, " if.sec");
  if (value & (1 << 12)) strcat(text, " if.swu");
  if (value & (1 << 13)) strcat(text, " if.pm");
  if (value & (1 << 14)) strcat(text, " if.m");
}

/**
 * @brief convert the group id to text for display
 * 
 * @param value the group id
 * @param text the text to add info too
 * @param as_ets the text as terminology as used in ets
 */
void MyFrame::int2grpidtext(uint64_t value, char* text, bool as_ets)
{
  char value_text[50];

  if (as_ets) {
    /*
     create the multicast address from group and scope
     FF3_:FD__:____:____:(8-f)___:____
     FF35:30:<ULA-routing-prefix>::<group id>
        | 5 == scope
        | 3 == scope
     Multicast prefix: FF35:0030:  [4 bytes]
     ULA routing prefix: FD11:2222:3333::  [6 bytes + 2 empty bytes]
     Group Identifier: 8000 : 0068 [4 bytes ]
    */
    // group number to the various bytes
    uint8_t byte_1 = (uint8_t)value;
    uint8_t byte_2 = (uint8_t)(value >> 8);
    uint8_t byte_3 = (uint8_t)(value >> 16);
    uint8_t byte_4 = (uint8_t)(value >> 24);
    uint8_t byte_5 = (uint8_t)(value >> 32);

    if (byte_5 == 0) {
       sprintf(value_text, " %02x%02x:%02x%02x", byte_4, byte_3, byte_2, byte_1);
    }
    else {
      sprintf(value_text, " %02x:%02x%02x:%02x%02x", byte_5, byte_4, byte_3, byte_2, byte_1);
    }

    strcat(text, value_text);
  }
  else {
    //sprintf(text, "IID: %lld", device->iid);
    sprintf(value_text, " %" PRIu64 "\n", value);
    strcat(text, value_text);
  }
}

/**
 * @brief convert the double (e.g. float)  to text for display
 * 
 * @param value the value
 * @param text the text to add info too
 */
void MyFrame::double2text(double value, char* text)
{
  char new_text[200];
  sprintf(new_text," %f", value);
  strcat(text, new_text);
}

/**
 * @brief update the buttons
 * 
 */
void  MyFrame::updateInfoButtons()
{
  char text[200];
  bool p;
  int p_int;
  float f;
  double d;
  // name=ReceiveShot dpt=urn:knx:dpt.uint_XY if=if.i ctype= 
  {
    const DPT_Uint_XY* d = (const DPT_Uint_XY *)app_get_DPT_Uint_XY_variable(URL_RECEIVESHOT, NULL);
    // set text of ReceiveShot
    strcpy(text, "");
    app_sprintf_DPT_Uint_XY(d, text, 200);
    m_scrolledwindow->mRECEIVESHOT->SetLabel(text); 
  }
  // name=ReceiveShotStatus dpt=urn:knx:dpt.shot_Status if=if.i ctype= 
  {
    const DPT_Shot_Status* d = (const DPT_Shot_Status *)app_get_DPT_Shot_Status_variable(URL_RECEIVESHOTSTATUS, NULL);
    // set text of ReceiveShotStatus
    strcpy(text, "");
    app_sprintf_DPT_Shot_Status(d, text, 200);
    m_scrolledwindow->mRECEIVESHOTSTATUS->SetLabel(text); 
  }
  // name=ReceiveReady dpt=urn:knx:dpt.start if=if.i ctype=bool
  p = (bool)*app_get_DPT_Start_variable(URL_RECEIVEREADY, NULL);  
  // set text of ReceiveReady
  strcpy(text, "");
  this->bool2text(p, text);
  m_scrolledwindow->mRECEIVEREADY->SetLabel(text); 

}







// EdgeBrowserAppDlg.h : header file
//

#pragma once
#include "ComponentBase.h"
//#include "EdgeBrowserApp.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <string>
#include <vector>
#include <winnt.h>


#ifdef __windows__
#undef __windows__
#endif

#ifdef USE_WEBVIEW2_WIN10
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.ViewManagement.h>
namespace winrtComp = winrt::Windows::UI::Composition;
#endif

// CEdgeBrowserAppDlg dialog
class CEdgeBrowserAppDlg : public CDialog
{
// Construction
public:
	CEdgeBrowserAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDGEBROWSERAPP_DIALOG };
#endif

		static PCWSTR GetWindowClass();
		void InitializeWebView();
		void CloseWebView(bool cleanupUserDataFolder = false);
		HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
		HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
		virtual LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);
		void RunAsync(std::function<void(void)> callback);
		void ResizeEverything();
		bool ExecuteWebViewCommands(WPARAM wParam, LPARAM lParam);
		bool ExecuteAppCommands(WPARAM wParam, LPARAM lParam);
		HRESULT DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv);

		void OnSize(UINT a, int b, int c);

		ICoreWebView2Controller* GetWebViewController()
		{
			return m_controller.get();
		}
		ICoreWebView2* GetWebView()
		{
			return m_webView.get();
		}
		ICoreWebView2Environment* GetWebViewEnvironment()
		{
			return m_webViewEnvironment.get();
		}
		HWND GetMainWindow()
		{
			return this->GetSafeHwnd();
		}

// Implementation
protected:
	HICON m_hIcon;
	DWORD m_creationModeId = 0;
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2> m_webView;
	wil::com_ptr<IDCompositionDevice> m_dcompDevice;
	std::vector<std::unique_ptr<ComponentBase>> m_components;
	HWND m_mainWindow = nullptr;
	HINSTANCE g_hInstance;
	static constexpr size_t s_maxLoadString = 100;
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);

	template <class ComponentType> ComponentType* GetComponent();
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

template <class ComponentType, class... Args> void CEdgeBrowserAppDlg::NewComponent(Args&&... args)
{
	m_components.emplace_back(new ComponentType(std::forward<Args>(args)...));
}

template <class ComponentType> ComponentType* CEdgeBrowserAppDlg::GetComponent()
{
	for (auto& component : m_components)
	{
		if (auto wanted = dynamic_cast<ComponentType*>(component.get()))
		{
			return wanted;
		}
	}
	return nullptr;
}


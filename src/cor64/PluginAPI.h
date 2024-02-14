#ifndef COR64_COMMONPLUGINAPI_H
#define COR64_COMMONPLUGINAPI_H

#include "GLideN64_Cor64.h"
#include "ZilmarGFX_1_3_Cor64.h"
#include "../FrameBufferInfoAPI.h"

class APICommand;

class PluginAPI
{
public:
	// Common
	void MoveScreen(int /*_xpos*/, int /*_ypos*/) {}
	void ViStatusChanged() {}
	void ViWidthChanged() {}

	void ProcessDList();
	void ProcessRDPList();
	void RomClosed();
	int RomOpen();
	void ShowCFB();
	void UpdateScreen();
	int InitiateGFX(const COR64_GFX_INFO & _gfxInfo);
	void ChangeWindow();

	void FindPluginPath(wchar_t * _strPath);
	void GetUserDataPath(wchar_t * _strPath);
	void GetUserCachePath(wchar_t * _strPath);
#ifdef M64P_GLIDENUI
	void GetUserConfigPath(wchar_t * _strPath);
#endif // M64P_GLIDENUI
	bool isRomOpen() const { return m_bRomOpen; }

	// Zilmar
	void DllTest(void * /*_hParent*/) {}
	void DrawScreen() {}
	void CloseDLL(void) {}

	void CaptureScreen(char * _Directory);
	void DllConfig(void * _hParent);
	void GetDllInfo (PLUGIN_INFO * PluginInfo);
	void ReadScreen(void **_dest, long *_width, long *_height);

	void DllAbout(/*void * _hParent*/);

	// FrameBufferInfo extension
	void FBWrite(unsigned int addr, unsigned int size);
	void FBWList(FrameBufferModifyEntry *plist, unsigned int size);
	void FBRead(unsigned int addr);
	void FBGetFrameBufferInfo(void *pinfo);
// #else
// 	// MupenPlus
// 	void ResizeVideoOutput(int _Width, int _Height);
// 	void ReadScreen2(void * _dest, int * _width, int * _height, int _front);

// 	m64p_error PluginStartup(m64p_dynlib_handle _CoreLibHandle);
// #ifdef M64P_GLIDENUI
// 	m64p_error PluginConfig();
// #endif // M64P_GLIDENUI
// 	m64p_error PluginShutdown();
// 	m64p_error PluginGetVersion(
// 		m64p_plugin_type * _PluginType,
// 		int * _PluginVersion,
// 		int * _APIVersion,
// 		const char ** _PluginNamePtr,
// 		int * _Capabilities
// 	);
// 	void SetRenderingCallback(void (*callback)(int));

// 	// FrameBufferInfo extension
// 	void FBWrite(unsigned int addr, unsigned int size);
// 	void FBRead(unsigned int addr);
// 	void FBGetFrameBufferInfo(void *pinfo);
// #endif

	static PluginAPI & get();

private:
	PluginAPI()
		: m_bRomOpen(false)
	{}
	PluginAPI(const PluginAPI &) = delete;

	void _initiateGFX(const COR64_GFX_INFO & _gfxInfo) const;

	bool m_bRomOpen;
};

inline PluginAPI & api()
{
	return PluginAPI::get();
}

#endif // COR64_COMMONPLUGINAPI_H

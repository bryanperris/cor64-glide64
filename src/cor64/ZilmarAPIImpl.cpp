#include "GLideN64_Cor64.h"
#include "PluginAPI.h"
#include "../GLideN64.h"
#include "../GLideNUI/GLideNUI.h"
#include "../Config.h"
#include <DisplayWindow.h>
#include <Revision.h>
#include "N64.h"

#include "m64p_common.h"
#include "m64p_config.h"

#include <iostream>

ptr_ConfigGetSharedDataFilepath ConfigGetSharedDataFilepath = nullptr;
ptr_ConfigGetUserConfigPath ConfigGetUserConfigPath = nullptr;
ptr_ConfigGetUserDataPath ConfigGetUserDataPath = nullptr;
ptr_ConfigGetUserCachePath ConfigGetUserCachePath = nullptr;
ptr_ConfigOpenSection      ConfigOpenSection = nullptr;
ptr_ConfigDeleteSection ConfigDeleteSection = nullptr;
ptr_ConfigSaveSection ConfigSaveSection = nullptr;
ptr_ConfigSaveFile ConfigSaveFile = nullptr;
ptr_ConfigSetDefaultInt    ConfigSetDefaultInt = nullptr;
ptr_ConfigSetDefaultFloat  ConfigSetDefaultFloat = nullptr;
ptr_ConfigSetDefaultBool   ConfigSetDefaultBool = nullptr;
ptr_ConfigSetDefaultString ConfigSetDefaultString = nullptr;
ptr_ConfigGetParamInt      ConfigGetParamInt = nullptr;
ptr_ConfigGetParamFloat    ConfigGetParamFloat = nullptr;
ptr_ConfigGetParamBool     ConfigGetParamBool = nullptr;
ptr_ConfigGetParamString   ConfigGetParamString = nullptr;
ptr_ConfigExternalGetParameter ConfigExternalGetParameter = nullptr;
ptr_ConfigExternalOpen ConfigExternalOpen = nullptr;
ptr_ConfigExternalClose ConfigExternalClose = nullptr;

/* definitions of pointers to Core video extension functions */
ptr_VidExt_Init                  CoreVideo_Init = nullptr;
ptr_VidExt_Quit                  CoreVideo_Quit = nullptr;
ptr_VidExt_ListFullscreenModes   CoreVideo_ListFullscreenModes = nullptr;
ptr_VidExt_ListFullscreenRates   CoreVideo_ListFullscreenRates = nullptr;
ptr_VidExt_SetVideoMode          CoreVideo_SetVideoMode = nullptr;
ptr_VidExt_SetVideoModeWithRate  CoreVideo_SetVideoModeWithRate = nullptr;
ptr_VidExt_SetCaption            CoreVideo_SetCaption = nullptr;
ptr_VidExt_ToggleFullScreen      CoreVideo_ToggleFullScreen = nullptr;
ptr_VidExt_ResizeWindow          CoreVideo_ResizeWindow = nullptr;
ptr_VidExt_GL_GetProcAddress     CoreVideo_GL_GetProcAddress = nullptr;
ptr_VidExt_GL_SetAttribute       CoreVideo_GL_SetAttribute = nullptr;
ptr_VidExt_GL_GetAttribute       CoreVideo_GL_GetAttribute = nullptr;
ptr_VidExt_GL_SwapBuffers        CoreVideo_GL_SwapBuffers = nullptr;
ptr_VidExt_GL_GetDefaultFramebuffer CoreVideo_GL_GetDefaultFramebuffer = nullptr;

ptr_PluginGetVersion             CoreGetVersion = nullptr;

void(*renderCallback)(int) = nullptr;

EXPORT void CALL ConfigurePlugin ( MUPEN_VID_EXT vidExt ) {
    config.resetToDefaults();
    CoreVideo_Init = vidExt.CoreVideo_Init;
    CoreVideo_Quit = vidExt.CoreVideo_Quit;
    CoreVideo_ListFullscreenModes = vidExt.CoreVideo_ListFullscreenModes;
    CoreVideo_ListFullscreenRates = vidExt.CoreVideo_ListFullscreenRates;
    CoreVideo_SetVideoMode = vidExt.CoreVideo_SetVideoMode;
    CoreVideo_SetVideoModeWithRate = vidExt.CoreVideo_SetVideoModeWithRate;
    CoreVideo_SetCaption = vidExt.CoreVideo_SetCaption;
    CoreVideo_ToggleFullScreen = vidExt.CoreVideo_ToggleFullScreen;
    CoreVideo_ResizeWindow = vidExt.CoreVideo_ResizeWindow;
    CoreVideo_GL_GetProcAddress = vidExt.CoreVideo_GL_GetProcAddress;
    CoreVideo_GL_SetAttribute = vidExt.CoreVideo_GL_SetAttribute;
    CoreVideo_GL_GetAttribute = vidExt.CoreVideo_GL_GetAttribute;
    CoreVideo_GL_SwapBuffers = vidExt.CoreVideo_GL_SwapBuffers;
    CoreVideo_GL_GetDefaultFramebuffer = vidExt.CoreVideo_GL_GetDefaultFramebuffer;
}

void PluginAPI::DllAbout(/*void * _hParent*/)
{
	Config_LoadConfig();
	wchar_t strIniFolderPath[PLUGIN_PATH_SIZE];
	api().FindPluginPath(strIniFolderPath);
	RunAbout(strIniFolderPath);
}

void PluginAPI::CaptureScreen(char * _Directory)
{
	// dwnd().setCaptureScreen(_Directory);
}

void PluginAPI::DllConfig(void * _hParent)
{
	//Config_DoConfig(/*_hParent*/);
}

void PluginAPI::GetDllInfo(PLUGIN_INFO * PluginInfo)
{
	PluginInfo->Version = 0x103;
	PluginInfo->Type = PLUGIN_TYPE_GFX;
	sprintf(PluginInfo->Name, "%s", pluginNameWithRevision);
	PluginInfo->NormalMemory = FALSE;
	PluginInfo->MemoryBswaped = TRUE;
}

EXPORT int CALL InitiateGFX (COR64_GFX_INFO Gfx_Info) {
    return api().InitiateGFX(Gfx_Info);
}

EXPORT void CALL RomOpen (void)
{
	RDRAMSize = 0;
	api().RomOpen();
}

EXPORT void CALL CaptureScreen ( char * Directory )
{
	api().CaptureScreen(Directory);
}

EXPORT void CALL CloseDLL (void)
{
	api().CloseDLL();
}

EXPORT void CALL DllAbout ( void * hParent )
{
	api().DllAbout(/*hParent*/);
}

EXPORT void CALL DllConfig ( void * hParent )
{
	api().DllConfig(hParent);
}

EXPORT void CALL DllTest ( void * hParent )
{
	api().DllTest(hParent);
}

EXPORT void CALL DrawScreen (void)
{
	api().UpdateScreen();
}

EXPORT void CALL GetDllInfo ( PLUGIN_INFO * PluginInfo )
{
	api().GetDllInfo(PluginInfo);
}

EXPORT void CALL ReadScreen (void **dest, long *width, long *height)
{
	api().ReadScreen(dest, width, height);
}
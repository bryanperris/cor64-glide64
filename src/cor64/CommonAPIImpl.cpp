#include <algorithm>
#include <string>
#include <fstream>
#include "GLideN64_Cor64.h"
#include "PluginAPI.h"
#include "../RSP.h"
#include "GLideN64.h"
#include <Log.h>
#include "../Config.h"

#ifdef OS_WINDOWS
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#endif

static
void _cutLastPathSeparator(wchar_t * _strPath)
{
#ifdef OS_ANDROID
	const u32 bufSize = 512;
	char cbuf[bufSize];
	wcstombs(cbuf, _strPath, bufSize);
	std::string pluginPath(cbuf);
	std::string::size_type pos = pluginPath.find_last_of("/");
	mbstowcs(_strPath, pluginPath.c_str(), PLUGIN_PATH_SIZE);
#else
	std::wstring pluginPath(_strPath);
	std::replace(pluginPath.begin(), pluginPath.end(), L'\\', L'/');
	std::wstring::size_type pos = pluginPath.find_last_of(L"/");
	wcscpy(_strPath, pluginPath.substr(0, pos).c_str());
#endif
}

static
void _getWSPath(const char * _path, wchar_t * _strPath, bool cutLastPathSeperator = false)
{
	::mbstowcs(_strPath, _path, PLUGIN_PATH_SIZE);
	if (cutLastPathSeperator) {
		_cutLastPathSeparator(_strPath);
	}
}


int PluginAPI::InitiateGFX(const COR64_GFX_INFO & _gfxInfo)
{
    LOG(LOG_APIFUNC, "cor64 InitiateGFX");

	HEADER = _gfxInfo.HEADER;
	DMEM = _gfxInfo.DMEM;
	IMEM = _gfxInfo.IMEM;
	RDRAM = _gfxInfo.RDRAM;

	REG.MI_INTR = _gfxInfo.MI_INTR_REG;
	REG.DPC_START = _gfxInfo.DPC_START_REG;
	REG.DPC_END = _gfxInfo.DPC_END_REG;
	REG.DPC_CURRENT = _gfxInfo.DPC_CURRENT_REG;
	REG.DPC_STATUS = _gfxInfo.DPC_STATUS_REG;
	REG.DPC_CLOCK = _gfxInfo.DPC_CLOCK_REG;
	REG.DPC_BUFBUSY = _gfxInfo.DPC_BUFBUSY_REG;
	REG.DPC_PIPEBUSY = _gfxInfo.DPC_PIPEBUSY_REG;
	REG.DPC_TMEM = _gfxInfo.DPC_TMEM_REG;

	REG.VI_STATUS = _gfxInfo.VI_STATUS_REG;
	REG.VI_ORIGIN = _gfxInfo.VI_ORIGIN_REG;
	REG.VI_WIDTH = _gfxInfo.VI_WIDTH_REG;
	REG.VI_INTR = _gfxInfo.VI_INTR_REG;
	REG.VI_V_CURRENT_LINE = _gfxInfo.VI_V_CURRENT_LINE_REG;
	REG.VI_TIMING = _gfxInfo.VI_TIMING_REG;
	REG.VI_V_SYNC = _gfxInfo.VI_V_SYNC_REG;
	REG.VI_H_SYNC = _gfxInfo.VI_H_SYNC_REG;
	REG.VI_LEAP = _gfxInfo.VI_LEAP_REG;
	REG.VI_H_START = _gfxInfo.VI_H_START_REG;
	REG.VI_V_START = _gfxInfo.VI_V_START_REG;
	REG.VI_V_BURST = _gfxInfo.VI_V_BURST_REG;
	REG.VI_X_SCALE = _gfxInfo.VI_X_SCALE_REG;
	REG.VI_Y_SCALE = _gfxInfo.VI_Y_SCALE_REG;

	CheckInterrupts = _gfxInfo.CheckInterrupts;

	REG.SP_STATUS = _gfxInfo.SP_STATUS;

    config.video.windowedWidth = 640;
    config.video.windowedHeight = 480;
    config.frameBufferEmulation.bufferSwapMode = Config::BufferSwapMode::bsOnVerticalInterrupt;
    config.frameBufferEmulation.enable = false;

	return TRUE;
}

void PluginAPI::FindPluginPath(wchar_t * _strPath)
{
	if (_strPath == nullptr)
		return;
#ifdef OS_WINDOWS
	GetModuleFileNameW((HINSTANCE)&__ImageBase, _strPath, PLUGIN_PATH_SIZE);
	_cutLastPathSeparator(_strPath);
#elif defined(OS_LINUX)
	std::ifstream maps;
	std::string line;
	std::size_t loc;
	maps.open("/proc/self/maps");

	if (maps.is_open())
	{
		while (getline(maps, line))
		{
			loc = line.find('/');
			if (loc == std::string::npos)
				continue;

			line = line.substr(loc);

			if (line.find("GLideN64") != std::string::npos)
			{
				_getWSPath(line.c_str(), _strPath, true);
				maps.close();
				return;
			}
		}

		maps.close();
	}

	char path[512];
	int res = readlink("/proc/self/exe", path, 510);
	if (res != -1) {
		path[res] = 0;
		_getWSPath(path, _strPath, true);
	}
#elif defined(OS_MAC_OS_X)
#define MAXPATHLEN 256
	char path[MAXPATHLEN];
	uint32_t pathLen = MAXPATHLEN * 2;
	if (_NSGetExecutablePath(path, &pathLen) == 0) {
		_getWSPath(path, _strPath, true);
	}
#elif defined(OS_ANDROID)
	GetUserCachePath(_strPath);
#endif
}

void PluginAPI::GetUserDataPath(wchar_t * _strPath)
{
	FindPluginPath(_strPath);
}

void PluginAPI::GetUserCachePath(wchar_t * _strPath)
{
	FindPluginPath(_strPath);
}
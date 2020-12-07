#ifndef __DPI_H__
#define __DPI_H__
#pragma once

#ifndef DPI_ENUMS_DECLARED
typedef enum PROCESS_DPI_AWARENESS 
{
	//软件不支持DPI aware,请用DWM虚拟化实现,此时GetWindowRect获取的坐标是DWM缩放过的
	PROCESS_DPI_UNAWARE = 0,
	//软件支持DIP aware,但软件被放到不同的显示器上时,会用DWM虚拟化绽放
	PROCESS_SYSTEM_DPI_AWARE = 1,
	//不允许进行DWM虚拟化,开发者自己针对不同的显示器进行缩放
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE 
{
	/*有效DPI,包含DWM虚拟化绽放DPI*/
	MDT_EFFECTIVE_DPI = 0,
	/*角度DPI*/
	MDT_ANGULAR_DPI = 1,
	/*原始DPI*/
	MDT_RAW_DPI = 2,
	MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

#define DPI_ENUMS_DECLARED
#endif // (DPI_ENUMS_DECLARED)

#define MINITOR_DEFAULT_DPI (96)

namespace DuiLib
{
	class UILIB_API CDPI
	{
	public:
		CDPI(void);

	public:
		static int GetMainMonitorDPI();
		static int GetDPIOfMonitor(HMONITOR hMonitor);
		static int GetDPIOfMonitorNearestToPoint(POINT pt);

	public:
		PROCESS_DPI_AWARENESS GetDPIAwareness();
		BOOL SetDPIAwareness(PROCESS_DPI_AWARENESS Awareness);
		UINT GetDPI();

		RECT Scale(RECT rcRect);
		void Scale(RECT *pRect);
		POINT Scale(POINT ptPoint);
		void Scale(POINT *pPoint);
		SIZE Scale(SIZE szSize);
		void Scale(SIZE *pSize);
		int Scale(int iValue);

		int  ScaleBack(int iValue);
		void ScaleBack(RECT *pRect);

		UINT GetScale();
		void SetScale(UINT uDPI);
	private:
		int m_nScaleFactor;
		int m_nScaleFactorSDA;
		PROCESS_DPI_AWARENESS m_Awareness;
	};
}
#endif //__DPI_H__
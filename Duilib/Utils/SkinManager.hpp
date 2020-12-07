#ifndef _SKINMANAGER_H__
#define _SKINMANAGER_H__

#include "stdafx.h"
#include "observer_impl_base.h"

namespace DuiLib {

	struct SkinChangedParam
	{
		bool bColor;
		DWORD bkcolor;
		CDuiString bgimage;
	};

	typedef class ObserverImpl<BOOL, SkinChangedParam> SkinChangedObserver;
	typedef class ReceiverImpl<BOOL, SkinChangedParam> SkinChangedReceiver;

	class CSkinManager
	{
	public:
		static CSkinManager* GetInstance() { return &m_objSkinManager; }

		void AddReceiver(ReceiverImpl<BOOL, SkinChangedParam>* receiver)
		{
			m_SkinChangeObserver.AddReceiver(receiver);
		}

		void RemoveReceiver(ReceiverImpl<BOOL, SkinChangedParam>* receiver)
		{
			m_SkinChangeObserver.RemoveReceiver(receiver);
		}

		void Broadcast(SkinChangedParam param)
		{
			m_SkinChangeObserver.Broadcast(param);
		}

		void Notify(SkinChangedParam param)
		{
			m_SkinChangeObserver.Notify(param);
		}

	private:
		CSkinManager() {};
		CSkinManager(const CSkinManager&) = delete;//½ûÀà¸³Öµ
		CSkinManager& operator=(const CSkinManager&) = delete;//½ûÀà¿½±´

	private:
		static CSkinManager m_objSkinManager;
		SkinChangedObserver m_SkinChangeObserver;
	};
}

#endif /*_SKINMANAGER_H__*/


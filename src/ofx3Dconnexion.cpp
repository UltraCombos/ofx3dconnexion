#include "ofx3Dconnexion.h"

/* SpaceWare Specific Includes */
#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/
#include "siapp.h"     /* Required for siapp.lib symbols */

#include "virtualkeys.hpp"


shared_ptr<ofx3Dconnexion::Initializer> ofx3Dconnexion::s_sp_initer;

ofx3Dconnexion::Initializer::Initializer(int* p_out_ret)
{
#if defined(TARGET_WIN32)
	*p_out_ret = SiInitialize();
#endif
}

ofx3Dconnexion::Initializer::~Initializer()
{
	SiTerminate();
}

struct ofx3Dconnexion::Impler
{
	Impler() : h_dev(NULL) {}

	SiHdl h_dev;
	std::string str_dev_name;
};

ofx3Dconnexion::ofx3Dconnexion()
{
	enum { ImplerSize = sizeof(Impler) };
	static_assert(ImplerSize < MaxImplerSize, "");
	m_p_impler = ::new(m_impler_ins) Impler;
}

ofx3Dconnexion::~ofx3Dconnexion()
{
	m_p_impler->~Impler();
	exit();
}

bool ofx3Dconnexion::setup()
{
	int ret = mf_init();

	switch (ret)
	{
	case SPW_NO_ERROR:
		break;

	case SPW_DLL_LOAD_ERROR:
		ofLogError("ofx3Dconnexion::setup(): Could not load SiAppDll dll files");
		return false;

	default:
		ofLogError("ofx3Dconnexion::setup(): Could not init");
		return false;
	}
	
#if defined(TARGET_WIN32)
	HWND hwnd = ofGetWin32Window();
	SiOpenData open_data;

	SiOpenWinInit(&open_data, hwnd);    /* init Win. platform specific data  */

	do 
	{
		SiHdl h_dev = SiOpen("ofx3Dconnexion", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &open_data);      /* Handle to 3D Mouse Device */
		if (h_dev == NULL)
		{
			break;
		}

		m_p_impler->h_dev = h_dev;

		SiDeviceName dev_name;
		int ret =  SiGetDeviceName(h_dev, &dev_name);

		m_p_impler->str_dev_name = dev_name.name;

		ofAppBaseWindow* p_windows = ofGetWindowPtr();

		ofAddListener(p_windows->events().update, this, &ofx3Dconnexion::mf_update, OF_EVENT_ORDER_BEFORE_APP);

		return true;

	} while (0); //Failed

#endif

	mf_term();
	return false;
}


void ofx3Dconnexion::exit()
{
	if (m_p_impler->h_dev)
	{
		ofAppBaseWindow* p_windows = ofGetWindowPtr();
		ofRemoveListener(p_windows->events().update, this, &ofx3Dconnexion::mf_update, OF_EVENT_ORDER_BEFORE_APP);

		int ret = SiClose(m_p_impler->h_dev);
		m_p_impler->h_dev = NULL;
		m_p_impler->str_dev_name = "";
	}
	
	mf_term();
}

bool ofx3Dconnexion::is_work()
{
	return m_p_impler->h_dev != NULL;
}

std::string ofx3Dconnexion::get_device_name()
{
	return m_p_impler->str_dev_name;
}

ofx3Dconnexion::MotionEvent& ofx3Dconnexion::get_event_motion()
{
	return m_event_motion;
}

int ofx3Dconnexion::mf_init()
{
	int ret = SPW_NO_ERROR;
	if (s_sp_initer == nullptr)
	{
		s_sp_initer = shared_ptr<Initializer>(new Initializer(&ret));
	}

	m_sp_initer = s_sp_initer;
	return ret;
}

void ofx3Dconnexion::mf_term()
{
	m_sp_initer = nullptr;

	if (s_sp_initer == NULL)
	{
		return;
	}

	//Only self;
	if (s_sp_initer.use_count() == 1)
	{
		s_sp_initer = nullptr;
	}
}

void ofx3Dconnexion::mf_update(ofEventArgs & args)
{
#if defined(TARGET_WIN32)
	MSG msg;						/* incoming message to be evaluated */
	SiGetEventData event_data;		/* SpaceWare Event Data */
	SiSpwEvent     event_3dc;		/* SpaceWare Event */

	if (!is_work())
	{
		return;
	}

	if (!GetMessage(&msg, NULL, 0, 0))
	{
		return;
	}

	/* init Window platform specific data for a call to SiGetEvent */
	SiGetEventWinInit(&event_data, msg.message, msg.wParam, msg.lParam);
	if (SiGetEvent(m_p_impler->h_dev, SI_AVERAGE_EVENTS, &event_data, &event_3dc) != SI_IS_EVENT)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return;
	}

	//ofLog(OF_LOG_NOTICE, "type = %d", event_3dc.type);

	switch (event_3dc.type)
	{
	case SI_BUTTON_EVENT:
		//{
		//	int c0 = 0;
		//}
		break;

	case SI_MOTION_EVENT:
		/* process 3D mouse motion event */
		{
			MotionEventArgs args;
			args.translate.x = event_3dc.u.spwData.mData[SI_TX];
			args.translate.y = event_3dc.u.spwData.mData[SI_TY];
			args.translate.z = event_3dc.u.spwData.mData[SI_TZ];
			args.rotation.x = event_3dc.u.spwData.mData[SI_RX];
			args.rotation.y = event_3dc.u.spwData.mData[SI_RY];
			args.rotation.z = event_3dc.u.spwData.mData[SI_RZ];
			args.translate /= 350.f;
			args.rotation /= 350.f;
			ofNotifyEvent(m_event_motion, args, this);
		}
		break;

	case SI_ZERO_EVENT:
		/* process 3D mouse zero event */
		{
			int c0 = 0;
		}
		break;

	case SI_BUTTON_PRESS_EVENT:
		/* process button press event */
		{
			int c0 = 0;
		}
		break;

	case SI_BUTTON_RELEASE_EVENT:
		/* process button release event */
		{
			int c0 = 0;
		}
		break;

	case SI_DEVICE_CHANGE_EVENT:
		/* process 3D mouse device change event */
		{
			int c0 = 0;
		}
		break;

	case SI_CMD_EVENT:
		/* V3DCMD_* events */
		{
			int c0 = 0;
		}
		break;

	case SI_APP_EVENT:
		/* V3DCMD_* events */
		{
			int c0 = 0;
		}
		break;
	}
#endif
}


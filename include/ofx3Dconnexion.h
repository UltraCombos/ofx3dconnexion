#ifndef INCLUDE_OF_ADDONS_OFX3DCONNEXION_H_
#define INCLUDE_OF_ADDONS_OFX3DCONNEXION_H_

#include "ofMain.h"

class ofx3Dconnexion
{
public:
	struct MotionEventArgs
	{
		ofVec3f translate;
		ofVec3f rotation;
	};

	typedef ofEvent< MotionEventArgs > MotionEvent;

	ofx3Dconnexion();
	~ofx3Dconnexion();

	bool setup();
	void exit();

	bool is_work();

	std::string get_device_name();

	MotionEvent& get_event_motion();

private:
	struct Initializer
	{
		Initializer(int*);
		~Initializer();
	};

	struct Impler;

	static shared_ptr<Initializer> s_sp_initer;
	shared_ptr<Initializer> m_sp_initer;

	enum { MaxImplerSize = 64 };
	char m_impler_ins[MaxImplerSize];
	Impler* m_p_impler;
	MotionEvent m_event_motion;

	int mf_init();
	void mf_term();
	void mf_update(ofEventArgs & args);
};

#endif//INCLUDE_OF_ADDONS_OFX3DCONNEXION_H_
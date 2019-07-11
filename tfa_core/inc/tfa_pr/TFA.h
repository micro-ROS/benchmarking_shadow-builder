#ifndef __TFA_H__
#define __TFA_H__

#include <common/Status.h>

class TFA {
public:
	TFA();
	Status initTfa();

private:
	TFACore m_tfaCore;
};


#endif /* __TFA_H__*/

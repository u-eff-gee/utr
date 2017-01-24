#ifndef Physics_h
#define Physics_h 1

#include "G4VUserPhysicsList.hh"

class Physics: public G4VUserPhysicsList
{
public:
	Physics();
   	~Physics();
        

protected:
      	void ConstructParticle();
      	void ConstructProcess();
      	void ConstructEMPenelope();
	void ConstructEMLivermore();
      	void ConstructHPNeutron();
	void ConstructChargedParticle();
};

#endif

#ifndef AngularDistribution_h
#define AngularDistribution_h 1

class AngularDistribution{
public:
	AngularDistribution(){};
	~AngularDistribution(){};

	double AngDist(double theta, double phi, double *st, int nst, double *mix) const;
};

#endif

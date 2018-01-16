#include <iostream>
#include <cmath>

#include "AngularDistribution.hh"

using std::cout;
using std::endl;

// PI/(180 DEGREE_TO_RAD)
#define DEGREE_TO_RAD 0.017453292519943295

bool AngularDistribution::IsInside(double rand_theta, double rand_phi, double rand_w, double *st, int nst, double *mix) const {
	if (rand_w <= AngDist(rand_theta, rand_phi, st, nst, mix)){
		return true;
	}
	return false;
}

double AngularDistribution::AngDist(
    double theta, double phi, double *st,
    int nst, double *mix) const {

	if (nst == 3) {
		// 0.1^+ -> 0.1^+ -> 0.1^+
		// Wildcard for test distributions
		if (st[0] == 0.1 && st[1] == 0.1 && st[2] == 0.1) {
			if (theta >= 85. * DEGREE_TO_RAD && theta <= 95. * DEGREE_TO_RAD &&
			    ((phi >= 355. * DEGREE_TO_RAD && phi <= 360. * DEGREE_TO_RAD) ||
			     (phi >= 0. * DEGREE_TO_RAD && phi <= 5. * DEGREE_TO_RAD))) {
				return 1.;
			}

			return 0.;
		}

		// 0^+ -> 0^+ -> 0^+
		// Isotropic distribution
		if ((st[0] == 0. && st[1] == 0. && st[2] == 0.) ||
		    (st[0] == -0.1 && st[1] == -0.1 && st[2] == -0.1)) {
			return 1.;
		}

		// 0^+ -> 1^+ -> 0^+ or 0^- -> 1^- -> 0^-
		if ((st[0] == 0. && st[1] == 1. && st[2] == 0.) ||
		    (st[0] == -0.1 && st[1] == -1. && st[2] == -0.1)) {
			return 0.75 * (1 + pow(cos(theta), 2) +
			            pow(sin(theta), 2) * cos(2 * phi));
		}

		// 0^+ -> 1^- -> 0^+ or 0^- -> 1^- -> 0^-
		if ((st[0] == 0. && st[1] == -1. && st[2] == 0.) ||
		    (st[0] == -0.1 && st[1] == 1. && st[2] == -0.1)) {
			return 0.75 * (1 + pow(cos(theta), 2) -
			            pow(sin(theta), 2) * cos(2 * phi));
		}

		// 0^+ -> 2^+ -> 0^+ or 0^- -> 2^- -> 0^-
		if ((st[0] == 0. && st[1] == 2. && st[2] == 0.) ||
		    (st[0] == -0.1 && st[1] == -2. && st[2] == -0.1)) {
			return 0.625 * (2. + cos(2. * theta) + cos(4. * theta) -
			             2. * cos(2. * phi) * (1. + 2. * cos(2. * theta)) *
			                 pow(sin(theta), 2.));
		}

		// 0^+ -> 2^+ -> 2^+ or 0^- -> 2^- -> 2^-
		if ((st[0] == 0. && st[1] == 2. && st[2] == 2.) ||
		    (st[0] == -0.1 && st[1] == -2. && st[2] == -2.)) {
			return 1./(56.*(1. + mix[1]*mix[1]))*(
					49. - 20.4939*mix[1] + 65.*mix[1]*mix[1] + 
						pow(cos(theta), 2)*(21. + 61.4817*mix[1] + 5.*mix[1]*mix[1]*(-7. + 8.*cos(2.*theta))) +
						cos(2.*phi)*(21. + 61.4817*mix[1] - 5.*mix[1]*mix[1]*(-7. + 8.*cos(2.*theta)))*pow(sin(theta), 2)
				);
		}

		// 0^+ -> 1^- -> 2^+ or 0^- -> 1^+ -> 2^-
		if ((st[0] == 0. && st[1] == -1. && st[2] == 2.) ||
		    (st[0] == -0.1 && st[1] == 1. && st[2] == -2.)) {

			return 1. + ((0.07071067811865475 + 0.9486832980505138*mix[1] + 0.35355339059327373*pow(mix[1],2))*
					      (1.0606601717798212*cos(2.*phi)*(-1. + pow(cos(theta),2)) + 0.35355339059327373*(-1. + 3.*pow(cos(theta),2))))/
				    (1. + pow(mix[1],2));
		}

		// 0^+ -> 1^+ -> 2^+ or 0^- -> 1^- -> 2^-
		if ((st[0] == 0. && st[1] == 1. && st[2] == 2.) ||
		    (st[0] == -0.1 && st[1] == -1. && st[2] == -2.)) {

			return 1. + ((0.07071067811865475 + 0.9486832980505138*mix[1] + 0.35355339059327373*pow(mix[1],2))*
					      (-1.0606601717798212*cos(2.*phi)*(-1. + pow(cos(theta),2)) + 0.35355339059327373*(-1. + 3.*pow(cos(theta),2))))/
				    (1. + pow(mix[1],2));
		}

		// 1.5^+ -> 2.5^- -> 1.5^+ or 1.5^- -> 2.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -2.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			return ((1. + 0.9999999999999998*pow(mix[0],2))*(1. + 0.9999999999999998*pow(mix[1],2)) -
			      0.5*(0.37416573867739406 + (-1.8973665961010275 - 0.19090088708030317*mix[1])*mix[1])*
			            ((0.37416573867739406 + (1.8973665961010275 - 0.19090088708030317*mix[0])*mix[0])*(1. - 3.*pow(cos(theta),2)) +
				             0.5727026612409095*(-1.959999999999999 + mix[0]*(3.313004678535784 + 1.*mix[0]))*cos(2.*phi)*(-1. + pow(cos(theta),2))) -
				         0.31098153547133134*pow(mix[0],2)*pow(mix[1],2)*
					       (-0.6000000000000001 + 6.*pow(cos(theta),2) - 7.*pow(cos(theta),4) +
						        cos(2.*phi)*(1. - 8.*pow(cos(theta),2) + 7.*pow(cos(theta),4))))/((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));
		}

		// 1.5^+ -> 2.5^+ -> 1.5^+ or 1.5^- -> 2.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 2.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == -2.5 && st[2] == -1.5)) {

			return ((1. + 0.9999999999999998*pow(mix[0],2))*(1. + 0.9999999999999998*pow(mix[1],2)) -
			      0.5*(0.37416573867739406 + (-1.8973665961010275 - 0.19090088708030317*mix[1])*mix[1])*
			            ((0.37416573867739406 + (1.8973665961010275 - 0.19090088708030317*mix[0])*mix[0])*(1. - 3.*pow(cos(theta),2)) -
				             0.5727026612409095*(-1.959999999999999 + mix[0]*(3.313004678535784 + 1.*mix[0]))*cos(2.*phi)*(-1. + pow(cos(theta),2))) +
				         0.31098153547133134*pow(mix[0],2)*pow(mix[1],2)*
					       (0.6000000000000001 - 6.*pow(cos(theta),2) + 7.*pow(cos(theta),4) +
						        cos(2.*phi)*(1. - 8.*pow(cos(theta),2) + 7.*pow(cos(theta),4))))/((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));

		}

		// 1.5^+ -> 1.5^+ -> 1.5^+ or 1.5^- -> 1.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 1.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == -1.5 && st[2] == -1.5)) {

			return 1. + (0.04*(1. + 3.872983346207417*mix[1])*(-1.*(-1. + 3.872983346207417*mix[0])*(1. + 3.*cos(2.*theta)) +
						        2.*(3. + 3.872983346207417*mix[0])*cos(2.*phi)*pow(sin(theta),2)))/((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));
		}

		// 1.5^+ -> 1.5^- -> 1.5^+ or 1.5^- -> 1.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -1.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			return 1. + (0.04*(1. + 3.872983346207417*mix[1])*(-1.*(-1. + 3.872983346207417*mix[0])*(1. + 3.*cos(2.*theta)) -
						        2.*(3. + 3.872983346207417*mix[0])*cos(2.*phi)*pow(sin(theta),2)))/((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));
		}

		// 0.5^- -> 1.5^- -> 0.5^- or 0.5^+ -> 1.5^+ -> 0.5^+
		if ((st[0] == -0.5 && st[1] == -1.5 && st[2] == -0.5) ||
		    (st[0] == 0.5 && st[1] == 1.5 && st[2] == 0.5)) {

			return 1. + (0.125*(-1. + 3.4641016151377544*mix[1] + pow(mix[1],2))*
					      (-1.*(1. + 3.4641016151377544*mix[0] - 1.*pow(mix[0],2))*(-1. + 3.*pow(cos(theta),2)) +
					               (-3. + 3.4641016151377544*mix[0] + 3.*pow(mix[0],2))*cos(2.*phi)*pow(sin(theta),2)))/
				    ((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));
		}

		// 0.5^- -> 1.5^+ -> 0.5^- or 0.5^+ -> 1.5^- -> 0.5^+
		if ((st[0] == -0.5 && st[1] == 1.5 && st[2] == -0.5) ||
		    (st[0] == 0.5 && st[1] == -1.5 && st[2] == 0.5)) {

			return 1. - (0.125*(-1. + 3.4641016151377544*mix[1] + pow(mix[1],2))*
					      ((1. + 3.4641016151377544*mix[0] - 1.*pow(mix[0],2))*(-1. + 3.*pow(cos(theta),2)) +
					               (-3. + 3.4641016151377544*mix[0] + 3.*pow(mix[0],2))*cos(2.*phi)*pow(sin(theta),2)))/
				    ((1. + pow(mix[0],2))*(1. + pow(mix[1],2)));

		}

} else if (nst >= 4) {
		cout
		    << "Warning: AngularDistributionGenerator:: Required spin sequence "
		       "not found."
		    << endl;
		return 0.;

	} else {
		cout
		    << "Warning: AngularDistributionGenerator:: Required spin sequence "
		       "not found."
		    << endl;
		return 0.;
	}

	return 0.;
}

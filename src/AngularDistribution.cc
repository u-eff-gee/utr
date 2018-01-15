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
			return 1. / ((1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1])) *
			        (1.16071 + mix[1] * (0.298807 + mix[1]) +
			         (0.0892857 + (2.09165 + 0.25 * mix[1]) * mix[1]) *
			             cos(2. * phi) +
			         pow(cos(theta), 2) *
			             (-1.33929 + (-5.37853 - 0.375 * mix[1]) * mix[1] +
			              (-1.51786 +
			               (-9.56183 - 0.875 * mix[1]) * mix[1] *
			                   cos(2. * phi))) +
			         pow(cos(theta), 4) *
			             (1.42857 + (7.47018 + 0.625 * mix[1]) * mix[1] +
			              (1.42857 + (7.47018 + 0.625 * mix[1]) * mix[1]) *
			                  cos(2. * phi)));
			}

		// 0^+ -> 1^- -> 2^+ or 0^- -> 1^+ -> 2^-
		if ((st[0] == 0. && st[1] == -1. && st[2] == 2.) ||
		    (st[0] == -0.1 && st[1] == 1. && st[2] == -2.)) {

			    return 1. / (1. + pow(mix[1], 2)) *
			        (0.975 + (-0.33541 + 0.875 * mix[1]) * mix[1] +
			         (-0.075 + (-1.00623 - 0.375 * mix[1]) * mix[1]) *
			             cos(2. * phi) +
			         pow(cos(theta), 2) *
			             (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1] +
			              (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1]) *
			                  cos(2. * phi)));	
		}

		// 0^+ -> 1^+ -> 2^+ or 0^- -> 1^- -> 2^-
		if ((st[0] == 0. && st[1] == 1. && st[2] == 2.) ||
		    (st[0] == -0.1 && st[1] == -1. && st[2] == -2.)) {

			    return 1. / (1. + pow(mix[1], 2)) *
			        (0.975 + (-0.33541 + 0.875 * mix[1]) * mix[1] +
			         (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1]) *
			             cos(2. * phi) +
			         pow(cos(theta), 2) *
			             (0.075 + (1.00623 + 0.375 * mix[1]) * mix[1] +
			              (-0.075 + (-1.00623 - 0.375 * mix[1]) * mix[1]) *
			                  cos(2. * phi)));
		}

		// 1.5^+ -> 2.5^- -> 1.5^+ or 1.5^- -> 2.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -2.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			    return 1. / ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2))) *
			        ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2)) -
			         0.5 * (0.374166 - (1.89737 + 0.190901 * mix[1]) * mix[1]) *
			             ((0.374166 + (1.89737 - 0.190901 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) +
			              0.572703 * (-1.96 + mix[0] * (3.313 + mix[0])) *
			                  (pow(cos(theta), 2) - 1.) *
			                  cos(2 * phi)) +
			         0.0621963 * pow(mix[0], 2) * pow(mix[1], 2) *
			             (3. +
			              pow(cos(theta), 4) *
			                  (35. - 35. * cos(2 * phi)) -
			              5. * cos(2 * phi) +
			              pow(cos(theta), 2) *
			                  (-30. + 40. * cos(2 * phi))));
		}

		// 1.5^+ -> 2.5^+ -> 1.5^+ or 1.5^- -> 2.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 2.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == -2.5 && st[2] == -1.5)) {

			    return 1. / ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2))) *
			        ((1. + pow(mix[0], 2)) * (1. + pow(mix[1], 2)) -
			         0.5 * (0.374166 - (1.89737 + 0.190901 * mix[1]) * mix[1]) *
			             ((0.374166 + (1.89737 - 0.190901 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) -
			              0.572703 * (-1.96 + mix[0] * (3.313 + mix[0])) *
			                  (pow(cos(theta), 2) - 1.) *
			                  cos(2 * phi)) +
			         0.0621963 * pow(mix[0], 2) * pow(mix[1], 2) *
			             (3. +
			              pow(cos(theta), 2) *
			                  (-30. - 40. * cos(2 * phi)) +
			              5. * cos(2 * phi) +
			              pow(cos(theta), 4) *
			                  (35. + 35. * cos(2 * phi))));
		}

		// 1.5^+ -> 1.5^+ -> 1.5^+ or 1.5^- -> 1.5^- -> 1.5^-
		if ((st[0] == 1.5 && st[1] == 1.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == -1.5 && st[2] == -1.5)) {

			    return 1. / (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1])) *
			        (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1]) -
			         (0.4 + 1.54919 * mix[1]) *
			             ((0.4 - 1.54919 * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) +
			              (-1.2 - 1.54919 * mix[0]) * cos(2. * phi) *
			                  pow(sin(theta), 2)));
		}

		// 1.5^+ -> 1.5^- -> 1.5^+ or 1.5^- -> 1.5^+ -> 1.5^-
		if ((st[0] == 1.5 && st[1] == -1.5 && st[2] == 1.5) ||
		    (st[0] == -1.5 && st[1] == 1.5 && st[2] == -1.5)) {

			    return 1. / (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1])) *
			        (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1]) -
			         (0.4 + 1.54919 * mix[1]) *
			             ((0.4 - 1.54919 * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) +
			              (1.2 + 1.54919 * mix[0]) * cos(2. * phi) *
			                  pow(sin(theta), 2)));
		}

		// 0.5^- -> 1.5^- -> 0.5^- or 0.5^+ -> 1.5^+ -> 0.5^+
		if ((st[0] == -0.5 && st[1] == -1.5 && st[2] == -0.5) ||
		    (st[0] == 0.5 && st[1] == 1.5 && st[2] == 0.5)) {

			    return 1. / (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1])) *
			        (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1]) -
			         (0.5 + (-1.73205 - 0.5 * mix[1]) * mix[1]) *
			             ((0.5 + (1.73205 - 0.5 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) +
			              1.5 * (-1. + mix[0] * (1.1547 + mix[0])) *
			                  (-1. + pow(cos(theta), 2)) *
			                  cos(2. * phi)));
		}

		// 0.5^- -> 1.5^+ -> 0.5^- or 0.5^+ -> 1.5^- -> 0.5^+
		if ((st[0] == -0.5 && st[1] == 1.5 && st[2] == -0.5) ||
		    (st[0] == 0.5 && st[1] == -1.5 && st[2] == 0.5)) {

			    return 1. / (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1])) *
			        (2. * (1. + mix[0] * mix[0]) * (1. + mix[1] * mix[1]) -
			         (0.5 + (-1.73205 - 0.5 * mix[1]) * mix[1]) *
			             ((0.5 + (1.73205 - 0.5 * mix[0]) * mix[0]) *
			                  (1. - 3. * pow(cos(theta), 2)) -
			              1.5 * (-1. + mix[0] * (1.1547 + mix[0])) *
			                  (-1. + pow(cos(theta), 2)) *
			                  cos(2. * phi)));
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

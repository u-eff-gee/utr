"""`Schiff-Formula` for the energy distribution of thin-target bremsstrahlung"""

from numpy import arctan, log, sqrt
from numpy import e as eulers_number

mu = 0.5109989461  # Electron rest mass times c**2 in MeV
C = 183.0 / sqrt(eulers_number)
ALPHA = 0.0072973525693  # Fine structure constant
ELECTRON_CHARGE = 1.6021766208e-19  # Electron charge in Coulomb
ELECTRON_CHARGE_SQUARED = 2.5669699242381073e-38


def auxiliary_b(E, E0, k, Z):
    """Auxiliary function b
    
    Defined immediately below Eq. (3) in [1]
    """
    return 2.0 * E0 * E * Z ** (1.0 / 3.0) / (C * mu * k)


def auxiliary_M(x, E, E0, k, Z):
    """
    Auxiliary function M
    
    Eq. (2) in [1]
    """
    return 1.0 / (
        (mu * k / (2 * E0 * E)) ** 2 + (Z ** (1.0 / 3.0) / (C * (x * x + 1.0))) ** 2
    )


def schiff(k, E0, Z, e2=ELECTRON_CHARGE_SQUARED):
    """
    Energy distribution of thin-target bremsstrahlung
    
    This is an analytical expression for the energy spectrum of the bremsstrahlung
    emitted by an electron beam impinging on a thin target as derived by L.I. Schiff [1].
        
    [1] L.I. Schiff, Energy-Angle Distribution of Thin Target Bremsstrahlung,
    Phys. Rev. 83 (1951)
        
    Parameters
    ----------
    k: float
        Photon energy in MeV
    E0: float
        Energy of the electron beam in MeV
    Z: int
        Proton number of the target material
    e2: float
        Square of the electron charge. It appears squared (i.e. e**4) as a \
    global factor and can be used to scale the distribution.

    Returns
    -------
    float: Intensity of the spectrum at the energy E
    """

    if not isinstance(Z, int):
        raise ValueError("Z must be an integer")
    if e2 < 0.0:
        raise ValueError("e2 must be >= 0")

    E = E0 - k
    b = auxiliary_b(E, E0, k, Z)
    b_squared = b * b
    inverse_b = 1.0 / b
    inverse_b_squared = inverse_b * inverse_b
    M0 = auxiliary_M(0.0, E, E0, k, Z)

    return (
        2.0 * Z * Z * ALPHA * (e2 / (mu)) ** 2 / k * (
            ((E0 * E0 + E * E) / (E0 * E0) - 2.0 * E / (3.0 * E0))
            * (log(M0) + 1.0 - 2.0 * inverse_b * arctan(b))
            + E / E0 * (
                2.0 * inverse_b_squared * log(1.0 + b_squared)
                + (4.0 * (2.0 - b_squared) / (3.0 * b_squared * b)) * arctan(b)
                - 2.66666666666 * inverse_b_squared + 0.22222222222
            )
        )
    )

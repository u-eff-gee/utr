import argparse

from numpy import linspace, max, zeros

from schiff import schiff

parser = argparse.ArgumentParser(description='Create an input histogram for the G4GeneralParticleSource which contains the energy spectrum of bremsstrahlung. A model for bremsstrahlung created by an electron beam impinging on a thin target by L.I. Schiff is used.')
parser.add_argument('e0', metavar='E0', type=float, help='Initial energy of the electron beam')
parser.add_argument('z', metavar='Z', type=int, help='Proton number of the target')
parser.add_argument('emax', metavar='EMAX', type=float, help='Maximum energy of the histogram')
parser.add_argument('--nbins', metavar='NBINS', type=int, default=1024, help='Number of bins of the spectrum. Note that a histogram for the G4GeneralParticleSource may have a maximum of 1024 bins.')

args = parser.parse_args()

spectrum = zeros((args.nbins, 2))
spectrum[:,0] = linspace(1e-3*args.e0, args.emax, args.nbins)
spectrum[:,1] = schiff(spectrum[:,0], args.e0, args.z, e2=1.)
# Normalize the intensities
spectrum[:,1] = spectrum[:,1]/max(spectrum[:,1])

with open('brems.dat', 'w') as outputfile:
    for i in range(args.nbins):
        outputfile.write('/gps/hist/point %2.6f %2.6f\n' % (spectrum[i][0], spectrum[i][1]))

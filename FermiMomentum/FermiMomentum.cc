/*
* COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
* Copyright (C) 2025  Savva Savenkov
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "FermiMomentum.hh"

cola::LorentzVector FermiMomentum::randRotate(double p) {
    double cosTheta = theta(engine), phiVal = phi(engine), sinTheta = std::pow(1 - cosTheta*cosTheta, .5);
    return cola::LorentzVector {
        std::pow(gconst::nucleonAverMass * gconst::nucleonAverMass + p*p, .5),
        p * sinTheta * std::cos(phiVal),
        p * sinTheta * std::sin(phiVal),
        p * cosTheta
    };
}

double GoldhaberMomentum::momentumMagn(int A, int NPart) {
    return enParam * std::sqrt(double((A - NPart) * NPart) / static_cast<double>(A - 1)) * normal(engine);
}

double MorriseyMomentum::momentumMagn(int A, int NPart) {
    return enParam * std::sqrt(NPart) * normal(engine);
}

double VanBiberMomentum::momentumMagn(int A, int NPart) {
    int spec = A - NPart;
    double VBVal = enParam1 * enParam1 * spec * (spec - 1) / static_cast<double>(A * (A - 1));
    double MVal = enParam2 * enParam2 * NPart;
    return std::sqrt(VBVal + MVal) * normal(engine);
}
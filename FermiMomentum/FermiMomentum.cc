//
// Created by _amp_ on 10/14/24.
//

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
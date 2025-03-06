/*
* COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
* Copyright (C) 2024-2025 Savva Savenkov
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

#ifndef CGLAUBER_FERMIMOMENTUM_HH
#define CGLAUBER_FERMIMOMENTUM_HH

#include <random>
#include <cmath>

#include "COLA.hh"
#include "GlauberConst.hh"

// base class for distributions
class FermiMomentum {
public:
    FermiMomentum() : engine(std::random_device()()), phi(0, 2*gconst::pi), theta(-1, 1), normal() {}
    virtual ~FermiMomentum() = default;

    cola::LorentzVector getMomentum(int A, int NPart) {
        return randRotate(momentumMagn(A, NPart));
    }

protected:
    std::mt19937_64 engine;
    std::uniform_real_distribution<double> phi;
    std::uniform_real_distribution<double> theta;
    std::normal_distribution<double> normal;

private:
    virtual double momentumMagn(int, int) = 0;
    cola::LorentzVector randRotate(double);

};

class GoldhaberMomentum : public FermiMomentum {
private:
    static constexpr double enParam = 193.;

    double momentumMagn(int, int) final;
};

class MorriseyMomentum : public FermiMomentum {
private:
    static constexpr double enParam = 150.;

    double momentumMagn(int, int) final;
};

class VanBiberMomentum : public FermiMomentum {
private:
    static constexpr double enParam1 = 146.;
    static constexpr double enParam2 = 150.; // same as Morrisey

    double momentumMagn(int, int) final;
};


#endif //CGLAUBER_FERMIMOMENTUM_HH

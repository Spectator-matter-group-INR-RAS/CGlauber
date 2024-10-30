//
// Created by _amp_ on 10/14/24.
//

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

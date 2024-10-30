//
// Created by apbus_amp_k on 26.02.24.
//

#ifndef CGLAUBER_CGLAUBERGENERATOR_H
#define CGLAUBER_CGLAUBERGENERATOR_H

#include <queue>
#include <string>
#include <memory>

#include "COLA.hh"

#include "FermiMomentum.hh"
#include "TGlauberMC.hh"

class CGlauberGenerator final: public cola::VGenerator{
private:
    int pdgCodeA;
    int pdgCodeB;
    double pZA;
    double pZB;
    double energy;
    double sNN;
    double xSectNN;

    std::unique_ptr<TGlauberMC> generator;
    std::unique_ptr<FermiMomentum> fermiMomentum;

public:
    CGlauberGenerator() = delete; // do not use default constructor
    CGlauberGenerator(const std::string&, const std::string&, double, bool, std::unique_ptr<FermiMomentum>&&);
    std::unique_ptr<cola::EventData> operator()() final;
};


#endif //CGLAUBER_CGLAUBERGENERATOR_H

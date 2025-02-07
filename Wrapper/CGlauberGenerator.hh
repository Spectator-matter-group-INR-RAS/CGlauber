/*
* COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
* Copyright (C) 2024 Savva Savenkov
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

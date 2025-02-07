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

#include <map>

#include "GlauberConst.hh"

#include "CGlauberGenerator.hh"

cola::AZ DefinedIons(const std::string& name) {
    if (name == "p")
        return {1, 1};
    else if (name == "pg")
        return {1, 1};
    else if (name == "pdg")
        return {1, 1};	// from arXiv:1101.5953
    else if (name == "dpf")
        return {2, 1};	// deuteron 2pf (tuned to Hulthen)
    else if (name == "dh")
        return {2, 1};	// deuteron Hulthen free
    else if (name == "d")
        return {2, 1};	// deuteron Hulthen constrained
    else if (name == "He3")
        return {3, 1};	// read configurations from file
    else if (name == "H3")
        return {3, 2};	// read configurations from file
    else if (name == "He4")
        return {4, 2};	// read configurations from file
    else if (name == "C")
        return {12, 6};	// read configurations from file
    else if (name == "O")
        return {16, 8};	// read configurations from file
    else if (name == "O2")
        return {16, 8};	// read configurations from file
    else if (name == "Oth")
        return {16, 8};	// read configurations from file
    else if (name == "Opar")
        return {16, 8};	// WS parameterization
    else if (name == "Oho")
        return {16, 8};	// Harmonic oscillator parameterization
    else if (name == "Al")
        return {27, 13};
    else if (name == "Si")
        return {28, 14};
    else if (name == "Si2")
        return {28, 14};
    else if (name == "S")
        return {32, 16};
    else if (name == "Ar")
        return {40, 18};
    else if (name == "Ca")
        return {40, 20};
    else if (name == "Ca2")
        return {40, 20};	// read configuration from file
    else if (name == "Ni")
        return {58, 28};
    else if (name == "Cu")
        return {63, 29};
    else if (name == "Curw ")
        return {63, 29};
    else if (name == "Cu2")
        return {63, 29};
    else if (name == "Cu2rw")
        return {63, 29};
    else if (name == "CuHN")
        return {63, 29};	// from arXiv:0904.4080v1
    else if (name == "Br")
        return {79, 35};	// from the ceiling
    else if (name == "Ag")
        return {109, 47};	// from the ceiling
    else if (name == "Xe")
        return {129, 54};	// adapted from arXiv:1703.04278
    else if (name == "Xes")
        return {129, 54};	// scale from Sb (Antimony, A=122, r=5.32) by 1.019 = (129/122)**0.333
    else if (name == "Xe2")
        return {129, 54};	// adapted from arXiv:1703.04278 and Z. Physik (1974) 270: 113
    else if (name == "Xe2a")
        return {129, 54};	// ALICE parameters (see public note from 2018 at https://cds.cern.ch/collection/ALICE%20Public%20Notes?ln=en)
    else if (name == "Xerw")
        return {129, 54};
    else if (name == "Xesrw")
        return {129, 54};
    else if (name == "Xe2arw")
        return {129, 54};
    else if (name == "W")
        return {186, 74};
    else if (name == "Au")
        return {197, 79};
    else if (name == "Aurw")
        return {197, 79};
    else if (name == "Au2")
        return {197, 79};
    else if (name == "Au2rw")
        return {197, 79};
    else if (name == "AuHN")
        return {197, 79};	// from arXiv:0904.4080v1
    else if (name == "Pb")
        return {208, 82};
    else if (name == "Pbrw")
        return {208, 82};	//only Pb 207 was tested but should be the same for 208
    else if (name == "Pb*")
        return {208, 82};
    else if (name == "PbHN")
        return {208, 82};
    else if (name == "Pbpn")
        return {208, 82};
    else if (name == "Pbpnrw")
        return {208, 82};
        // Uranium description taken from Heinz & Kuhlman, nucl-th/0411054.  In this code, fR is defined as 6.8*0.91, fW=6.8*0.26
    else if (name == "U")
        return {238, 92};
    else if (name == "U2")
        return {238, 92};
    else
        throw std::runtime_error("No such ion in CGlauber");
}

std::unique_ptr<cola::EventData> CGlauberGenerator::operator()() {
    generator->Run(1);
    auto ev = generator->GetEvent();
    auto tNucleons = generator->GetNucleons();
    int nev = 0;
    if (tNucleons != nullptr)
        nev = tNucleons->GetEntries();

    auto cParticles = cola::EventParticles(nev);
    auto fMomentumA = fermiMomentum->getMomentum(generator->GetNucleusA()->GetN(), generator->GetNpartA());
    auto fMomentumB = fermiMomentum->getMomentum(generator->GetNucleusB()->GetN(), generator->GetNpartB());

    double rapidity = std::log(energy + pZA) - std::log(gconst::nucleonAverMass);

    fMomentumA.boostAxisRapidity(rapidity);
    if (pZB != 0)
        fMomentumB.boostAxisRapidity(-rapidity);

    for (int i = 0; i < nev; i++) {
        auto tNucleon = dynamic_cast<TGlauNucleon*>(tNucleons->At(i));
        cParticles[i] = cola::Particle{
                0,
                tNucleon->GetX(),
                tNucleon->GetY(),
                tNucleon->GetZ(),
                tNucleon->IsInNucleusA() ? fMomentumA : fMomentumB,
                tNucleon->GetType() ? 2212 : 2112,
                tNucleon->IsInNucleusA() ?
                (tNucleon->IsSpectator() ? cola::ParticleClass::spectatorA : cola::ParticleClass::nonelasticA) :
                (tNucleon->IsSpectator() ? cola::ParticleClass::spectatorB : cola::ParticleClass::nonelasticB)
        };
    }
    return std::make_unique<cola::EventData>(cola::EventData{
        cola::EventIniState{
                pdgCodeA,
                pdgCodeB,
                pZA,
                pZB,
                energy,
                static_cast<float>(xSectNN),
                static_cast<float>(generator->GetB()),
                generator->GetNcoll(),
                generator->GetNcollpp(),
                generator->GetNcollpn(),
                generator->GetNcollnn(),
                generator->GetNpart(),
                generator->GetNpartA(),
                generator->GetNpartB(),
                ev->PhiA,
                ev->ThetaA,
                ev->PhiB,
                ev->ThetaB,
                cParticles
        },
        cParticles
    });
}

CGlauberGenerator::CGlauberGenerator(const std::string& NA, const std::string& NB, double E, const bool isCollider,
                                     std::unique_ptr<FermiMomentum>&& fermiMomentum) : fermiMomentum(std::move(fermiMomentum)) {
    E *= gconst::GeV;

    cola::AZ AZA = DefinedIons(NA);
    cola::AZ AZB = DefinedIons(NB);
    pdgCodeA = cola::AZToPdg(AZA);
    pdgCodeB = cola::AZToPdg(AZB);
    
    if (isCollider) {
        pZA =    pow(E*E*0.25 - gconst::nucleonAverMass*gconst::nucleonAverMass,0.5);
        pZB = -1*pow(E*E*0.25 - gconst::nucleonAverMass*gconst::nucleonAverMass,0.5);
        energy = (E/2.0 - gconst::nucleonAverMass);
        sNN = E;
    } else {
        pZA = pow(E*(E+2*gconst::nucleonAverMass),0.5);
        pZB = 0;
        energy = E;
        sNN = pow(2*gconst::nucleonAverMass*gconst::nucleonAverMass+2*E*gconst::nucleonAverMass, 0.5);
    }

    double enAtRest = isCollider ? sNN*sNN / 2 / gconst::nucleonAverMass - gconst::nucleonAverMass : E;
    if(enAtRest < 425*gconst::GeV){
        const std::map<double, double> bystricky = {{0.28, 0.027},
                                                    {0.29, 0.051},
                                                    {0.30, 0.082},
                                                    {0.31, 0.120},
                                                    {0.32, 0.171},
                                                    {0.34, 0.321},
                                                    {0.36, 0.563},
                                                    {0.38, 0.921},
                                                    {0.40, 1.41},
                                                    {0.42, 2.05},
                                                    {0.44, 2.83},
                                                    {0.46, 3.74},
                                                    {0.48, 4.76},
                                                    {0.50, 5.87},
                                                    {0.52, 7.04},
                                                    {0.56, 9.48},
                                                    {0.60, 11.89},
                                                    {0.64, 14.14},
                                                    {0.68, 16.15},
                                                    {0.72, 17.89},
                                                    {0.76, 19.34},
                                                    {0.80, 20.53},
                                                    {0.84, 21.51},
                                                    {0.88, 22.33},
                                                    {0.92, 23.03},
                                                    {0.96, 23.81},
                                                    {1.00, 24.39},
                                                    {1.04, 24.89},
                                                    {1.08, 25.30},
                                                    {1.12, 25.66},
                                                    {1.16, 25.95},
                                                    {1.20, 26.21},
                                                    {1.24, 26.42},
                                                    {1.28, 26.59},
                                                    {1.32, 26.74},
                                                    {1.36, 26.86},
                                                    {1.40, 26.96},
                                                    {1.44, 27.04},
                                                    {1.48, 27.11},
                                                    {1.52, 27.17},
                                                    {1.56, 27.21},
                                                    {1.64, 27.28},
                                                    {1.68, 27.30},
                                                    {1.72, 27.32},
                                                    {1.76, 27.33},
                                                    {1.80, 27.34},
                                                    {1.84, 27.35},
                                                    {1.88, 27.36},
                                                    {1.92, 27.36},
                                                    {1.96, 27.36},
                                                    {2.00, 27.36},
                                                    {2.10, 27.37},
                                                    {2.20, 27.37},
                                                    {2.30, 27.38},
                                                    {2.40, 27.38},
                                                    {2.50, 27.40},
                                                    {2.60, 27.41},
                                                    {2.70, 27.43},
                                                    {2.80, 27.46},
                                                    {2.90, 27.49},
                                                    {3.0, 27.52},
                                                    {4.0, 27.97},
                                                    {5.0, 28.48},
                                                    {6.0, 28.92},
                                                    {7.0, 29.28},
                                                    {8.0, 29.55},
                                                    {9.0, 29.75},
                                                    {10.0, 29.89},
                                                    {12.0, 30.04},
                                                    {14.0, 30.10},
                                                    {16.0, 30.09},
                                                    {18.0, 30.05},
                                                    {20.0, 30.00},
                                                    {25.0, 29.88},
                                                    {30.0, 29.80},
                                                    {40.0, 29.78},
                                                    {50.0, 29.91},
                                                    {60.0, 30.13},
                                                    {70.0, 30.38},
                                                    {80.0, 30.66},
                                                    {90.0, 30.93},
                                                    {100.0, 31.19},
                                                    {125.0, 31.78},
                                                    {150.0, 32.23},
                                                    {175.0, 32.56},
                                                    {200.0, 32.79},
                                                    {225.0, 32.92},
                                                    {250.0, 32.97},
                                                    {275.0, 32.96},
                                                    {300.0, 32.89},
                                                    {325.0, 32.78},
                                                    {350.0, 32.63},
                                                    {375.0, 32.44},
                                                    {400.0, 32.24},
                                                    {425.0, 32.01}}; // energy : xsect data
        auto nextVal = bystricky.lower_bound(enAtRest / gconst::GeV);
        auto tabVal = nextVal++;
        double a = (nextVal->second - tabVal->second) / (nextVal->first - tabVal->first);
        double b = nextVal->second - a * nextVal->first;
        xSectNN = a * enAtRest / gconst::GeV + b;
    }
    else {
        double S = sNN * sNN;
        xSectNN = 25.0 + 0.146 * pow(log(S / (gconst::GeV * gconst::GeV)), 2);
    }
    this->generator = std::make_unique<TGlauberMC>(NA.c_str(), NB.c_str(), xSectNN, -1, time(nullptr));
}

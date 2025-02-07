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

#include "CGlauberFactory.hh"

#include <memory>

#include "CGlauberGenerator.hh"

cola::VFilter *CGlauberFactory::create(const std::map<std::string, std::string>& paramMap) {
    auto fMomStr = paramMap.at("fermiMom");
    std::unique_ptr<FermiMomentum> fermiMomentum;

    if (fMomStr == "Goldhaber" or fMomStr == "1")
        fermiMomentum = std::make_unique<GoldhaberMomentum>();
    else if (fMomStr == "Morrisey" or fMomStr == "2")
        fermiMomentum = std::make_unique<MorriseyMomentum>();
    else if (fMomStr == "VanBiber" or fMomStr == "3")
        fermiMomentum = std::make_unique<VanBiberMomentum>();
    else
        throw std::runtime_error("This Fermi momentum option is unrecognized! Available options are 1-3, seek documentation for more information");

    return new CGlauberGenerator(paramMap.at("name_a"), paramMap.at("name_b"), std::stod(paramMap.at("energy")),
                                 std::stoi(paramMap.at("is_collider")), std::move(fermiMomentum));
}
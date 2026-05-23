/*
 * COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
 * Copyright (C) 2024-2026 Savva Savenkov
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

#include "CGlauberGenerator.hh"

#include <memory>

using namespace cola;

std::unique_ptr<cola::VFilter> CGlauberFactory::Create(const std::unordered_map<std::string, std::string>& metaData) {
  std::unique_ptr<FermiMomentum> fermiMomentum;

  if (auto it = metaData.find("fermiMom"); it != metaData.end()) {
    std::string fMomStr = it->second;
    if (fMomStr == "Goldhaber" or fMomStr == "1")
      fermiMomentum = std::make_unique<GoldhaberMomentum>();
    else if (fMomStr == "Morrisey" or fMomStr == "2")
      fermiMomentum = std::make_unique<MorriseyMomentum>();
    else if (fMomStr == "VanBiber" or fMomStr == "3")
      fermiMomentum = std::make_unique<VanBiberMomentum>();
    else {
      std::cout << "This Fermi momentum option is unrecognized! Available options are 1-3, seek documentation for more "
                   "information\nProceeding with default option: Goldhaber\n";
      fermiMomentum = std::make_unique<GoldhaberMomentum>();
    }
  } else {
    std::cout << "No Fermi momentum option specified\nProceeding with default option: Goldhaber\n";
    fermiMomentum = std::make_unique<GoldhaberMomentum>();
  }
  if (auto it = metaData.find("is_collider"); it != metaData.end()) {
    is_collider = std::stoi(it->second);
  } else {
    std::cout << "Geometry is not specified\nProceeding with default option: collider\n";
  }
  if (auto it = metaData.find("energy"); it != metaData.end()) {
    energy = std::stod(it->second);
  } else {
    std::cout << "Energy is not specified\nProceeding with default option: sqrt(s_nn) = 5.02*10^3 GeV\n";
  }
  if (auto it = metaData.find("name_a"); it != metaData.end()) {
    name_a = it->second;
  } else {
    std::cout << "Projectile is not specified\nProceeding with default option: Pb\n";
  }
  if (auto it = metaData.find("name_b"); it != metaData.end()) {
    name_b = it->second;
  } else {
    std::cout << "Target is not specified\nProceeding with default option: Pb\n";
  }

  return std::make_unique<CGlauberGenerator>(name_a.value_or("Pb"), name_b.value_or("Pb"), energy.value_or(5.02e3),
                                             is_collider.value_or(true), std::move(fermiMomentum));
}

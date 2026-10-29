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
#include "FermiMomentum.hh"

#include <COLA.hh>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

using namespace cola;

std::unique_ptr<cola::VFilter> CGlauberFactory::Create(const std::unordered_map<std::string, std::string>& meta_data) {
  std::unique_ptr<FermiMomentum> fermi_momentum;

  if (auto it = meta_data.find("fermiMom"); it != meta_data.end()) {
    std::string f_mom_str = it->second;
    if (f_mom_str == "Goldhaber" or f_mom_str == "1") {
      fermi_momentum = std::make_unique<GoldhaberMomentum>();
    } else if (f_mom_str == "Morrisey" or f_mom_str == "2") {
      fermi_momentum = std::make_unique<MorriseyMomentum>();
    } else if (f_mom_str == "VanBiber" or f_mom_str == "3") {
      fermi_momentum = std::make_unique<VanBiberMomentum>();
    } else {
      std::cout << "This Fermi momentum option is unrecognized! Available options are 1-3, seek documentation for more "
                   "information\nProceeding with default option: Goldhaber\n";
      fermi_momentum = std::make_unique<GoldhaberMomentum>();
    }
  } else {
    std::cout << "No Fermi momentum option specified\nProceeding with default option: Goldhaber\n";
    fermi_momentum = std::make_unique<GoldhaberMomentum>();
  }
  if (auto it = meta_data.find("is_collider"); it != meta_data.end()) {
    is_collider_ = std::stoi(it->second);
  } else {
    std::cout << "Geometry is not specified\nProceeding with default option: collider\n";
  }
  if (auto it = meta_data.find("energy"); it != meta_data.end()) {
    energy_ = std::stod(it->second);
  } else {
    std::cout << "Energy is not specified\nProceeding with default option: sqrt(s_nn) = 5.02*10^3 GeV\n";
  }
  if (auto it = meta_data.find("name_a"); it != meta_data.end()) {
    name_a_ = it->second;
  } else {
    std::cout << "Projectile is not specified\nProceeding with default option: Pb\n";
  }
  if (auto it = meta_data.find("name_b"); it != meta_data.end()) {
    name_b_ = it->second;
  } else {
    std::cout << "Target is not specified\nProceeding with default option: Pb\n";
  }

  return std::make_unique<CGlauberGenerator>(name_a_.value_or("Pb"), name_b_.value_or("Pb"), energy_.value_or(5.02e3),
                                             is_collider_.value_or(true), std::move(fermi_momentum));
}

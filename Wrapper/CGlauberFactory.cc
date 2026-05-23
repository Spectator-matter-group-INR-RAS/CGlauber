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

#include "CGlauberFactory.hh"

#include "CGlauberGenerator.hh"

#include <memory>
#include <stdexcept>

std::unique_ptr<cola::VFilter> CGlauberFactory::Create(const std::unordered_map<std::string, std::string>& meta_data) {
  const auto f_mom_str = meta_data.at("fermiMom");
  std::unique_ptr<FermiMomentum> fermi_momentum;

  if (f_mom_str == "Goldhaber" || f_mom_str == "1") {
    fermi_momentum = std::make_unique<GoldhaberMomentum>();
  } else if (f_mom_str == "Morrisey" || f_mom_str == "2") {
    fermi_momentum = std::make_unique<MorriseyMomentum>();
  } else if (f_mom_str == "VanBiber" || f_mom_str == "3") {
    fermi_momentum = std::make_unique<VanBiberMomentum>();
  } else {
    throw std::runtime_error(
        "This Fermi momentum option is unrecognized! Available options are 1-3, seek documentation for more "
        "information");
  }

  return std::make_unique<CGlauberGenerator>(meta_data.at("name_a"), meta_data.at("name_b"),
                                             std::stod(meta_data.at("energy")),
                                             std::stoi(meta_data.at("is_collider")) != 0, std::move(fermi_momentum));
}

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

#ifndef CGLAUBER_CGLAUBERGENERATOR_H
#define CGLAUBER_CGLAUBERGENERATOR_H

#include "COLA.hh"
#include "FermiMomentum.hh"
#include "TGlauberMC.hh"

#include <memory>
#include <string>

class CGlauberGenerator final : public cola::VGenerator {
 private:
  bool is_collider_;
  int pdg_code_a_;
  int pdg_code_b_;
  double p_za_;
  double p_zb_;
  double energy_;  // kinetic
  double s_nn_;    // in fact sqrt(sNN)
  double x_sect_nn_;

  std::unique_ptr<TGlauberMC> generator_;
  std::unique_ptr<FermiMomentum> fermi_momentum_;

 public:
  CGlauberGenerator() = delete;
  CGlauberGenerator(const std::string& name_a, const std::string& name_b, double energy, bool is_collider,
                    std::unique_ptr<FermiMomentum>&& fermi_momentum);
  std::unique_ptr<cola::EventData> operator()() override;
};

#endif  // CGLAUBER_CGLAUBERGENERATOR_H

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

#include "FermiMomentum.hh"

cola::LorentzVector FermiMomentum::RandRotate(double momentum) {
  const double cos_theta = theta_(engine_);
  const double phi_val = phi_(engine_);
  const double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
  return cola::LorentzVector{
      std::sqrt(gconst::kNucleonAverMass * gconst::kNucleonAverMass + momentum * momentum),
      momentum * sin_theta * std::cos(phi_val),
      momentum * sin_theta * std::sin(phi_val),
      momentum * cos_theta,
  };
}

double GoldhaberMomentum::MomentumMagn(int a, int n_part) {
  return kEnParam * std::sqrt(static_cast<double>((a - n_part) * n_part) / static_cast<double>(a - 1)) *
         normal_(engine_);
}

double MorriseyMomentum::MomentumMagn(int /*a*/, int n_part) { return kEnParam * std::sqrt(n_part) * normal_(engine_); }

double VanBiberMomentum::MomentumMagn(int a, int n_part) {
  const int spectators = a - n_part;
  const double vb_val = kEnParam1 * kEnParam1 * spectators * (spectators - 1) / static_cast<double>(a * (a - 1));
  const double m_val = kEnParam2 * kEnParam2 * n_part;
  return std::sqrt(vb_val + m_val) * normal_(engine_);
}

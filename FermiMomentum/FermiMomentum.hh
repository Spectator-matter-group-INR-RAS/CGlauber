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

#ifndef CGLAUBER_FERMIMOMENTUM_HH
#define CGLAUBER_FERMIMOMENTUM_HH

#include "COLA.hh"
#include "GlauberConst.hh"

#include <cmath>
#include <random>

class FermiMomentum {
 public:
  FermiMomentum() : engine_(std::random_device()()), phi_(0, 2 * gconst::kPi), theta_(-1, 1) {}
  virtual ~FermiMomentum() = default;

  cola::LorentzVector GetMomentum(int a, int n_part) { return RandRotate(MomentumMagn(a, n_part)); }

 protected:
  std::mt19937_64 engine_;
  std::uniform_real_distribution<double> phi_;
  std::uniform_real_distribution<double> theta_;
  std::normal_distribution<double> normal_;

 private:
  virtual double MomentumMagn(int a, int n_part) = 0;
  cola::LorentzVector RandRotate(double momentum);
};

class GoldhaberMomentum final : public FermiMomentum {
 private:
  static constexpr double kEnParam = 193.;

  double MomentumMagn(int a, int n_part) final;
};

class MorriseyMomentum final : public FermiMomentum {
 private:
  static constexpr double kEnParam = 150.;

  double MomentumMagn(int a, int n_part) final;
};

class VanBiberMomentum final : public FermiMomentum {
 private:
  static constexpr double kEnParam1 = 146.;
  static constexpr double kEnParam2 = 150.;  // same as Morrisey

  double MomentumMagn(int a, int n_part) final;
};

#endif  // CGLAUBER_FERMIMOMENTUM_HH

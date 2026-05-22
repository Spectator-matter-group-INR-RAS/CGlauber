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

#ifndef CGLAUBER_CGLAUBERFACTORY_HH
#define CGLAUBER_CGLAUBERFACTORY_HH

#include <COLA.hh>

#include <memory>
#include <string>
#include <unordered_map>

class CGlauberFactory final : public cola::VGeneratorFactory {
 public:
  std::unique_ptr<cola::VFilter> Create(const std::unordered_map<std::string, std::string>& meta_data) final;

  const std::string& GetFilterName() const final {
    static const std::string k_name{"CGlauber"};
    return k_name;
  }
};

#endif  // CGLAUBER_CGLAUBERFACTORY_HH

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

#ifndef CGLAUBER_GLAUBERCONST_H
#define CGLAUBER_GLAUBERCONST_H

namespace gconst {
    constexpr double GeV = 1e3; // Conversion from GeV to MeV for TGlauber
    constexpr double nucleonAverMass = 0.93891875434*GeV;
    constexpr double pi = 3.14159265358979323846;
} // gconst

#endif //CGLAUBER_GLAUBERCONST_H

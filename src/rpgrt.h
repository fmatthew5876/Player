/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EP_RPGRT_H
#define EP_RPGRT_H

namespace rpgrt {

enum class EngineVersion {
	eUnknown = 0,
	e2kMin = 1,
	e2kV1050 = 1,
	e2kV1070 = 2,
	e2kV1090 = 3,
	e2kLegacy = e2kV1090,
	e2kV1510 = 4,
	e2kV1520 = 5,
	e2kUpdated = e2kV1520,
	e2kV1610 = 6,
	e2kEnglish = e2kV1610,

	e2k3Min = 1001,
	e2k3V1030 = 1001,
	e2k3Legacy = e2k3V1030,
	e2k3V1080 = 1002,
	e2k3V1091 = 1003,
	e2k3Updated = e2k3V1091,
	e2k3V1120 = 1004,
	e2k3V1121 = 1005,
	e2k3English = e2kV1610,
};

inline bool is2k(EngineVersion ev) {
	return ev >= EngineVersion::e2kMin && ev < EngineVersion::e2k3Min;
}

inline bool is2k3(EngineVersion ev) {
	return ev >= EngineVersion::e2k3Min;
}

inline bool isLegacy(EngineVersion ev) {
	return (is2k(ev) && ev >= EngineVersion::e2kLegacy) || (is2k3(ev) && ev >= EngineVersion::e2k3Legacy);
}

inline bool isUpdated(EngineVersion ev) {
	return (is2k(ev) && ev >= EngineVersion::e2kUpdated) || (is2k3(ev) && ev >= EngineVersion::e2k3Updated);
}

inline bool isEnglish(EngineVersion ev) {
	return (is2k(ev) && ev >= EngineVersion::e2kEnglish) || (is2k3(ev) && ev >= EngineVersion::e2k3English);
}

} // namespace rpgrt

#endif


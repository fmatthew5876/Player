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

#ifndef EP_SPRITE_BATTLER_H
#define EP_SPRITE_BATTLER_H

// Headers
#include "sprite.h"
#include "async_handler.h"

class BattleAnimation;
class Game_Battler;

/**
 * Sprite_Battler class, used for battle sprites
 */
class Sprite_Battler : public Sprite {
public:
	/**
	 * Constructor.
	 *
	 * @param battler game battler to display
	 * @param battle_index battle index for Z ordering
	 */
	Sprite_Battler(const Game_Battler* battler, int battle_index);

	/** @return the battler */
	const Game_Battler* GetBattler() const;

	/** Recompute the Z value for the sprite from it's Y coordinate. */
	void ResetZ();

protected:
	void Draw(Bitmap& dst);

	const Game_Battler* battler;
	int battle_index = 0;

	FileRequestBinding request_id;
};


const Game_Battler* Sprite_Battler::GetBattler() const {
	return battler;
}

#endif

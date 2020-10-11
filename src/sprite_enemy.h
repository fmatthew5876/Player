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

#ifndef EP_SPRITE_ENEMY_H
#define EP_SPRITE_ENEMY_H

// Headers
#include "sprite_battler.h"
#include "game_battler.h"
#include "async_handler.h"

class BattleAnimation;
class Game_Enemy;

/**
 * Sprite_Enemy class, used for battle sprites
 */
class Sprite_Enemy : public Sprite_Battler {
public:
	/**
	 * Constructor.
	 *
	 * @param battler game battler to display
	 */
	explicit Sprite_Enemy(const Game_Enemy* enemy);

	/** @return enemy sprite */
	const Game_Enemy* GetEnemy() const;

	/**
	 * A hack for 2k battle system. Treat the sprite as not dead
	 * even if the battler is dead. This is needed because battler "dies"
	 * before the 2k battle system animates the death
	 *
	 * @param value whether to force alive or not
	 */
	void SetForcedAlive(bool value);

	/**
	 * Recompute the Z value for the sprite from it's Y coordinate.
	 */
	void ResetZ();

protected:
	void OnMonsterSpriteReady(FileRequestResult* result, int hue);
	void Draw(Bitmap& dst);

	const Game_Enemy* enemy = nullptr;
	bool forced_alive = false;
	FileRequestBinding request_id;
};

inline const Game_Enemy* Sprite_Enemy::GetEnemy() const {
	return enemy;
}

inline void Sprite_Enemy::SetForcedAlive(bool value) {
	forced_alive = value;
}

#endif

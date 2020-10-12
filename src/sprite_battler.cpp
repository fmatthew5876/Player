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

// Headers
#include "game_battler.h"
#include "sprite_battler.h"
#include "bitmap.h"
#include "cache.h"
#include "main_data.h"
#include "game_screen.h"
#include "player.h"
#include <lcf/reader_util.h>
#include "output.h"

Sprite_Battler::Sprite_Battler(const Game_Battler* battler, int index) :
	battler(battler), battle_index(index) {
}

void Sprite_Battler::ResetZ() {
	static_assert(Game_Battler::Type_Ally < Game_Battler::Type_Enemy, "Game_Battler enums re-ordered! Fix Z order logic here!");

	constexpr int id_limit = 128;

	int y = battler->GetBattlePosition().y;
	const auto& graphic = this->GetBitmap();
	if (battler->GetType() == Game_Battler::Type_Enemy && graphic) {
		y += graphic->GetHeight() / 2;
	}

	int z = battler->GetType();
	z *= SCREEN_TARGET_HEIGHT * 2;
	z += y;
	z *= id_limit;
	z += id_limit - battle_index;
	z += Priority_Battler;

	SetZ(z);
}

void Sprite_Battler::Draw(Bitmap& dst) {
	SetTone(Main_Data::game_screen->GetTone());
	SetFlashEffect(battler->GetFlashColor());

#if 0
	if (animation) {
		animation->SetVisible(IsVisible());
	}
#endif

	const bool flip = battler->IsDirectionFlipped();
	SetFlipX(flip);
#if 0
	if (animation) {
		SetFlipX(flip);
	}
#endif

	Sprite::Draw(dst);
}

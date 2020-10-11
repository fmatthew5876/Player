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
#include "battle_animation.h"
#include "game_enemy.h"
#include "sprite_enemy.h"
#include "bitmap.h"
#include "cache.h"
#include "main_data.h"
#include "player.h"
#include <lcf/reader_util.h>
#include "output.h"

Sprite_Enemy::Sprite_Enemy(const Game_Enemy* enemy) :
	Sprite_Battler(enemy, enemy->GetTroopMemberId()),
	enemy(enemy)
{
	auto sprite_name = battler->GetSpriteName();
	auto hue = battler->GetHue();

	SetX(battler->GetDisplayX());
	SetY(battler->GetDisplayY());

	// Not animated -> Monster
	if (sprite_name.empty()) {
		auto graphic = Bitmap::Create(0, 0);
		SetOx(graphic->GetWidth() / 2);
		SetOy(graphic->GetHeight() / 2);
		SetBitmap(graphic);
		ResetZ();
		return;
	}

	FileRequestAsync* request = AsyncHandler::RequestFile("Monster", sprite_name);
	request->SetGraphicFile(true);
	request_id = request->Bind([this, hue](FileRequestResult* result) { OnMonsterSpriteReady(result, hue); });
	request->Start();
}

void Sprite_Enemy::ResetZ() {
	static_assert(Game_Battler::Type_Ally < Game_Battler::Type_Enemy, "Game_Battler enums re-ordered! Fix Z order logic here!");

	constexpr int id_limit = 128;

	int y = battler->GetBattlePosition().y;
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

void Sprite_Battler::CreateSprite() {

}

void Sprite_Enemy::OnMonsterSpriteReady(FileRequestResult* result, int hue) {
	auto graphic = Cache::Monster(result->file);

	SetOx(graphic->GetWidth() / 2);
	SetOy(graphic->GetHeight() / 2);

	ResetZ();

	bool hue_change = (hue != 0);
	if (hue_change) {
		BitmapRef new_graphic = Bitmap::Create(graphic->GetWidth(), graphic->GetHeight());
		new_graphic->HueChangeBlit(0, 0, *graphic, graphic->GetRect(), hue);
		graphic = new_graphic;
	}

	SetBitmap(graphic);
}

void Sprite_Enemy::Draw(Bitmap& dst) {
	auto* enemy = GetEnemy();

	const auto bt = enemy->GetBlinkTimer();
	const auto dt = enemy->GetDeathTimer();
	const auto et = enemy->GetExplodeTimer();

	// FIXME: RPG_RT only checks death
	if (!enemy->Exists() && dt == 0) {
		return;
	}

	auto alpha = 255;
	auto zoom = 1.0;

	if (bt % 10 < 5) {
		if (dt > 0 && !forced_alive) {
			alpha = 7 * dt;
		} else if (et > 0) {
			alpha = 12 * et;
			zoom = static_cast<double>(20 - et) / 20.0 + 1.0;
		}
	}

	if (enemy->IsTransparent()) {
		alpha = 160 * alpha / 255;
	}

	int flying_offset = 0;
	if (Player::IsRPG2k3() && enemy->IsFlying()) {
		flying_offset = enemy->GetBattleFrameCounter() * 4;
	}

	SetOpacity(alpha);
	SetZoomX(zoom);
	SetZoomY(zoom);
	SetX(enemy->GetDisplayX());
	SetY(enemy->GetDisplayY() + flying_offset);

	Sprite_Battler::Draw(dst);
}

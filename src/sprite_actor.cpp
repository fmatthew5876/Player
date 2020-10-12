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
#include "game_actor.h"
#include "sprite_actor.h"
#include "bitmap.h"
#include "cache.h"
#include "main_data.h"
#include "game_screen.h"
#include "player.h"
#include <lcf/reader_util.h>
#include "output.h"

Sprite_Actor::Sprite_Actor(const Game_Actor* actor) :
	// FIXME: FIX Index
	Sprite_Battler(actor, 1)
{
	sprite_name = ToString(battler->GetSpriteName());
	hue = battler->GetHue();

	SetX(battler->GetDisplayX());
	SetY(battler->GetDisplayY());

	SetOx(24);
	SetOy(24);
	ResetZ();
	SetAnimationState(anim_state);
	idling = true;
}

void Sprite_Actor::Update() {
	if (IsVisible() &&
		(sprite_name != battler->GetSpriteName() ||
		hue != battler->GetHue())) {

		CreateSprite();
	}

	if (!battler->IsHidden() && old_hidden != battler->IsHidden()) {
		SetZoomX(1.0);
		SetZoomY(1.0);
		SetOpacity(255);
		SetVisible(true);
		DoIdleAnimation();
	}

	old_hidden = battler->IsHidden();

	const auto cycle = GetActor()->GetBattleFrameCounter();

	if (anim_state > 0) {
		// Animations for allies
		if (Player::IsRPG2k3()) {
			if (animation) {
				// Is a battle animation
				animation->Update();

				if (animation->IsDone()) {
					if (loop_state == LoopState_DefaultAnimationAfterFinish) {
						DoIdleAnimation();
					} else if (loop_state == LoopState_LoopAnimation) {
						animation->SetFrame(0);
					} else if (loop_state == LoopState_WaitAfterFinish) {
						if (animation->GetFrames() > 0) {
							animation->SetFrame(animation->GetFrames() - 1);
						}
						idling = true;
					}
				}

				return;
			}
			// Is a battle charset animation

			static const int frames[] = {0,1,2,1,0};
			int frame = frames[cycle / 10];
			if (frame == sprite_frame)
				return;

			const lcf::rpg::BattlerAnimation* anim = lcf::ReaderUtil::GetElement(lcf::Data::battleranimations, battler->GetBattleAnimationId());
			if (!anim) {
				Output::Warning("Invalid battler animation ID {}", battler->GetBattleAnimationId());
				return;
			}

			const lcf::rpg::BattlerAnimationExtension* ext = lcf::ReaderUtil::GetElement(anim->base_data, anim_state);
			if (!ext) {
				Output::Warning("Animation {}: Invalid battler anim-extension state {}", anim->ID, anim_state);
				return;
			}

			SetSrcRect(Rect(frame * 48, ext->battler_index * 48, 48, 48));

			if (cycle == 40) {
				switch (loop_state) {
					case LoopState_DefaultAnimationAfterFinish:
						cycle = 0;
						idling = true;
						break;
					case LoopState_WaitAfterFinish:
						--cycle; // incremented to last cycle next update
						idling = true;
						break;
					case LoopState_LoopAnimation:
						cycle = 0;
						break;
					default:
						assert(false && "Bad loop state");
				}
			}

			if (idling) {
				DoIdleAnimation();
			}
		}
	}

	// needed for screen shaking
	SetX(battler->GetDisplayX());
	// needed for flying enemies
	SetY(battler->GetDisplayY());

	SetFlashEffect(battler->GetFlashColor());

	if (animation) {
		animation->SetVisible(IsVisible());
	}

	const bool flip = battler->IsDirectionFlipped();
	SetFlipX(flip);
	if (animation) {
		SetFlipX(flip);
	}
}

void Sprite_Actor::SetAnimationState(int state, LoopState loop) {
	// Default value is 100 (function called with val+1)
	// 100 maps all states to "Bad state" (7)
	if (state == 101) {
		state = 7;
	}

	anim_state = state;

	flash_counter = 0;

	loop_state = loop;

	cycle = 0;

	idling = false;

	if (Player::IsRPG2k3()) {
		if (battler->GetBattleAnimationId() > 0) {
			const lcf::rpg::BattlerAnimation* anim = lcf::ReaderUtil::GetElement(lcf::Data::battleranimations, battler->GetBattleAnimationId());
			if (!anim) {
				Output::Warning("Invalid battler animation ID {}", battler->GetBattleAnimationId());
				return;
			}

			const lcf::rpg::BattlerAnimationExtension* ext = lcf::ReaderUtil::GetElement(anim->base_data, anim_state);
			if (!ext) {
				Output::Warning("Animation {}: Invalid battler anim-extension state {}", anim->ID, anim_state);
				return;
			}

			StringView sprite_file = ext->battler_name;

			if (ext->animation_type == lcf::rpg::BattlerAnimationExtension::AnimType_animation) {
				SetBitmap(BitmapRef());
				lcf::rpg::Animation* battle_anim = lcf::ReaderUtil::GetElement(lcf::Data::animations, ext->animation_id);
				if (!battle_anim) {
					Output::Warning("Invalid battle animation ID {}", ext->animation_id);
					animation.reset();
				} else {
					animation.reset(new BattleAnimationBattle(*battle_anim, { battler }));
					animation->SetZ(GetZ());
				}
			}
			else {
				animation.reset();
				if (!sprite_file.empty()) {
					FileRequestAsync* request = AsyncHandler::RequestFile("BattleCharSet", sprite_file);
					request->SetGraphicFile(true);
					request_id = request->Bind(&Sprite_Actor::OnBattlercharsetReady, this, ext->battler_index);
					request->Start();
				}
			}
		}
	}
}

void Sprite_Actor::SetAnimationLoop(LoopState loop) {
	loop_state = loop;
}

void Sprite_Actor::DetectStateChange() {
	if (idling) {
		DoIdleAnimation();
	}
}

bool Sprite_Actor::IsIdling() {
	return idling;
}

int Sprite_Actor::GetWidth() const {
	if (animation) {
		return animation->GetWidth();
	}
	return Sprite::GetWidth();
}

int Sprite_Actor::GetHeight() const {
	if (animation) {
		return animation->GetHeight();
	}
	return Sprite::GetHeight();
}

void Sprite_Actor::ResetZ() {
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

void Sprite_Actor::CreateSprite() {
}

void Sprite_Actor::SetForcedAlive(bool value) {
	forced_alive = value;
}

void Sprite_Actor::DoIdleAnimation() {
	if (battler->IsDefending()) {
		SetAnimationState(AnimationState_Defending);
		idling = true;
		return;
	}

	const lcf::rpg::State* state = battler->GetSignificantState();
	int idling_anim;
	if (battler->GetBattleAnimationId() <= 0) {
		// Monster
		// Only visually different state is Death
		if (state && state->ID == 1 && !forced_alive) {
			idling_anim = AnimationState_Dead;
		} else {
			idling_anim = AnimationState_Idle;
		}
	} else {
		idling_anim = state ? state->battler_animation_id + 1 : AnimationState_Idle;
	}

	if (idling_anim == 101)
		idling_anim = 7;

	if (idling_anim != anim_state)
		SetAnimationState(idling_anim);

	idling = true;
}

void Sprite_Actor::OnMonsterSpriteReady(FileRequestResult* result) {
	graphic = Cache::Monster(result->file);

	SetOx(graphic->GetWidth() / 2);
	SetOy(graphic->GetHeight() / 2);

	ResetZ();

	bool hue_change = hue != 0;
	if (hue_change) {
		BitmapRef new_graphic = Bitmap::Create(graphic->GetWidth(), graphic->GetHeight());
		new_graphic->HueChangeBlit(0, 0, *graphic, graphic->GetRect(), hue);
		graphic = new_graphic;
	}

	SetBitmap(graphic);
}

void Sprite_Actor::OnBattlercharsetReady(FileRequestResult* result, int32_t battler_index) {
	SetBitmap(Cache::Battlecharset(result->file));
	SetSrcRect(Rect(0, battler_index * 48, 48, 48));
}

void Sprite_Actor::Draw(Bitmap& dst) {
	SetTone(Main_Data::game_screen->GetTone());
	SetFlashEffect(battler->GetFlashColor());

	if (animation) {
		animation->SetVisible(IsVisible());
	}

	const bool flip = battler->IsDirectionFlipped();
	SetFlipX(flip);
	if (animation) {
		SetFlipX(flip);
	}

	Sprite::Draw(dst);
}

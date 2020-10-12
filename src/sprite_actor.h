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

#ifndef EP_SPRITE_ACTOR_H
#define EP_SPRITE_ACTOR_H

// Headers
#include "sprite_battler.h"
#include "game_battler.h"
#include "async_handler.h"
#include "game_actor.h"

class BattleAnimation;
class Game_Actor;

/**
 * Sprite_Actor class, used for battle sprites
 */
class Sprite_Actor : public Sprite_Battler {
public:
	enum AnimationState {
		AnimationState_Null,
		AnimationState_Idle,
		AnimationState_RightHand,
		AnimationState_LeftHand,
		AnimationState_SkillUse,
		AnimationState_Dead,
		AnimationState_Damage,
		AnimationState_BadStatus,
		AnimationState_Defending,
		AnimationState_WalkingLeft,
		AnimationState_WalkingRight,
		AnimationState_Victory,
		AnimationState_Item,
		AnimationState_SelfDestruct = 255
	};

	enum LoopState {
		LoopState_DefaultAnimationAfterFinish,
		LoopState_LoopAnimation,
		LoopState_WaitAfterFinish
	};

	/**
	 * Constructor.
	 *
	 * @param battler game battler to display
	 */
	Sprite_Actor(const Game_Actor* actor);

	const Game_Actor* GetActor() const;

	void Update();

	void SetAnimationState(int state, LoopState loop = LoopState_LoopAnimation);
	void SetAnimationLoop(LoopState loop);

	/**
	 * Updates the current animation state of the actor depending on his state.
	 */
	void DetectStateChange();

	/**
	 * Returns true when the actor is in it's default state (Depending on
	 * conditions)
	 *
	 * @return Whether state is default
	 */
	bool IsIdling();

	int GetWidth() const override;
	int GetHeight() const override;

	/**
	 * Recompute the Z value for the sprite from it's Y coordinate.
	 */
	void ResetZ();

protected:
	void Draw(Bitmap& dst) override;
	void CreateSprite();
	void DoIdleAnimation();
	void OnBattlercharsetReady(FileRequestResult* result, int32_t battler_index);
	int GetMaxOpacity() const;

	std::string sprite_name;
	int hue = 0;
	Game_Battler* battler;
	BitmapRef graphic;
	int anim_state = AnimationState_Idle;
	int cycle = 0;
	int sprite_frame = -1;
	int fade_out = 255;
	int fade_out_incr = 15;
	int flash_counter = 0;
	int battle_index = 0;
	LoopState loop_state = LoopState_DefaultAnimationAfterFinish;
	bool old_hidden = false;
	std::unique_ptr<BattleAnimation> animation;
	// false when a newly set animation didn't loop once
	bool idling = true;
	bool forced_alive = false;
	float zoom = 1.0;

	FileRequestBinding request_id;
};

inline const Game_Actor* Sprite_Actor::GetActor() const {
	return static_cast<const Game_Actor*>(GetBattler());
}

#endif

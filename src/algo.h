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

#ifndef EP_ALGO_H
#define EP_ALGO_H

#include <lcf/rpg/fwd.h>
#include <lcf/rpg/system.h>
#include <lcf/rpg/saveactor.h>
#include "game_battler.h"

class Game_Actor;
class Game_Enemy;

namespace Algo {

/**
 * Compute whether a row adjustment should occur.
 * 
 * @param row The row value to check
 * @param cond The current battle condition
 * @param offense Whether to adjust for an offensive action (true) or defensive action (false)
 *
 * @return Whether the row adjustment should apply or not.
 */
bool IsRowAdjusted(lcf::rpg::SaveActor::RowType row, lcf::rpg::System::BattleCondition cond, bool offense);

/**
 * Compute whether a row adjustment should occur.
 * 
 * @param actor The actor whose row to check
 * @param cond The current battle condition
 * @param offense Whether to adjust for an offensive action (true) or defensive action (false)
 * @param allow_enemy Compute row adjustment for enemies also and treat them as always in front row.
 *
 * @return Whether the row adjustment should apply or not.
 */
bool IsRowAdjusted(const Game_Battler& battler,
		lcf::rpg::System::BattleCondition cond,
		bool offense,
		bool allow_enemy);

/**
 * Uses RPG_RT algorithm for performing a variance adjument to damage/healing effects and returns the result.
 *
 * @param base - the base amount of the effect
 * @param var - the variance level from 0 to 10
 *
 * @return the adjusted damage amount
 */
int VarianceAdjustEffect(int base, int var);

/**
 * Compute the hit rate for a physical attack
 *
 * @param source The source of the action
 * @param target The target of the action
 * @param cond The current battle condition
 * @param weapon Which weapon to use or kWeaponAll for combined
 * @param emulate_2k3_enemy_row_bug Whether or not to emulate 2k3 bug where RPG_RT considers defending enemies in the front row
 *
 * @return Success hit rate
 */
int CalcNormalAttackToHit(const Game_Battler& source,
		const Game_Battler& target,
		Game_Battler::Weapon weapon,
		lcf::rpg::System::BattleCondition cond,
		bool emulate_2k3_enemy_row_bug);

/**
 * Compute the hit rate for a skill
 *
 * @param source The source of the action
 * @param target The target of the action
 * @param skill Which skill to calculate hit rate for
 *
 * @return Success hit rate
 */
int CalcSkillToHit(const Game_Battler& source, const Game_Battler& target, const lcf::rpg::Skill& skill);

/**
 * Compute the critical hit rate if source attacks target.
 *
 * @param source The attacker
 * @param target The defender
 * @param weapon Which weapon to use or kWeaponAll for combined
 *
 * @return Critical hit rate.
 */
int CalcCriticalHitChance(const Game_Battler& source, const Game_Battler& target, Game_Battler::Weapon weapon);

/**
 * Check if target is defending and perform damage adjustment if so.
 *
 * @param dmg The base amount of damage.
 * @param target who is to receive damage
 *
 * @return The adjusted damage.
 */
int AdjustDamageForDefend(int dmg, const Game_Battler& target);

/**
 * Compute the base damage for a normal attack
 * This includes: source atk, target def, source row, attributes, target row, critical, source charged, and variance
 * It does not include: hit rate, critical hit rate, target defend adjustment, value clamping
 *
 * @param source The source of the action
 * @param target The target of the action
 * @param weapon Which weapon to use or kWeaponAll for combined
 * @param is_critical_hit If true, apply critical hit bonus
 * @param apply_variance If true, apply variance to the damage
 * @param cond The current battle condition
 * @param emulate_2k3_enemy_row_bug Whether or not to emulate 2k3 bug where RPG_RT considers defending enemies in the front row
 *
 * @return effect amount
 */
int CalcNormalAttackEffect(const Game_Battler& source,
		const Game_Battler& target,
		Game_Battler::Weapon weapon,
		bool is_critical_hit,
		bool apply_variance,
		lcf::rpg::System::BattleCondition cond,
		bool emulate_2k3_enemy_row_bug);

/**
 * Compute the base damage for a skill
 * This includes: power, source atk/mag, target def/mag, attributes, variance
 * It does not include: hit rate, target defend adjustment, value clamping
 *
 * @param source The source of the action
 * @param target The target of the action
 * @param skill The skill to use
 * @param apply_variance If true, apply variance to the damage
 *
 * @return effect amount
 */
int CalcSkillEffect(const Game_Battler& source,
		const Game_Battler& target,
		const lcf::rpg::Skill& skill,
		bool apply_variance);

/**
 * Compute the base damage for self-destruct
 * This includes: power, source atk, target def, variance
 * It does not include: hit rate, target defend adjustment, value clamping
 *
 * @param source The source of the action
 * @param target The target of the action
 * @param apply_variance If true, apply variance to the damage
 *
 * @return effect amount
 */
int CalcSelfDestructEffect(const Game_Battler& source,
		const Game_Battler& target,
		bool apply_variance);

/**
 * Determine whether a skill is usable.
 *
 * @param skill the skill to check
 * @param require_states_persist If we should require persistent states for non-battle.
 * @return Whether the skill can be used.
 */
bool IsSkillUsable(const lcf::rpg::Skill& skill,
		bool require_states_persist);

} // namespace Algo


#endif

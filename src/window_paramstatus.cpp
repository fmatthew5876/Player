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
#include <iomanip>
#include <sstream>
#include "game_actors.h"
#include "window_paramstatus.h"
#include "bitmap.h"
#include "font.h"
#include "cache.h"
#include "text.h"

Window_ParamStatus::Window_ParamStatus(int ix, int iy, int iwidth, int iheight, int actor_id) :
	Window_Base(ix, iy, iwidth, iheight),
	actor_id(actor_id)
{

	SetContents(Bitmap::Create(width - 16, height - 16));

	Refresh();
}

void Window_ParamStatus::Refresh() {
	contents->Clear();

	auto font = Font::Default();
	auto system = Cache::SystemOrBlack();

	auto* actor = Game_Actors::GetActor(actor_id);

	auto draw = [&](int y, const std::string& name, int value) {
		// Draw Term
		Text::Draw(*contents, 0, y, *font, *system, 1, name);

		// Draw Value
		Text::Draw(*contents, 90, y, *font, *system, Font::ColorDefault, std::to_string(value), Text::AlignRight);
		return y + 16;
	};

	int y = 2;
	y = draw(y, Data::terms.attack, actor->GetAtk());
	y = draw(y, Data::terms.defense, actor->GetDef());
	y = draw(y, Data::terms.spirit, actor->GetSpi());
	y = draw(y, Data::terms.agility, actor->GetAgi());
}


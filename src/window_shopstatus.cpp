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
#include <string>
#include <sstream>
#include "game_party.h"
#include "window_shopstatus.h"
#include "bitmap.h"
#include "font.h"
#include "text.h"
#include "cache.h"

Window_ShopStatus::Window_ShopStatus(int ix, int iy, int iwidth, int iheight) :
	Window_Base(ix, iy, iwidth, iheight), item_id(0) {

	SetContents(Bitmap::Create(width - 16, height - 16));

	Refresh();
}

void Window_ShopStatus::Refresh() {
	contents->Clear();

	int number = 0;
	int equipped = 0;

	if (item_id != 0) {
		number = Main_Data::game_party->GetItemCount(item_id);
		equipped = Main_Data::game_party->GetEquippedItemCount(item_id);
	}

	auto system = Cache::SystemOrBlack();
	auto font = Font::Default();

	Text::Draw(*contents, 0, 2, *font, *system, 1, Data::terms.possessed_items);
	Text::Draw(*contents, 0, 18, *font, *system, 1, Data::terms.equipped_items);

	Text::Draw(*contents, 120, 2, *font, *system, Font::ColorDefault, std::to_string(number), Text::AlignRight);

	Text::Draw(*contents, 120, 18, *font, *system, Font::ColorDefault, std::to_string(equipped), Text::AlignRight);
}

void Window_ShopStatus::SetItemId(int new_item_id) {
	if (new_item_id != item_id) {
		item_id = new_item_id;
		Refresh();
	}
}

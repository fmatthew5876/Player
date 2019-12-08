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

#include <sstream>

#include "fps_overlay.h"
#include "player.h"
#include "graphics.h"
#include "bitmap.h"
#include "input.h"
#include "font.h"

FpsOverlay::FpsOverlay() :
	Drawable(TypeOverlay, Priority_Overlay + 100, true)
{
	Graphics::RegisterDrawable(this);
}

void FpsOverlay::Update() {
	int mod = Player::GetSpeedModifier();

	if (mod != last_speed_mod) {
		speedup_dirty = true;
		last_speed_mod = mod;
	}
}

void FpsOverlay::Draw(Bitmap& dst) {
	// FIXME: Break this dependency on DisplayUi
	bool fps_draw = (
#ifndef EMSCRIPTEN
		DisplayUi->IsFullscreen() &&
#endif
		Player::fps_flag);

	if (fps_draw) {
		if (fps_dirty) {
			std::string text = GetFpsString();
			Rect rect = Font::Default()->GetSize(text);

			if (!fps_bitmap || fps_bitmap->GetWidth() < rect.width + 1) {
				// Height never changes
				fps_bitmap = Bitmap::Create(rect.width + 1, rect.height - 1, true);
			}
			fps_bitmap->Clear();
			fps_bitmap->Fill(Color(0, 0, 0, 128));
			fps_bitmap->TextDraw(1, 0, Color(255, 255, 255, 255), text);

			fps_rect = Rect(0, 0, rect.width + 1, rect.height - 1);

			fps_dirty = false;
		}

		dst.Blit(1, 2, *fps_bitmap, fps_rect, 255);
	}

	// Always drawn when speedup is on independent of FPS
	if (last_speed_mod > 1) {
		if (speedup_dirty) {
			std::string text = "> x" + std::to_string(last_speed_mod);

			Rect rect = Font::Default()->GetSize(text);

			if (!speedup_bitmap || speedup_bitmap->GetWidth() < rect.width + 1) {
				// Height never changes
				speedup_bitmap = Bitmap::Create(rect.width + 1, rect.height - 1, true);
			}
			speedup_bitmap->Clear();
			speedup_bitmap->Fill(Color(0, 0, 0, 128));
			speedup_bitmap->TextDraw(1, 0, Color(255, 255, 255, 255), text);

			speedup_rect = Rect(0, 0, rect.width + 1, rect.height - 1);

			speedup_dirty = false;
		}

		int dwidth = DisplayUi->GetDisplaySurface()->GetWidth();
		dst.Blit(dwidth - speedup_rect.width - 1, 2, *speedup_bitmap, speedup_rect, 255);
	}
}

int FpsOverlay::GetFps() const {
	return last_fps;
}

int FpsOverlay::GetUps() const {
	return last_ups;
}

void FpsOverlay::AddFrame() {
	++fps;
}

void FpsOverlay::AddUpdate() {
	++ups;
}

void FpsOverlay::ResetCounter() {
	last_fps = fps;
	last_ups = ups;

	fps = 0;
	ups = 0;

	fps_dirty = true;
}

std::string FpsOverlay::GetFpsString() const {
	std::stringstream text;
	text << "FPS: " << GetFps();
	return text.str();
}

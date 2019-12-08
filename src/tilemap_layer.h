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

#ifndef EP_TILEMAP_LAYER_H
#define EP_TILEMAP_LAYER_H

// Headers
#include <vector>
#include <map>
#include <set>
#include "system.h"
#include "drawable.h"
#include "tone.h"

class TilemapLayer;

/**
 * TilemapSubLayer class.
 */
class TilemapSubLayer : public Drawable {
public:
	TilemapSubLayer(TilemapLayer* tilemap, int z);

	void Draw(Bitmap& dst) override;

private:
	TilemapLayer* tilemap = nullptr;
};

/**
 * TilemapLayer class.
 */
class TilemapLayer {
public:
	TilemapLayer(int ilayer);

	void DrawTile(Bitmap& dst, Bitmap& screen, int x, int y, int row, int col, bool allow_fast_blit = true);
	void Draw(Bitmap& dst, int z_order);

	void Update();

	BitmapRef const& GetChipset() const;
	void SetChipset(BitmapRef const& nchipset);
	const std::vector<short>& GetMapData() const;
	void SetMapData(std::vector<short> nmap_data);
	const std::vector<unsigned char>& GetPassable() const;
	void SetPassable(std::vector<unsigned char> npassable);
	bool GetVisible() const;
	void SetVisible(bool nvisible);
	int GetOx() const;
	void SetOx(int nox);
	int GetOy() const;
	void SetOy(int noy);
	int GetWidth() const;
	void SetWidth(int nwidth);
	int GetHeight() const;
	void SetHeight(int nheight);
	int GetAnimationSpeed() const;
	void SetAnimationSpeed(int speed);
	int GetAnimationType() const;
	void SetAnimationType(int type);
	void OnSubstitute();
	/**
	 * Influences how tiles of the tilemap are blitted.
	 * When enabled the opacity information of the tile is ignored and a opaque
	 * tile is assumed (Faster).
	 *
	 * @param fast true: enable fast blit (ignores alpha)
	 */
	void SetFastBlit(bool fast);

	void SetTone(Tone tone);

private:
	BitmapRef chipset;
	BitmapRef chipset_effect;
	std::set<short> chipset_tone_tiles;

	std::vector<short> map_data;
	std::vector<uint8_t> passable;
	// FIXME Should be span<uint8_t>
	const std::vector<uint8_t>& substitutions;
	bool visible = true;
	int ox = 0;
	int oy = 0;
	int width = 0;
	int height = 0;
	char animation_frame = 0;
	char animation_step_ab = 0;
	char animation_step_c = 0;
	int animation_speed = 0;
	int animation_type = 0;
	int layer = 0;
	bool fast_blit = false;

	void CreateTileCache(const std::vector<short>& nmap_data);
	void GenerateAutotileAB(short ID, short animID);
	void GenerateAutotileD(short ID);

	static const int TILES_PER_ROW = 64;

	struct TileXY {
		uint8_t x;
		uint8_t y;
		bool valid;
		TileXY() : valid(false) {}
		TileXY(uint8_t x, uint8_t y) : x(x), y(y), valid(true) {}
	};

	BitmapRef GenerateAutotiles(int count, const std::map<uint32_t, TileXY>& map);

	TileXY GetCachedAutotileAB(short ID, short animID);
	TileXY GetCachedAutotileD(short ID);
	BitmapRef autotiles_ab_screen;
	BitmapRef autotiles_ab_screen_effect;
	std::set<short> autotiles_ab_screen_tone_tiles;
	BitmapRef autotiles_d_screen;
	BitmapRef autotiles_d_screen_effect;
	std::set<short> autotiles_d_screen_tone_tiles;

	int autotiles_ab_next = -1;
	int autotiles_d_next = -1;

	TileXY autotiles_ab[3][3][16][47];
	TileXY autotiles_d[12][50];

	std::map<uint32_t, TileXY> autotiles_ab_map;
	std::map<uint32_t, TileXY> autotiles_d_map;

	struct TileData {
		short ID;
		int z;
	};
	std::vector<std::vector<TileData> > data_cache;
	std::vector<std::shared_ptr<TilemapSubLayer> > sublayers;

	Tone tone;
};

inline BitmapRef const& TilemapLayer::GetChipset() const {
	return chipset;
}

inline const std::vector<short>& TilemapLayer::GetMapData() const {
	return map_data;
}

inline const std::vector<unsigned char>& TilemapLayer::GetPassable() const {
	return passable;
}

inline bool TilemapLayer::GetVisible() const {
	return visible;
}

inline void TilemapLayer::SetVisible(bool nvisible) {
	visible = nvisible;
}

inline int TilemapLayer::GetOx() const {
	return ox;
}

inline void TilemapLayer::SetOx(int nox) {
	ox = nox;
}

inline int TilemapLayer::GetOy() const {
	return oy;
}

inline void TilemapLayer::SetOy(int noy) {
	oy = noy;
}

inline int TilemapLayer::GetWidth() const {
	return width;
}

inline void TilemapLayer::SetWidth(int nwidth) {
	width = nwidth;
}

inline int TilemapLayer::GetHeight() const {
	return height;
}

inline void TilemapLayer::SetHeight(int nheight) {
	height = nheight;
}

inline int TilemapLayer::GetAnimationSpeed() const {
	return animation_speed;
}

inline void TilemapLayer::SetAnimationSpeed(int speed) {
	animation_speed = speed;
}

inline int TilemapLayer::GetAnimationType() const {
	return animation_type;
}

inline void TilemapLayer::SetAnimationType(int type) {
	animation_type = type;
}

inline void TilemapLayer::SetFastBlit(bool fast) {
	fast_blit = fast;
}


#endif

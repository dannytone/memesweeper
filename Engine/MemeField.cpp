#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include "Beveller.h"
#include <algorithm>

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2& screenPos, bool fucked, Graphics& gfx) const
{
	if (!fucked)
	{
		switch (state)
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighbourMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			break;
		}
	}
	else // we are fucked
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else 
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighbourMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		}

	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden || state == State::Flagged);
	state = State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

int MemeField::Tile::GetNeighbourMemeCount() const
{
	return nNeighbourMemes;
}

void MemeField::Tile::SetNeighbourMemeCount(int memeCount)
{
	assert(nNeighbourMemes == -1);
	nNeighbourMemes = memeCount;
}


bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return field[gridPos.y * width + gridPos.x];
}


const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos)
{
	Vei2 offset = { x_offset,y_offset };
	return (screenPos - offset) / SpriteCodex::tileSize;
}

int MemeField::CountNeighbourMemes(const Vei2& gridPos)
{
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(height - 1, gridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}
	return count;
}

void MemeField::SetFlagCount(const int value)
{
	nFlags = nFlags + value;
}

MemeField::MemeField(int nMemes_in, Graphics& gfx)
{
	x_offset = (gfx.ScreenWidth / 2) - ((width * SpriteCodex::tileSize) / 2);
	y_offset = (gfx.ScreenHeight / 2) - ((height * SpriteCodex::tileSize) / 2);
	assert(nMemes_in > 0 && nMemes_in < width* height);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	nMemes = nMemes_in;

	for (int nSpawned = 0; nSpawned < nMemes_in; nSpawned++)
	{
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist(rng), yDist(rng) };
		} 		
		while (TileAt(spawnPos).HasMeme());
		TileAt(spawnPos).SpawnMeme();
	}

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).SetNeighbourMemeCount(CountNeighbourMemes(gridPos));
		}
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	Vei2 offset = { x_offset,y_offset };
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw((gridPos * SpriteCodex::tileSize) + offset, isFucked, gfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(x_offset, (SpriteCodex::tileSize * width) + x_offset, y_offset, (SpriteCodex::tileSize * height) + y_offset);
}

void MemeField::OnRevealClick(const Vei2& screenPos)
{
	if (!isFucked)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed() && !tile.IsFlagged())
		{
			if (tile.GetNeighbourMemeCount() == 0)
			{
				tile.Reveal();
				RevealSurrounding(gridPos);
			}
			else
			{
				tile.Reveal();
			}
			
			if (tile.HasMeme())
			{
				isFucked = true;
			}
			
		}
	}
}

void MemeField::OnFlagClick(const Vei2& screenPos)
{
	if (!isFucked)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width&& gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed())
		{
			tile.ToggleFlag();
		}
		nFlags = 0;
		nCorrectFlags = 0;
		for (Vei2 gridPos2 = { 0,0 }; gridPos2.y < height; gridPos2.y++)
		{
			for (gridPos2.x = 0; gridPos2.x < width; gridPos2.x++)
			{
				if (TileAt(gridPos2).IsFlagged())
				{
					nFlags++;
				}
				if (TileAt(gridPos2).IsFlagged() && TileAt(gridPos2).HasMeme())
				{
					nCorrectFlags++;
				}
			}
		}
		if (nCorrectFlags == nMemes && nCorrectFlags == nFlags)
		{
			win = true;
		}
	}
}

void MemeField::RevealSurrounding(const Vei2& gridPos_in)
{
	const int looper = std::max(width, height);
	for (int i = 0; i <= looper; i++)
	{
		for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
		{
			for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
			{
				if (TileAt(gridPos).IsRevealed() && TileAt(gridPos).GetNeighbourMemeCount() == 0)
				{
					const int xStart = std::max(0, gridPos.x - 1);
					const int yStart = std::max(0, gridPos.y - 1);
					const int xEnd = std::min(width - 1, gridPos.x + 1);
					const int yEnd = std::min(height - 1, gridPos.y + 1);

					for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
					{
						for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
						{
							if (!TileAt(gridPos).IsFlagged() && !TileAt(gridPos).IsRevealed())
							{
								TileAt(gridPos).Reveal();
							}
						}
					}
				}
			}
		}
	}
}
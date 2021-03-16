#pragma once
#include "Graphics.h"

class MemeField
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, bool fucked, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		int GetNeighbourMemeCount() const;
		void SetNeighbourMemeCount(int memeCount);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighbourMemes = -1;
	};

public:
	MemeField(int nMemes_in, Graphics& gfx);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2& screenPos);
	void OnFlagClick(const Vei2& screenPos);
	bool GameIsWon() const;
	bool GameIsLost() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	void RevealSurrounding();
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighbourMemes(const Vei2& gridPos);
private:
	static constexpr int width = 9;
	static constexpr int height = 9;
	int x_offset;
	int y_offset;
	bool isFucked = false;
	int nMemes;
	Tile field[width * height];
};
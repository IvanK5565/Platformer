#pragma once
namespace myGame {
	enum class StateResult {
		ENDED,
		CONTINUES,
		FAILED
	};
	enum class States {
		Idle,
		Run,
		Jump,
		Attack,
		Hurt,
		Death,
		Count
	};
	enum class Direction {
		None,
		Up,
		Down,
		Right,
		Left,
		Count
	};
	enum class CollideType {
		RightTriangle,
		LeftTriangle,
		Rectangle
	};
	enum class TexturesID {
		Player,
		Enemy,
		Tile,
	};

	enum class StatusCode {
		CORRECT,
		UNCORRECT,
	};

	enum class MapStatus {
		Act,
		Win,
		Lose,
	};
}
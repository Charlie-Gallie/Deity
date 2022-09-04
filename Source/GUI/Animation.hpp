#pragma once

#include <initializer_list>
#include <vector>

#include "GUI/SpriteLoader.hpp"
#include "GUI/Sprite.hpp"

namespace dty {
	using SpriteArray = std::vector<Ref<Sprite>>;

	class IsometricPlane;
	class Animation {
	public:
		Animation(Ref<IsometricPlane> _isometricPlane, SpriteArray _sprites = {}) {
			isometricPlane = _isometricPlane;
			sprites.assign(_sprites.begin(), _sprites.end());
		}

		void Update(float deltaTime) {
			currentFrameTime += deltaTime;

			if (lifetimeLimit > 0) {
				currentLifetime += deltaTime;
				if (currentLifetime >= lifetimeLimit) {
					Clear();
					lifetimeLimit = 0;
				}
			}

			if (currentFrameTime >= frameDelay) {
				++currentSpriteIndex;
				if (currentSpriteIndex >= sprites.size())
					currentSpriteIndex = 0;

				currentFrameTime = 0;
			}
		}

		void SetFrameDelay(float _frameDelay) { frameDelay = _frameDelay; }
		void BeginLifetime() { lifetimeLimit = lifetimeLimit; }

		Ref<Sprite> GetCurrentSprite() { 
			if (!sprites.size())
				return nullptr;
			return sprites[currentSpriteIndex];
		}

		void SetLifetimeLimit(float _lifetimeLimit) {
			lifetimeLimit = _lifetimeLimit;
			currentLifetime = 0.0f;
		}

		void Clear() { 
			currentSpriteIndex = 0;
			sprites.clear();
			sprites.shrink_to_fit();
		}

		void SetGridPositions(Vector2u _position) {
			for (Ref<Sprite> sprite : sprites)
				sprite->SetGridPosition(_position);
		}

		void SetSizes(Vector2u _size) {
			for (Ref<Sprite> sprite : sprites)
				sprite->size = _size;
		}

		SpriteArray GetSprites() { return sprites; }
		void AppendSprite(Ref<Sprite> _sprite) { sprites.push_back(_sprite); }
		void AppendSprite(SpriteName _spriteName, Ref<SpriteLoader> _spriteLoader) { 
			Ref<Sprite> sprite = CreateRef<Sprite>();
			sprite->SetCurrentSprite(_spriteName, _spriteLoader);
			sprites.push_back(sprite);
		}
	private:
		SpriteArray sprites;
		Ref<IsometricPlane> isometricPlane;

		size_t currentSpriteIndex = 0;
		float
			frameDelay = 0.100f,		// Time between frames in seconds
			currentFrameTime = 0.0f,	// How long the frame has been playing for
			lifetimeLimit = 0.0f,		// How long the animation should play for
			currentLifetime = 0.0f;		// How long the animation has been playing for
	};
}
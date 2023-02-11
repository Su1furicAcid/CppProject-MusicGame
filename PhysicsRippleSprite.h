#pragma once
// PhysicsRippleSprite.h
#include <map>
using std::map;

#include "cocos2d.h"
USING_NS_CC;

struct PhysicsRippleSpriteConfig {
	int quadCountX{ 240 };
	int quadCountY{ 240 };
	int touchRadius{ 4 };
	float updateInterval{ 1 / 90.0f };

	PhysicsRippleSpriteConfig() {
	}

	PhysicsRippleSpriteConfig(int countX, int countY, int radius, float interval) :
		quadCountX(countX),
		quadCountY(countY),
		touchRadius(radius),
		updateInterval(interval) {
	}
};

class PhysicsRippleSprite : public CCNode {
public:
	// TODO: improve
	static PhysicsRippleSprite* create(const char* filename, const PhysicsRippleSpriteConfig& config = PhysicsRippleSpriteConfig());
	static PhysicsRippleSprite* create(CCTexture2D* texture, const PhysicsRippleSpriteConfig& config = PhysicsRippleSpriteConfig());

public:
	virtual ~PhysicsRippleSprite();
	bool init(const char* filename, const PhysicsRippleSpriteConfig& config);
	bool init(CCTexture2D* texture, const PhysicsRippleSpriteConfig& config);
	void reset();
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	void onDraw(const Mat4& transform);
	virtual void update(float deltaTime) override;
	void addRipple(const CCPoint& pos, float strength);

private:
	void initRippleBuffer();
	void initRippleCoeff();
	void initRippleMesh();
	void generateRippleCoeff(int touchRadius);

private:
	PhysicsRippleSpriteConfig m_config;

private:
	CCTexture2D* m_texture{ nullptr };
	int m_bufferSize{ 0 };
	CCPoint* m_vertices{ nullptr };
	CCPoint* m_texCoords{ nullptr };

private:
	//float* m_rippleCoeff{ nullptr };
	map<int, float*> m_rippleCoeffs;
	float* m_rippleSource{ nullptr };
	float* m_rippleDest{ nullptr };

private:
	float m_elapseTime{ 0 };

private:
	CustomCommand m_customCommand;
};

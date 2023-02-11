#include "PhysicsRippleSprite.h"
#include <algorithm>
PhysicsRippleSprite*
PhysicsRippleSprite::create(const char* filename, const PhysicsRippleSpriteConfig& config) {
	auto rippleSprite = new PhysicsRippleSprite();
	if (rippleSprite && rippleSprite->init(filename, config)) {
		//自动释放节省内存
		rippleSprite->autorelease();
		return rippleSprite;
	}
	else {
		CC_SAFE_DELETE(rippleSprite);
		return nullptr;
	}
}

//重载函数
PhysicsRippleSprite*
PhysicsRippleSprite::create(CCTexture2D* texture, const PhysicsRippleSpriteConfig& config) {
	auto rippleSprite = new PhysicsRippleSprite();
	if (rippleSprite && rippleSprite->init(texture, config)) {
		rippleSprite->autorelease();
		return rippleSprite;
	}
	else {
		CC_SAFE_DELETE(rippleSprite);
		return nullptr;
	}
}

//手动内存释放
PhysicsRippleSprite::~PhysicsRippleSprite() {
	CC_SAFE_RELEASE(m_texture);
	CC_SAFE_DELETE_ARRAY(m_vertices);
	CC_SAFE_DELETE_ARRAY(m_texCoords);

	for (auto kv : m_rippleCoeffs) {
		CC_SAFE_DELETE_ARRAY(kv.second);
	}
	CC_SAFE_DELETE_ARRAY(m_rippleSource);
	CC_SAFE_DELETE_ARRAY(m_rippleDest);
}

bool PhysicsRippleSprite::init(const char* filename, const PhysicsRippleSpriteConfig& config) {
	auto texture = CCTextureCache::sharedTextureCache()->addImage(filename);
	return init(texture, config);
}

bool PhysicsRippleSprite::init(CCTexture2D* texture, const PhysicsRippleSpriteConfig& config) {
	if (!texture) {
		return false;
	}
	m_texture = texture;
	//保持引用避免被释放掉，避免nullptr类型错误
	m_texture->retain();
	m_config = config;
	initRippleBuffer();
	initRippleCoeff();
	initRippleMesh();
	setContentSize(m_texture->getContentSize());
	setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
	scheduleUpdate();
	return true;
}

void PhysicsRippleSprite::reset() {
	// 初始化
	memset(m_rippleSource, 0, (m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float));
	memset(m_rippleDest, 0, (m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float));
	m_elapseTime = 0;
}

void PhysicsRippleSprite::initRippleBuffer() {
	//初始化两个高度网格数组
	m_rippleSource = new float[(m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float)];
	m_rippleDest = new float[(m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float)];
	//+2是为了覆盖边界
	memset(m_rippleSource, 0, (m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float));
	memset(m_rippleDest, 0, (m_config.quadCountX + 2) * (m_config.quadCountY + 2) * sizeof(float));
}

void PhysicsRippleSprite::initRippleCoeff() {
	generateRippleCoeff(m_config.touchRadius);
}

void PhysicsRippleSprite::generateRippleCoeff(int touchRadius) {
	//map中没有要查找的数据
	if (m_rippleCoeffs.find(touchRadius) == m_rippleCoeffs.end()) {
		auto rippleCoeff = new float[(touchRadius * 2 + 1) * (touchRadius * 2 + 1) * sizeof(float)];
		for (int y = 0; y <= 2 * touchRadius; ++y) {
			for (int x = 0; x <= 2 * touchRadius; ++x) {
				float distance = sqrt((x - touchRadius) * (x - touchRadius) + (y - touchRadius) * (y - touchRadius));
				if (distance <= touchRadius) {
					float factor = distance / touchRadius;
					// 能量变化 -512 -> 0
					rippleCoeff[y * (touchRadius * 2 + 1) + x] = -(cos(factor * M_PI) + 1.0f) * 256.0f;
				}
				else {
					rippleCoeff[y * (touchRadius * 2 + 1) + x] = 0.0f;
				}
			}
		}
		m_rippleCoeffs[touchRadius] = rippleCoeff;
	}
}

void PhysicsRippleSprite::initRippleMesh() {
	//将网格与纹理对应
	int verticesPerStrip = 2 * (m_config.quadCountX + 1);
	m_bufferSize = verticesPerStrip * m_config.quadCountY;
	m_vertices = new CCPoint[m_bufferSize];
	m_texCoords = new CCPoint[m_bufferSize];
	CCSize textureSize = m_texture->getContentSize();
	CCPoint normalized;
	int index = 0;
	for (int y = 0; y < m_config.quadCountY; ++y) {
		for (int x = 0; x < (m_config.quadCountX + 1); ++x) {
			for (int z = 0; z < 2; ++z) {
				normalized.x = (float)x / (float)m_config.quadCountX;
				normalized.y = (float)(y + z) / (float)m_config.quadCountY;
				m_vertices[index] = ccp(normalized.x * textureSize.width, normalized.y * textureSize.height);
				m_texCoords[index] = ccp(normalized.x * m_texture->getMaxS(), m_texture->getMaxT() - (normalized.y * m_texture->getMaxT()));
				++index;
			}
		}
	}
}

void PhysicsRippleSprite::onDraw(const Mat4& transform) {
	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins(transform);
	GL::bindTexture2D(m_texture->getName());
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CCAssert(sizeof(CCPoint) == sizeof(ccVertex2F), "Incorrect ripple sprite buffer format");
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords);

	int verticesPerStrip = m_bufferSize / m_config.quadCountY;
	for (int i = 0; i < m_config.quadCountY; ++i) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * verticesPerStrip, verticesPerStrip);
	}
}

//渲染部分
void PhysicsRippleSprite::update(float deltaTime) {
	m_elapseTime += deltaTime;
	if (m_elapseTime < m_config.updateInterval) {
		return;
	}
	else {
		m_elapseTime -= int(m_elapseTime / m_config.updateInterval) * m_config.updateInterval;
	}
	//首先计算波幅
	for (int y = 0; y < m_config.quadCountY; ++y) {
		for (int x = 0; x < m_config.quadCountX; ++x) {
			//       a 
			//     c * d
			//       b 
			float a = m_rippleSource[(y) * (m_config.quadCountX + 2) + x + 1];
			float b = m_rippleSource[(y + 2) * (m_config.quadCountX + 2) + x + 1];
			float c = m_rippleSource[(y + 1) * (m_config.quadCountX + 2) + x];
			float d = m_rippleSource[(y + 1) * (m_config.quadCountX + 2) + x + 2];
			float result = (a + b + c + d) / 2.f - m_rippleDest[(y + 1) * (m_config.quadCountX + 2) + x + 1];
			result -= result / 32.f;
			m_rippleDest[(y + 1) * (m_config.quadCountX + 2) + x + 1] = result;
		}
	}
	//然后计算折射
	int index = 0;
	for (int y = 0; y < m_config.quadCountY; ++y) {
		for (int x = 0; x < m_config.quadCountX; ++x) {
			//       a
			//     c * d
			//       b
			float a = m_rippleDest[(y) * (m_config.quadCountX + 2) + x + 1];
			float b = m_rippleDest[(y + 2) * (m_config.quadCountX + 2) + x + 1];
			float c = m_rippleDest[(y + 1) * (m_config.quadCountX + 2) + x];
			float d = m_rippleDest[(y + 1) * (m_config.quadCountX + 2) + x + 2];
			const float offsetFactor = 4096;
			float s_offset = ((b - a) / offsetFactor);
			float t_offset = ((c - d) / offsetFactor);
			// 控制偏移量
			s_offset = (s_offset < -0.5f) ? -0.5f : s_offset;
			t_offset = (t_offset < -0.5f) ? -0.5f : t_offset;
			s_offset = (s_offset > 0.5f) ? 0.5f : s_offset;
			t_offset = (t_offset > 0.5f) ? 0.5f : t_offset;
			for (int z = 0; z < 2; ++z) {
				//实现场景的偏移
				float s_tc = (float)x / (float)m_config.quadCountX;
				s_tc *= m_texture->getMaxS();
				float t_tc = (float)(y + z) / (float)m_config.quadCountY;
				t_tc = m_texture->getMaxT() - (t_tc * m_texture->getMaxT());
				m_texCoords[index] = ccp(s_tc + s_offset, t_tc + t_offset);
				++index;
			}
			//考虑边界碰撞反弹
			if (x == m_config.quadCountX - 1) {
				for (int z = 0; z < 2; ++z) {
					float s_tc = 1;
					s_tc *= m_texture->getMaxS();
					float t_tc = (float)(y + z) / (float)m_config.quadCountY;
					t_tc = m_texture->getMaxT() - (t_tc * m_texture->getMaxT());

					m_texCoords[index] = ccp(s_tc + s_offset, t_tc + t_offset);

					++index;
				}
			}
		}
	}
	for (int y = 1; y < m_config.quadCountY; ++y) {
		for (int x = 1; x < (m_config.quadCountX + 1) * 2; x += 2) {
			// 纹理适应
			m_texCoords[(y - 1) * (m_config.quadCountX + 1) * 2 + x] = m_texCoords[y * (m_config.quadCountX + 1) * 2 + x - 1];
		}
	}

	//状态更新
	std::swap(m_rippleSource, m_rippleDest);
}

void PhysicsRippleSprite::addRipple(const CCPoint& pos, float strength) {
	CCSize textureSize = m_texture->getContentSize();

	//波纹起始位置
	int xIndex = (int)((pos.x / textureSize.width) * m_config.quadCountX);
	int yIndex = (int)((pos.y / textureSize.height) * m_config.quadCountY);

	//产生波纹
	int touchRadius = int(strength * m_config.touchRadius);
	generateRippleCoeff(touchRadius);

	for (int y = yIndex - touchRadius; y <= yIndex + touchRadius; ++y) {
		for (int x = xIndex - touchRadius; x <= xIndex + touchRadius; ++x) {
			if (x >= 0 && x < m_config.quadCountX &&
				y >= 0 && y < m_config.quadCountY) {
				// +1 to both x/y values because the border is padded
				float rippleCoeff = m_rippleCoeffs[touchRadius][(y - (yIndex - touchRadius)) * (touchRadius * 2 + 1) + x - (xIndex - touchRadius)];
				log("Coeff = %f", rippleCoeff);
				m_rippleSource[(y + 1) * (m_config.quadCountX + 2) + x + 1] += rippleCoeff;
			}
		}
	}
}

void PhysicsRippleSprite::draw(Renderer* renderer, const Mat4& transform, uint32_t flags) {
	m_customCommand.init(_globalZOrder);
	m_customCommand.func = CC_CALLBACK_0(PhysicsRippleSprite::onDraw, this, transform);
	renderer->addCommand(&m_customCommand);
}
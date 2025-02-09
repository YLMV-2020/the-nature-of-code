#pragma once

namespace NatureLab {

    struct VectorV4System {
        Math::Vector2 _position;
        Math::Vector2 _velocity;
        Math::Vector2 _acceleration;

        int _width, _height;
        Math::Vector2 _dir;

        inline VectorV4System(){
            
            this->_width = SceneAssets::SCREEN_WIDTH - SceneAssets::LIMIT_WIDTH;
            this->_height = SceneAssets::SCREEN_HEIGHT - SceneAssets::LIMIT_HEIGHT;

            this->_position = Math::Vector2(1 + rand() % _width, 1 + rand() % _height);
            this->_acceleration = Math::Vector2(rand() % 4, rand() % 9);
            this->_velocity = Math::Vector2(rand() % 4, rand() % 9);
        }

        inline void update(Math::Vector2 _mouse, int _limitVelocity, float deltaTime) {
            this->_dir = _mouse - _position;
            this->_dir = Math::Vector2::normalized(_dir) * 0.2f;

            this->_acceleration = _dir;
            this->_velocity = this->_velocity + _acceleration * deltaTime;

            this->limit(_limitVelocity);
            this->_position = this->_position + _velocity * deltaTime;
            this->checkEdges();
        }

        inline void checkEdges() {
            if (this->_position.x > _width) {
                this->_position.x = 0;
            }
            else if (this->_position.x < 0) {
                this->_position.x = _width;
            }

            if (this->_position.y > _height) {
                this->_position.y = 0;
            }
            else if (this->_position.y < 0) {
                this->_position.y = _height;
            }
        }

        inline void limit(float limit) {
            if (this->_velocity.x > 5) this->_velocity.x = limit;
            if (this->_velocity.y > 5) this->_velocity.y = limit;
            if (this->_velocity.x < -5) this->_velocity.x = -limit;
            if (this->_velocity.y < -5) this->_velocity.y = -limit;
        }
    };

    class VectorV4Nature : public INature, ITimeEvent {
    public:

        inline VectorV4Nature() {
            this->start();
        }

        inline void start() override {
            INature::start();
            this->_ballTexture = SceneAssets::getTexture("ball");

            for (int i = 0; i < _ballCount; i++)
                _natureSystem.push_back(new VectorV4System());
        }

        inline void update() override {
            ITimeEvent::update();
            INature::update();

            for (const auto &it : _natureSystem){
                it->update(_mouse, _limitVelocity, deltaTime);
            }
        }

        inline void show() override {
            INature::show();
            this->update();
            for (const auto& it : _natureSystem) {
                sprite->draw(_ballTexture, Math::Vector2(it->_position.x, it->_position.y), Math::Vector2(30, 30), 0.0f);
            }
        }

        inline void setMouse(float x, float y) {
            _mouse.x = x;
            _mouse.y = y;
        }

    private:

        std::vector<VectorV4System*> _natureSystem;
        Texture2D _ballTexture;
        Math::Vector2 _mouse;

    public:

        float _limitVelocity = 50.0f;
        int _ballCount = 10;
    };
}
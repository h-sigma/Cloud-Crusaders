#ifndef WORLD_HPP
#define WORLD_HPP


#include <array>
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "CommandQueue.hpp"
#include "Entity.hpp"
#include "Scene.hpp"
#include "State.hpp"
#include "SpriteNode.hpp"
#include "Cloud.hpp"
#include "Player.hpp"
#include "PostEffect.hpp"

class World : private sf::NonCopyable
{
    public:
        explicit World(State::Context context);
        void update(sf::Time dt);
        void draw();
        CommandQueue& getCommandQueue();
    private:
        void loadTextures();
        void updateSounds();
        void buildScene();
        void adaptPlayerVelocity();
        void adaptPlayerPosition();
        sf::FloatRect getViewBounds() const;
        sf::FloatRect getBattlefieldBounds() const;
        void spawnEnemies();
        void addEnemies();
        void addEnemy(Cloud::Type type, float x, float y);
        void guideMissiles();
        void handleCollisions();
        void destroyEntitiesOutsideView();
    private:
        enum Layers : int{
            Background, 
            AirLower,
            AirUpper, 
            LayerTotal
        };    
        struct SpawnPoint{
            SpawnPoint(Cloud::Type type, float x, float y) : type(type), x(x), y(y) {}
            SpawnPoint(SpawnPoint& ) = default;
            SpawnPoint(const SpawnPoint& ) = default;
            Cloud::Type type;
            float x;
            float y;
        };
    private:
        State::Context mContext;
        sf::RenderTexture mSceneTexture;
        BloomEffect mBloomEffect;
        sf::View mWorldView;
        sf::FloatRect mWorldBounds;
        sf::Vector2f mSpawnPosition;
        Cloud* mPlayer;

        TextureHolder mTextures;
        const FontHolder& mFontHolder;
        SceneNode mSceneGraph;
        std::array<SceneNode* , LayerTotal> mSceneLayers;
        CommandQueue mCommandQueue;
        std::vector<SpawnPoint> mEnemySpawnPositions;
        std::vector<Cloud*> mActiveEnemies;

        float mScrollSpeed;

};


#endif
#include "World.hpp"
#include "Category.hpp"
#include "Pickup.hpp"
#include "TemplateTypes.hpp"
#include "ParticleNode.hpp"
#include "SoundNode.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <set>

//World
World::World(State::Context context)
:  mContext(context)
, mSceneTexture()
, mWorldView(context.mRenderWindow->getDefaultView())
, mWorldBounds {
    0.f,
    0.f,
    mWorldView.getSize().x,
    2000
    }
,  mSpawnPosition(mWorldView.getSize().x / 2.f , mWorldBounds.height - mWorldView.getSize().y )
,  mPlayer(nullptr)
,  mFontHolder(*context.mFontHolder)
,  mSceneGraph(Category::Type::None)
,  mScrollSpeed(-15.f)
{
    mSceneTexture.create(context.mRenderWindow->getSize().x , context.mRenderWindow->getSize().y);
    loadTextures();
    buildScene();
    addEnemies();

    mWorldView.setCenter(mSpawnPosition);
}   

void World::loadTextures()
{
    mTextures.load(Textures::ID::Entities , "assets/Entities.png");
    mTextures.load(Textures::ID::Jungle , "assets/Jungle.png");
    mTextures.load(Textures::ID::Buttons, "assets/Buttons.png");
    mTextures.load(Textures::ID::Explosion, "assets/Explosion.png");
    mTextures.load(Textures::ID::Particle, "assets/Particle.png");
}

void World::buildScene()
{
    for(std::size_t i = 0 ; i < Layers::LayerTotal ; i++)
    {
        Category::Type category = (i == Layers::AirLower ) ? Category::SceneAirLayer : Category::SceneGroundLayer;
        SceneNode::Ptr layer( std::make_unique<SceneNode> (category) );
        mSceneLayers[i] = layer.get();

        mSceneGraph.attachChild(std::move(layer));
    }

    auto sounds = std::make_unique<SoundNode> (*mContext.mSoundPlayer);
    mSceneLayers[Category::SceneGroundLayer]->attachChild(std::move(sounds));

    //attach the jungle
    sf::Texture& texture = mTextures.get(Textures::ID::Jungle);
    texture.setRepeated(true);

    sf::IntRect rect(mWorldBounds);
    std::unique_ptr<SpriteNode> backgroundSprite( std::make_unique<SpriteNode> (texture , rect) );
    backgroundSprite->setPosition(mWorldBounds.left , mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

    //attach particle systems to lower air
    std::unique_ptr<ParticleNode> smokeParticle = std::make_unique<ParticleNode>(Particle::Type::Smoke, mTextures);
    mSceneLayers[AirLower]->attachChild(std::move(smokeParticle));

    std::unique_ptr<ParticleNode> propellantParticle = std::make_unique<ParticleNode>(Particle::Type::Propellant, mTextures);
    mSceneLayers[AirLower]->attachChild(std::move(propellantParticle));


    //attach the main Cloud
    std::unique_ptr<Cloud> player(std::make_unique<Cloud>(Cloud::Type::Lightning , mTextures, mFontHolder));
    mPlayer = player.get();
    mPlayer->setPosition(mSpawnPosition);
    mPlayer->setVelocity(0.f , 0.f);
    mSceneLayers[AirUpper]->attachChild(std::move(player));

    //attach the left escort
    std::unique_ptr<Cloud> leftEscort(std::make_unique<Cloud>(Cloud::Type::Bird, mTextures, mFontHolder));
    leftEscort->setPosition(-25.f , 50.f);
    leftEscort->setScale(.5f , .5f);
    mPlayer->attachChild(std::move(leftEscort));

    //attach the right escort
    std::unique_ptr<Cloud> rightEscort(std::make_unique<Cloud>(Cloud::Type::Bird, mTextures, mFontHolder));
    rightEscort->setPosition(25.f , 50.f);
    rightEscort->setScale(.5f , .5f);
    mPlayer->attachChild(std::move(rightEscort));
    
}   

void World::draw()
{
    auto& mWindow = *mContext.mRenderWindow;
    if(PostEffect::isSupported())
    {
        mSceneTexture.clear();
        mSceneTexture.setView(mWorldView);
        mSceneTexture.draw(mSceneGraph);
        mSceneTexture.display();
        mBloomEffect.apply(mSceneTexture, mWindow);
    }
    else
    {    
        mWindow.setView(mWorldView);
        mWindow.draw(mSceneGraph);    
    }
    
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::update(sf::Time dt)
{   
    //world scrolling
    mWorldView.move(0.f , dt.asSeconds() * mScrollSpeed);
    mPlayer->setVelocity(0.f,0.f);

    destroyEntitiesOutsideView();
    guideMissiles();

    while(!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop() , dt);
    adaptPlayerVelocity();

    handleCollisions();
    mSceneGraph.removeWrecks();
    spawnEnemies();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();

    updateSounds();
}

void World::adaptPlayerVelocity()
{
    sf::Vector2f vel = mPlayer->getVelocity();

    if(vel.x != 0 && vel.y != 0)
    mPlayer->setVelocity(vel / std::sqrt(2.f));

    mPlayer->setVelocity(mPlayer->getVelocity()+ sf::Vector2f{0.f,mScrollSpeed});
}

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds{
        mWorldView.getCenter() - (mWorldView.getSize()/2.f),
        mWorldView.getSize()
    };
    const float boundary = 40.f;

    sf::Vector2f position = mPlayer->getPosition();
    position.x = std::max(position.x , viewBounds.left + boundary);
    position.x = std::min(position.x , viewBounds.left + viewBounds.width - boundary);
    position.y = std::max(position.y , viewBounds.top + boundary);
    position.y = std::min(position.y , viewBounds.top + viewBounds.height - boundary);
    mPlayer->setPosition(position);
    
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect( mWorldView.getCenter() - mWorldView.getSize() / 2.0f , mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;
    return bounds;
}

void World::spawnEnemies()
{
    while(!mEnemySpawnPositions.empty() && mEnemySpawnPositions.back().y > getBattlefieldBounds().top)
    {
        SpawnPoint spawn = mEnemySpawnPositions.back();

        std::unique_ptr<Cloud> enemy (std::make_unique<Cloud> (spawn.type , mTextures, mFontHolder)); 
        enemy->setPosition(spawn.x , spawn.y);
        enemy->setRotation(180.f);

        mSceneLayers[Layers::AirUpper]->attachChild(std::move(enemy)); 

        mEnemySpawnPositions.pop_back();
    }
}

void World::addEnemies()
{
    Scale width( this->getViewBounds().left, this->getViewBounds().width );

    float wave = mWorldBounds.height;
    Scale height(0.f, this->getViewBounds().height);


    //wave 1
    wave -= height(1.5);
    addEnemy(Cloud::Type::Decree, width(0.3f), wave);
    addEnemy(Cloud::Type::Decree, width(0.7f), wave);
    addEnemy(Cloud::Type::Rod, width(0.5f), wave);
    

    //wave2
    wave -= height(0.5);
    addEnemy(Cloud::Type::Decree, width(0.3f), wave);
    addEnemy(Cloud::Type::Decree, width(0.7f), wave);
    addEnemy(Cloud::Type::Rod, width(0.5f), wave);
    

    //wave3
    wave -= height(0.4);
    addEnemy(Cloud::Type::Decree, width(0.3f), wave);
    addEnemy(Cloud::Type::Decree, width(0.7f), wave);
    addEnemy(Cloud::Type::Rod, width(0.5f), wave);
    
    
    std::sort(mEnemySpawnPositions.begin() , mEnemySpawnPositions.end() , [](SpawnPoint lhs, SpawnPoint rhs){
        return lhs.y < rhs.y;
    });
}

void World::addEnemy(Cloud::Type type, float x, float y)
{
    mEnemySpawnPositions.push_back(SpawnPoint(type,x,y));
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::Enemy;
    enemyCollector.action = derivedAction<Cloud> ([this](Cloud& enemy, sf::Time){
        if(!enemy.isDestroyed())
            mActiveEnemies.push_back(&enemy);
    });

    Command missileGuider;
    missileGuider.category = Category::AllyProjectile;
    missileGuider.action = derivedAction<Projectile>( [this](Projectile& missile, sf::Time){
        if(!missile.isGuided())
            return; // do nothing to unguided bullets
        
        float minDistance = std::numeric_limits<float>::max();
        Cloud* closestEnemy = nullptr;

        for(const auto& enemy : mActiveEnemies)
        {
            float enemyDistance = distance(missile, *enemy);

            if(minDistance > enemyDistance)
            {
                minDistance = enemyDistance;
                closestEnemy = enemy;
            }
        }

        if(closestEnemy != nullptr)
        {
            missile.guideTowards(closestEnemy->getWorldPosition());
        }
    });

    mCommandQueue.push(enemyCollector);
    mCommandQueue.push(missileGuider);

    mActiveEnemies.clear();

}

void World::handleCollisions()
{
    std::vector<SceneNode*> sceneMembers;
    sceneMembers.reserve(128);
    std::vector<SceneNode::Pair> collisionPairs;
    collisionPairs.reserve(128);

    mSceneGraph.addToSceneCollision(sceneMembers);
    mSceneGraph.findCollidingPairs(sceneMembers.begin(), sceneMembers, collisionPairs);

    for(auto& pair : collisionPairs)
    {
        if(matchesCategories(pair, Category::Player, Category::EnemyProjectile) || matchesCategories(pair, Category::Enemy, Category::AllyProjectile))
        {
            auto& cloud = static_cast<Cloud&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            cloud.damage(projectile.getDamage());
            projectile.remove();
        }
        if(matchesCategories(pair, Category::Player, Category::Pickup))
        {
            auto& player = static_cast<Cloud&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);

            pickup.apply(player);
            pickup.remove();
            player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
        }
        if(matchesCategories(pair, Category::Player, Category::Enemy))
        {
            auto& player = static_cast<Cloud&>(*pair.first);
            auto& enemy = static_cast<Cloud&>(*pair.second);

            player.damage(enemy.getHitpoints());
            enemy.remove();
        }
    }
}
void World::destroyEntitiesOutsideView()
{
    Command destroy;
    destroy.category = Category::Projectile | Category::Enemy | Category::Pickup;
    destroy.action = derivedAction<Entity>( [this](Entity& entity, sf::Time ){
        if(!getBattlefieldBounds().intersects(entity.getBoundingRect()))
        {
            entity.destroy();
        }
    });
    mCommandQueue.push(destroy);
}

void World::updateSounds()
{
    auto& mSounds = *mContext.mSoundPlayer;
    mSounds.setListenerPosition(mPlayer->getWorldPosition());
    mSounds.removeStoppedSounds();
}
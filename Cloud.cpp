
#include "Cloud.hpp"
#include "Utility.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "SoundNode.hpp"

#include <cmath>

namespace
{
    std::vector<CloudData> Table = initializeCloudData();
}

Cloud::Cloud(Cloud::Type type, const TextureHolder& textures, const FontHolder& fonts) 
: Entity(::Table[type].hitpoints)
, mType(type) 
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mExplosion(textures.get(Textures::ID::Explosion))
{

    mExplosion.setDuration(sf::seconds(1.f));
    mExplosion.setFrameSize(sf::Vector2i(256,256));
    mExplosion.setNumFrames(16);
    mExplosion.setRepeated(false);

    centerOrigin(mExplosion);
    centerOrigin(mSprite);

    //don't display HP for Bird/Escort
    if(type != Cloud::Type::Bird)
    {
        std::unique_ptr<TextNode> healthDisplay = std::make_unique<TextNode>(fonts, "");
        mHealthDisplay = healthDisplay.get();
        attachChild(std::move(healthDisplay));
    }

    //only display Missile Count for Player
    if(getCategory() == Category::Player)
    {
        std::unique_ptr<TextNode> missileDisplay = std::make_unique<TextNode>(fonts, "");
        missileDisplay->setPosition(0, 70);
        mMissileDisplay = missileDisplay.get();
        attachChild(std::move(missileDisplay));
    }

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures](SceneNode& node, sf::Time){
        createBullets(node, textures);
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time){
        createProjectile(node, Projectile::Missile, 0.f, 0.5f,  textures);
    };

    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action = [this , &textures](SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };
}

unsigned int Cloud::getCategory() const {
    switch(mType)
    {
        case Type::Lightning:
        case Type::Wind:
            return Category::Player;
        case Type::Bird:
            return Category::Ally;
        case Type::Rod:
        case Type::Decree:
            return Category::Enemy;
        default:
            return Category::None;
    }
}

void Cloud::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(isDestroyed() && mShowExplosion)
        target.draw(mExplosion, states);
    else
        target.draw(mSprite, states);
}

float Cloud::getMaxSpeed() const
{
    return Table[mType].speed;
}

void Cloud::fire()
{
    if( Table[mType].fireInterval != sf::Time::Zero)
    {
        mIsFiring = true;
    }
}

void Cloud::launchMissile()
{
    if( mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        mMissileAmmo--;
    }
}

void Cloud::checkPickupDrop(CommandQueue& commands)
{
    if (!isAllied() && randomInt(3) == 0 && !mSpawnedPickup)
            commands.push(mDropPickupCommand);

    mSpawnedPickup = true;
}

void Cloud::createPickup(SceneNode& node, const TextureHolder& textures)
{
    auto type = static_cast<Pickup::Type>(randomInt(Pickup::Type::TypeCount));
    std::unique_ptr<Pickup> temPickup = std::make_unique<Pickup>(type, textures);
    temPickup->setPosition(getWorldPosition());
    temPickup->setVelocity(0.f, 1.f);
    node.attachChild(std::move(temPickup));
}

void Cloud::checkRumbleAnimation()
{
    static int frame = 0;
    if(Table[mType].hasRollAnimation)
    {
        if(frame-- > 4)
            return;
        sf::IntRect textureRect = Table[mType].textureRect;

        if(getVelocity().x < -1.f)
            textureRect.left -= (frame = 15, textureRect.width);
        else if(getVelocity().x > 1.f)
            textureRect.left += (frame = 15, textureRect.width);
        
        if(frame == 15 || frame == 0)
        mSprite.setTextureRect(textureRect);
    }
}

void Cloud::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    if (isDestroyed())
    {
        checkPickupDrop(commands);
        mExplosion.update(dt);
        return;
    }


    checkRumbleAnimation();

    checkProjectileLaunch(dt, commands);

    updateMovementPattern(dt);

    Entity::updateCurrent(dt, commands);

    updateDisplayTexts();
}

void Cloud::checkProjectileLaunch(sf::Time dt , CommandQueue& commands)
{
    if(!isAllied() || getCategory() == Category::Ally)
        fire();
    if(mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        playLocalSound(commands, isAllied() ? SoundEffect::AlliedGunfire : SoundEffect::EnemyGunfire);
        commands.push(mFireCommand);
        mFireCountdown += sf::seconds( Table[mType].fireInterval.asSeconds() / (mFireRateLevel + 1.f));
        mIsFiring = false;
    }
    else if(mFireCountdown > sf::Time::Zero)
        mFireCountdown -= dt;
        
    if(mIsLaunchingMissile)
    {
        playLocalSound(commands, SoundEffect::LaunchMissile);
        commands.push(mMissileCommand);
        mIsLaunchingMissile = false;
    }
    
}

void Cloud::remove()
{
    Entity::remove();
    mShowExplosion = false;
}
 
bool Cloud::isAllied() const
{
    switch(mType)
    {
        case Cloud::Type::Lightning:
        case Cloud::Type::Wind:
        case Cloud::Type::Bird:
            return true;
        case Cloud::Type::Rod:
        case Cloud::Type::Decree:
            return false;
        default:
            return false;
    }
}

bool Cloud::isMarkedForRemoval() const
{
    return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void Cloud::createBullets(SceneNode& node, const TextureHolder& textures) const
{
    Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet; 
    switch(mSpreadLevel)
    {
        case 1:
            createProjectile(node, type, 0.0f, 0.5f, textures);
            break;
        case 2:
            createProjectile(node, type, -0.33f, +0.33f, textures);
            createProjectile(node, type, +0.33f, +0.33f, textures);
            break;
        case 3:
            createProjectile(node, type, -0.5f, +0.33f, textures);
            createProjectile(node, type, +0.0f, +0.5f, textures);
            createProjectile(node, type, +0.5f, 0.33f, textures);
            break;
        default:
            break;
    }
}

void Cloud::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
    std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(type,textures);
    sf::Vector2f offset{
        xOffset * mSprite.getGlobalBounds().width,
        yOffset * mSprite.getGlobalBounds().height
    };
    sf::Vector2f velocity (0, projectile->getMaxSpeed());

    float sign = isAllied() ? -1.f : +1.f;
    (*projectile).setPosition(getWorldPosition() + offset * sign);
    (*projectile).setVelocity(velocity * sign);

    node.attachChild(std::move(projectile));
}

void Cloud::updateMovementPattern(sf::Time dt)
{
    const std::vector<Direction>& directions = Table[mType].directions;
    if(!directions.empty())
    {
        if(mTravelledDistance > directions[mDirectionIndex].direction)
        {
            mDirectionIndex = (mDirectionIndex + 1) % directions.size();
            mTravelledDistance = 0.f;
        }

        float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);
        setVelocity(vx, vy);
        mTravelledDistance += getMaxSpeed()  * dt.asSeconds();
    }
}

void Cloud::updateDisplayTexts()
{
    if(mType != Cloud::Type::Bird)
    {
        mHealthDisplay->setString(std::to_string(getHitpoints()) + "HP");
        mHealthDisplay->setPosition(0.f, 30.f);
        mHealthDisplay->setRotation(-getRotation());
    }
    if(mType == Cloud::Type::Lightning)
        mHealthDisplay->setColor( sf::Color::Cyan);
    
    if(getHitpoints() < Table[mType].hitpoints / 4.f)
        mHealthDisplay->setColor( sf::Color::Red);

    if(mMissileDisplay)
    {
        if(mMissileAmmo == 0)
            mMissileDisplay->setString("");
        else 
            mMissileDisplay->setString("M: " + std::to_string(mMissileAmmo));
    }
}

sf::FloatRect Cloud::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Cloud::collectMissiles(int count)
{
    mMissileAmmo += count;
}

void Cloud::increaseSpread(int count)
{
    mSpreadLevel += count;
}

void Cloud::increaseFireRate(int count)
{
    mFireRateLevel += count;
}


void Cloud::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
    using namespace std::placeholders;
    Command playSound;
    playSound.category = Category::SoundEffect;
    playSound.action = derivedAction<SoundNode>(
        std::bind(&SoundNode::playSound, _1, effect, getWorldPosition())
    );
    commands.push(playSound);
}
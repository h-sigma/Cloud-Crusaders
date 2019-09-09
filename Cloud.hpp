#ifndef CLOUD_HPP
#define CLOUD_HPP

#include "Entity.hpp"
#include "TextNode.hpp"
#include "ResourceIdentifier.hpp"
#include "Projectile.hpp"
#include "Animation.hpp"

class Cloud : public Entity
{
    public:
        enum Type : int {
            Lightning,
            Wind,
            Bird,
            Rod,
            Decree,
            CloudCount
        };
    public:
        explicit Cloud(Type type, const TextureHolder& textures, const FontHolder& fonts); //so accidental copy and assignment are invalidated
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual unsigned int getCategory() const override;
        float getMaxSpeed() const;
        void fire();
        void launchMissile();
        void collectMissiles(int count);
        void increaseSpread(int count);
        void increaseFireRate(int count);
        virtual void remove() override;
        virtual sf::FloatRect getBoundingRect() const override;
        virtual void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
    private:
        virtual void updateCurrent(sf::Time dt, CommandQueue& commands) override;
        virtual bool isMarkedForRemoval() const;
        void updateMovementPattern(sf::Time dt);
        void updateDisplayTexts();
        void checkProjectileLaunch(sf::Time dt , CommandQueue& commands);
        bool isAllied() const;
        void createBullets(SceneNode& node, const TextureHolder& textures) const;
        void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
        void checkPickupDrop(CommandQueue& commands);
        void createPickup(SceneNode& node, const TextureHolder& textures);
        void checkRumbleAnimation();
    protected:
        Type mType;
        sf::Sprite mSprite;
        TextNode* mHealthDisplay = nullptr;
        TextNode* mMissileDisplay = nullptr;

        uint mDirectionIndex = 0;
        float mTravelledDistance = 0.f;

        bool mIsFiring = false;
        bool mIsLaunchingMissile = false;

        sf::Time mFireCountdown = sf::Time::Zero;
        int mFireRateLevel = 1;
        int mSpreadLevel = 1;

        int mMissileAmmo = 2;

        bool mSpawnedPickup = false;

        Command mFireCommand;
        Command mMissileCommand;
        Command mDropPickupCommand;

        Animation mExplosion;
        bool mShowExplosion = true;
};


#endif
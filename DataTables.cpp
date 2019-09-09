#include "DataTables.hpp"
#include "Cloud.hpp"
#include "Pickup.hpp"
#include "Projectile.hpp"
#include "Particle.hpp"

using namespace std::placeholders;

Direction::Direction(float angle, float direction) : angle(angle), direction(direction){}

std::vector<CloudData> initializeCloudData()
{
    std::vector<CloudData> data{ Cloud::Type::CloudCount };

    data[Cloud::Type::Lightning].hitpoints = 250;
    data[Cloud::Type::Lightning].speed = 400.f;
    data[Cloud::Type::Lightning].texture = Textures::ID::Entities;
    data[Cloud::Type::Lightning].textureRect = sf::IntRect{44 , 0 , 44, 63};
    data[Cloud::Type::Lightning].fireInterval = sf::seconds(0.5f);
    data[Cloud::Type::Lightning].hasRollAnimation = true;

    data[Cloud::Type::Wind].hitpoints = 100;
    data[Cloud::Type::Wind].speed = 600.f;
    data[Cloud::Type::Wind].texture = Textures::ID::Entities;
    data[Cloud::Type::Wind].textureRect = sf::IntRect{44, 0, 44, 63};
    data[Cloud::Type::Wind].fireInterval = sf::seconds(0.25f);
    
    data[Cloud::Type::Bird].hitpoints = 500;
    data[Cloud::Type::Bird].speed = 0;
    data[Cloud::Type::Bird].texture = Textures::ID::Entities;
    data[Cloud::Type::Bird].textureRect = sf::IntRect{183, 64, 30, 31};
    data[Cloud::Type::Bird].fireInterval = sf::seconds(0.50f);
    

    data[Cloud::Type::Decree].hitpoints = 20;
    data[Cloud::Type::Decree].speed = 80.f;
    data[Cloud::Type::Decree].texture = Textures::ID::Entities;
    data[Cloud::Type::Decree].textureRect = sf::IntRect{146, 0 , 79, 63};
    data[Cloud::Type::Decree].fireInterval = sf::seconds(5.f);
    data[Cloud::Type::Decree].directions.push_back(Direction(+45,80));
    data[Cloud::Type::Decree].directions.push_back(Direction(-45,160));
    data[Cloud::Type::Decree].directions.push_back(Direction(+45,80));
    
    data[Cloud::Type::Rod].hitpoints = 40;
    data[Cloud::Type::Rod].speed = 50.f;
    data[Cloud::Type::Rod].texture = Textures::ID::Entities;
    data[Cloud::Type::Rod].textureRect = sf::IntRect{243, 0 , 28, 63};
    data[Cloud::Type::Rod].fireInterval = sf::seconds(3.f);
    data[Cloud::Type::Rod].directions.push_back(Direction(+45,50));
    data[Cloud::Type::Rod].directions.push_back(Direction(0,50));
    data[Cloud::Type::Rod].directions.push_back(Direction(-45,100));
    data[Cloud::Type::Rod].directions.push_back(Direction(0,50));
    data[Cloud::Type::Rod].directions.push_back(Direction(+45,50));
    
    return std::move(data);
}

std::vector<ProjectileData>  initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Entities;
    data[Projectile::AlliedBullet].textureRect = sf::IntRect{225, 48 , 12, 55 };

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Entities;
    data[Projectile::EnemyBullet].textureRect = sf::IntRect{225, 48 , 12, 55 };

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Entities;
    data[Projectile::Missile].textureRect = sf::IntRect{160, 64 , 15, 32};

	return std::move(data);
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::Type::HealthRefill].texture = Textures::ID::Entities ;
    data[Pickup::Type::HealthRefill].textureRect = sf::IntRect{0, 64, 39, 39};
    data[Pickup::Type::HealthRefill].action = std::bind(&Cloud::repair, _1, 25 ) ;
    
    data[Pickup::Type::MissileRefill].texture = Textures::ID::Entities ;
    data[Pickup::Type::MissileRefill].textureRect = sf::IntRect{40, 64, 39, 39};
    data[Pickup::Type::MissileRefill].action = std::bind(&Cloud::collectMissiles, _1, 3 ) ;
    
    data[Pickup::Type::FireRate].texture = Textures::ID::Entities ;
    data[Pickup::Type::FireRate].textureRect = sf::IntRect{80, 64, 39, 39};
    data[Pickup::Type::FireRate].action = std::bind(&Cloud::increaseFireRate, _1, 1 ) ;
    
    data[Pickup::Type::FireSpread].texture = Textures::ID::Entities ;
    data[Pickup::Type::FireSpread].textureRect = sf::IntRect{120, 64, 39, 39};
    data[Pickup::Type::FireSpread].action = std::bind(&Cloud::increaseSpread, _1, 1 ) ;

    return data;
}


std::vector<ParticleData> initializeParticleData()
{   
    std::vector<ParticleData> data(Particle::Type::ParticleCount);

    data[Particle::Type::Propellant].color = sf::Color(250,250,50) ;
    data[Particle::Type::Propellant].lifetime = sf::seconds(.6f);

    data[Particle::Type::Smoke].color = sf::Color(50,50,50) ;
    data[Particle::Type::Smoke].lifetime = sf::seconds(4.0f); 
    // data[Particle::Type::].color = sf::Color:: ;
    // data[Particle::Type::].lifetime = sf::asSeconds();
    return data;
}


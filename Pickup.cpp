#include "Pickup.hpp"
#include "DataTables.hpp"
#include "Category.hpp"

#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace 
{
    std::vector<PickupData> Table = initializePickupData();
}


Pickup::Pickup(Type type, const TextureHolder& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
{

}

unsigned int Pickup::getCategory() const
{
    return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Cloud& cloud) const
{
    Table[mType].action(cloud);
}


void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

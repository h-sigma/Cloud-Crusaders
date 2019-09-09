#ifndef POSTEFFECT_HPP
#define POSTEFFECT_HPP

namespace sf
{
    class RenderTexture;
    class RenderTarget;
    class Shader;
}

#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"

#include <array>

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

class PostEffect
{
    public:
        virtual ~PostEffect(){}
        virtual void apply(const sf::RenderTexture& texture, sf::RenderTarget& output) = 0;
        static bool isSupported();
    protected:
        static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);
};

class BloomEffect : public PostEffect
{
    public:
        BloomEffect();
        virtual void apply(const sf::RenderTexture& texture, sf::RenderTarget& output) override;
    private:
        using RenderTextureArray = std::array<sf::RenderTexture, 2>;
    private:
        void prepareTextures(sf::Vector2u size) ;
        void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output) ;
        void blurMultipass(RenderTextureArray& renderTextures) ;
        void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor) ;
        void downsample(const sf::RenderTexture& input, sf::RenderTexture& output) ;
        void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output) ;
    private:
        sf::RenderTexture mBrightnessTexture;
        RenderTextureArray mFirstPassTextures;
        RenderTextureArray mSecondPassTextures;
        ShaderHolder mShaders;
};


#endif
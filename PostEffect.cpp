#include "PostEffect.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>

bool PostEffect::isSupported()
{
    return sf::Shader::isAvailable();
}

void PostEffect::applyShader(const sf::Shader& shader, sf::RenderTarget& output)
{
    sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());

    sf::VertexArray vertices (sf::TriangleStrip, 4);

    vertices[0] = sf::Vertex( {0, 0} , {0, 1} );
    vertices[1] = sf::Vertex( {outputSize.x, 0} , {1, 1} );
    vertices[2] = sf::Vertex( {0, outputSize.y} , {0, 0} );
    vertices[3] = sf::Vertex( {outputSize.x, outputSize.y} , {1, 0} );

    sf::RenderStates states;
    states.shader = &shader;
    states.blendMode = sf::BlendNone;
    output.draw(vertices, states);
    
}

BloomEffect::BloomEffect()
{
    mShaders.load(Shaders::BrightnessPass,   "assets/Shaders/Fullpass.vert", "assets/Shaders/Brightness.frag");
	mShaders.load(Shaders::DownSamplePass,   "assets/Shaders/Fullpass.vert", "assets/Shaders/DownSample.frag");
	mShaders.load(Shaders::GaussianBlurPass, "assets/Shaders/Fullpass.vert", "assets/Shaders/GuassianBlur.frag");
	mShaders.load(Shaders::AddPass,          "assets/Shaders/Fullpass.vert", "assets/Shaders/Add.frag");
}

void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
    prepareTextures(input.getSize());

    filterBright(input, mBrightnessTexture);
    downsample(mBrightnessTexture, mFirstPassTextures[0]);
    blurMultipass(mFirstPassTextures);
    downsample(mFirstPassTextures[0], mSecondPassTextures[0]);
    blurMultipass(mSecondPassTextures);
    add(mFirstPassTextures[0], mSecondPassTextures[0], mFirstPassTextures[1]);
    mFirstPassTextures[1].display();
    add(input, mFirstPassTextures[1], output);
}

void BloomEffect::prepareTextures(sf::Vector2u size)
{
    if (mBrightnessTexture.getSize() != size)
    {
        mBrightnessTexture.create(size.x, size.y);
        mBrightnessTexture.setSmooth(true);

        mFirstPassTextures[0].create(size.x / 2, size.y / 2);
        mFirstPassTextures[0].setSmooth(true);
        mFirstPassTextures[1].create(size.x / 2, size.y / 2);
        mFirstPassTextures[1].setSmooth(true);

        mSecondPassTextures[0].create(size.x / 4, size.y / 4);
        mSecondPassTextures[0].setSmooth(true);
        mSecondPassTextures[1].create(size.x / 4, size.y / 4);
        mSecondPassTextures[1].setSmooth(true);
    }
}

void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    sf::Shader& brightness = mShaders.get(Shaders::BrightnessPass);
    brightness.setParameter("source", input.getTexture());

    applyShader(brightness, output);
    output.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures)
{
    sf::Vector2u textureSize = renderTextures[0].getSize();

    for(int count = 0; count < 2 ; ++count)
    {
        blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f/textureSize.y));
        blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f/textureSize.x, 0.f));
    }
}

void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
    sf::Shader& gaussianBlur = mShaders.get(Shaders::GaussianBlurPass);
    gaussianBlur.setParameter("source", input.getTexture());
    gaussianBlur.setParameter("offsetFactor", offsetFactor);
    applyShader(gaussianBlur, output);
    output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output)
{
    sf::Shader& downsampler = mShaders.get(Shaders::DownSamplePass);
    downsampler.setParameter("source", input.getTexture());
    downsampler.setParameter("sourceSize", sf::Vector2f(input.getSize()) );

    applyShader(downsampler, output);
    output.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
    sf::Shader& adder = mShaders.get(Shaders::AddPass);
    adder.setParameter("source", source.getTexture());
    adder.setParameter("bloom", bloom.getTexture());

    applyShader(adder,output);
}
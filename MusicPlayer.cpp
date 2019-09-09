#include "MusicPlayer.hpp"


MusicPlayer::MusicPlayer()
: mMusic()
, mFileNames()
, mVolume(100.f)
{
	mFileNames[Music::MenuTheme]    = "assets/Music/MenuTheme.ogg";
	mFileNames[Music::MissionTheme] = "assets/Music/MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFileNames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}
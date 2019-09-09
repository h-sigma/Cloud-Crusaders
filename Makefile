#Usage
#make #compile and link the game
.PHONY = clean
CXX = g++
CXXFLAGS = -std=c++17 -W -Wall -Wno-return-type
LINKERFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

DEPENDS =   Main.o \
			StateStack.o State.o GameState.o MenuState.o PauseState.o TitleState.o SettingState.o\
			Application.o World.o CommandQueue.o Command.o Player.o \
			Component.o Container.o Button.o Label.o \
			Scene.o SpriteNode.o TextNode.o Entity.o SoundNode.o\
			Projectile.o Pickup.o Cloud.o\
			ParticleNode.o EmitterNode.o Animation.o PostEffect.o MusicPlayer.o SoundPlayer.o\
			Utility.o DataTables.o

#Main -- State -- System Components -- GUI -- Scene Hierarchy 

Main :	${DEPENDS}
	${CXX} -g -o Main ${CXXFLAGS} ${DEPENDS} ${LINKERFLAGS}

#Main
Main.o : Application.hpp

#State
StateStack.o : StateStack.hpp State.hpp ResourceHolder.hpp ResourceIdentifier.hpp StateIdentifiers.hpp
State.o : State.hpp StateStack.hpp ResourceHolder.hpp ResourceIdentifier.hpp StateIdentifiers.hpp MusicPlayer.hpp SoundPlayer.hpp
GameState.o : GameState.hpp World.hpp StateStack.hpp MusicPlayer.hpp
MenuState.o : MenuState.hpp State.hpp ResourceHolder.hpp Utility.hpp Component.hpp Button.hpp Container.hpp MusicPlayer.hpp
PauseState.o : PauseState.hpp Utility.hpp State.hpp Container.hpp Button.hpp
TitleState.o : TitleState.hpp Utility.hpp State.hpp
SettingState.o : SettingState.hpp Container.hpp Player.hpp ResourceIdentifier.hpp Button.hpp Label.hpp State.hpp Utility.hpp 

#System Components
Application.o : Application.hpp SoundPlayer.hpp MusicPlayer.hpp GameState.hpp PauseState.hpp TitleState.hpp MenuState.hpp SettingState.hpp ResourceHolder.hpp ResourceIdentifier.hpp Player.hpp
World.o : World.hpp PostEffect.hpp TemplateTypes.hpp ParticleNode.hpp State.hpp SoundNode.hpp Category.hpp ResourceHolder.hpp ResourceIdentifier.hpp CommandQueue.hpp Entity.hpp Scene.hpp SpriteNode.hpp Cloud.hpp Player.hpp
CommandQueue.o : CommandQueue.hpp Scene.hpp Command.hpp
Command.o : Command.hpp Category.hpp
Player.o : Player.hpp CommandQueue.hpp Cloud.hpp DataTables.hpp Utility.hpp

#GUI
Component.o : Component.hpp
Container.o : Container.hpp Component.hpp 
Button.o : Button.hpp Component.hpp ResourceHolder.hpp ResourceIdentifier.hpp Utility.hpp State.hpp SoundPlayer.hpp
Label.o : Label.hpp Component.hpp ResourceHolder.hpp ResourceIdentifier.hpp Utility.hpp

#Scene Hierarchy
Scene.o : Scene.hpp Category.hpp Command.hpp CommandQueue.hpp Utility.hpp

SpriteNode.o : SpriteNode.hpp Scene.hpp
TextNode.o : TextNode.hpp Scene.hpp ResourceHolder.hpp ResourceIdentifier.hpp Utility.hpp
Entity.o : Entity.hpp Scene.hpp ResourceHolder.hpp ResourceIdentifier.hpp CommandQueue.hpp Category.hpp
SoundNode.o : SoundNode.hpp SoundPlayer.hpp Scene.hpp

Cloud.o : Cloud.hpp Entity.hpp SoundNode.hpp Pickup.hpp ResourceIdentifier.hpp DataTables.hpp TextNode.hpp Utility.hpp Projectile.hpp Animation.hpp
Projectile.o : Projectile.hpp DataTables.hpp Utility.hpp Entity.hpp CommandQueue.hpp Category.hpp EmitterNode.hpp
Pickup.o : Pickup.hpp Entity.hpp ResourceIdentifier.hpp Pickup.hpp DataTables.hpp Category.hpp

ParticleNode.o : Particle.hpp ParticleNode.hpp DataTables.hpp ResourceHolder.hpp Category.hpp Scene.hpp Particle.hpp ResourceIdentifier.hpp 
EmitterNode.o : EmitterNode.hpp 
Animation.o : Animation.hpp ResourceHolder.hpp ResourceIdentifier.hpp
PostEffect.o : PostEffect.hpp ResourceHolder.hpp ResourceIdentifier.hpp
MusicPlayer.o : MusicPlayer.hpp ResourceIdentifier.hpp ResourceHolder.hpp
SoundPlayer.o : SoundPlayer.hpp ResourceHolder.hpp ResourceIdentifier.hpp

Utility.o : Utility.hpp Animation.hpp
DataTables.o : DataTables.hpp Cloud.hpp Pickup.hpp Projectile.hpp Particle.hpp

clean:
	-rm Main ${DEPENDS}
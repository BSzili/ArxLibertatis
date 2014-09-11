CXX = g++
OSTYPE = $(shell uname -s)
DEBUG ?= 0

CXXFLAGS = -Wall 

ifeq ($(DEBUG),1)
	CXXFLAGS += -DARX_DEBUG=1
#	-g
else
	CXXFLAGS += -O2
endif

CXXFLAGS += $(shell sdl-config --cflags) -Isrc -Itools -Iamiga

ifeq ($(OSTYPE),AROS)
	LDFLAGS += -lfreetype2 -lglew
else
	LDFLAGS += -lfreetype
endif
LDFLAGS += -lopenal -lz
LDFLAGS += $(shell sdl-config --libs) -lGLU
ifeq ($(OSTYPE), AmigaOS)
CXXFLAGS += -D__USE_INLINE__ -D__USE_BASETYPE__ -Wno-deprecated-declarations -fsigned-char
# -fno-strict-aliasing
# optimizing this file breaks the saving/loading
src/scene/ChangeLevel.o: CXXFLAGS += -O0
LDFLAGS += -lpthread
endif
ifeq ($(OSTYPE), AROS)
LDFLAGS += -lpthread
endif


AI_SOURCES = \
	src/ai/PathFinder.cpp \
	src/ai/PathFinderManager.cpp \
	src/ai/Paths.cpp

ANIMATION_SOURCES = \
	src/animation/Animation.cpp \
	src/animation/AnimationRender.cpp \
	src/animation/Cinematic.cpp \
	src/animation/CinematicKeyframer.cpp \
	src/animation/Intro.cpp

AUDIO_SOURCES = \
	src/audio/Ambiance.cpp \
	src/audio/Audio.cpp \
	src/audio/AudioEnvironment.cpp \
	src/audio/AudioGlobal.cpp \
	src/audio/AudioResource.cpp \
	src/audio/AudioSource.cpp \
	src/audio/Mixer.cpp \
	src/audio/Sample.cpp \
	src/audio/Stream.cpp \
	src/audio/codec/ADPCM.cpp \
	src/audio/codec/RAW.cpp \
	src/audio/codec/WAV.cpp \
	src/audio/openal/OpenALBackend.cpp \
	src/audio/openal/OpenALSource.cpp \
	src/audio/openal/OpenALUtils.cpp

CORE_SOURCES = \
	src/core/Application.cpp \
	src/core/ArxGame.cpp \
	src/core/Config.cpp \
	src/core/Core.cpp \
	src/core/GameTime.cpp \
	src/core/Localisation.cpp \
	src/core/SaveGame.cpp \
	src/core/Startup.cpp \
	src/core/Version.cpp

GAME_SOURCES = \
	src/game/Camera.cpp \
	src/game/Damage.cpp \
	src/game/Entity.cpp \
	src/game/EntityId.cpp \
	src/game/EntityManager.cpp \
	src/game/Equipment.cpp \
	src/game/Inventory.cpp \
	src/game/Item.cpp \
	src/game/Levels.cpp \
	src/game/Map.cpp \
	src/game/Missile.cpp \
	src/game/NPC.cpp \
	src/game/Player.cpp \
	src/game/Spells.cpp

GRAPHICS_SOURCES = \
	src/graphics/Draw.cpp \
	src/graphics/GraphicsModes.cpp \
	src/graphics/GraphicsUtility.cpp \
	src/graphics/Math.cpp \
	src/graphics/Renderer.cpp \
	src/graphics/data/CinematicTexture.cpp \
	src/graphics/data/FTL.cpp \
	src/graphics/data/Mesh.cpp \
	src/graphics/data/MeshManipulation.cpp \
	src/graphics/data/Progressive.cpp \
	src/graphics/data/TextureContainer.cpp \
	src/graphics/effects/CinematicEffects.cpp \
	src/graphics/effects/DrawEffects.cpp \
	src/graphics/effects/Fog.cpp \
	src/graphics/effects/SpellEffects.cpp \
	src/graphics/font/Font.cpp \
	src/graphics/font/FontCache.cpp \
	src/graphics/image/Image.cpp \
	src/graphics/image/stb_image.cpp \
	src/graphics/image/stb_image_write.cpp \
	src/graphics/particle/Particle.cpp \
	src/graphics/particle/ParticleEffects.cpp \
	src/graphics/particle/ParticleManager.cpp \
	src/graphics/particle/ParticleSystem.cpp \
	src/graphics/spells/Spells01.cpp \
	src/graphics/spells/Spells02.cpp \
	src/graphics/spells/Spells03.cpp \
	src/graphics/spells/Spells04.cpp \
	src/graphics/spells/Spells05.cpp \
	src/graphics/spells/Spells06.cpp \
	src/graphics/spells/Spells07.cpp \
	src/graphics/spells/Spells09.cpp \
	src/graphics/spells/Spells10.cpp \
	src/graphics/texture/PackedTexture.cpp \
	src/graphics/texture/Texture.cpp \
	src/graphics/texture/TextureStage.cpp \
	src/graphics/opengl/GLTexture2D.cpp \
	src/graphics/opengl/GLTextureStage.cpp \
	src/graphics/opengl/OpenGLRenderer.cpp

GUI_SOURCES = \
	src/gui/Credits.cpp \
	src/gui/Interface.cpp \
	src/gui/Menu.cpp \
	src/gui/MenuPublic.cpp \
	src/gui/MenuWidgets.cpp \
	src/gui/MiniMap.cpp \
	src/gui/Note.cpp \
	src/gui/Speech.cpp \
	src/gui/Text.cpp \
	src/gui/TextManager.cpp

INPUT_SOURCES = \
	src/input/Input.cpp \
	src/input/SDLInputBackend.cpp

IO_SOURCES = \
	src/io/CinematicLoad.cpp \
	src/io/Implode.cpp \
	src/io/IniReader.cpp \
	src/io/IniSection.cpp \
	src/io/IniWriter.cpp \
	src/io/IO.cpp \
	src/io/SaveBlock.cpp \
	src/io/Screenshot.cpp 

IO_RESOURCE_SOURCES = \
	src/io/Blast.cpp \
	src/io/resource/PakEntry.cpp \
	src/io/resource/PakReader.cpp \
	src/io/resource/ResourcePath.cpp

IO_LOGGER_SOURCES = \
	src/io/log/ConsoleLogger.cpp \
	src/io/log/LogBackend.cpp \
	src/io/log/Logger.cpp

IO_LOGGER_EXTRA_SOURCES = \
	src/io/log/FileLogger.cpp \
	src/io/log/CriticalLogger.cpp

IO_FILESYSTEM_SOURCES = \
	src/io/fs/FilePath.cpp \
	src/io/fs/FileStream.cpp \
	src/io/fs/Filesystem.cpp \
	src/io/fs/SystemPaths.cpp \
	src/io/fs/FilesystemPOSIX.cpp
	
IO_LOGGER_POSIX_SOURCES = \
	src/io/log/ColorLogger.cpp

MATH_SOURCES = \
	src/math/Angle.cpp \
	src/math/Random.cpp

PHYSICS_SOURCES = \
	src/physics/Anchors.cpp \
	src/physics/Attractors.cpp \
	src/physics/Box.cpp \
	src/physics/Clothes.cpp \
	src/physics/Collisions.cpp \
	src/physics/CollisionShapes.cpp \
	src/physics/Physics.cpp

PLATFORM_SOURCES = \
	src/platform/Dialog.cpp \
	src/platform/Environment.cpp \
	src/platform/Lock.cpp \
	src/platform/OS.cpp \
	src/platform/Platform.cpp \
	src/platform/ProgramOptions.cpp \
	src/platform/Time.cpp

PLATFORM_EXTRA_SOURCES = \
	src/platform/Thread.cpp

PLATFORM_CRASHHANDLER_SOURCES = \
	src/platform/CrashHandler.cpp

SCENE_SOURCES = \
	src/scene/ChangeLevel.cpp \
	src/scene/CinematicSound.cpp \
	src/scene/GameSound.cpp \
	src/scene/Interactive.cpp \
	src/scene/Light.cpp \
	src/scene/LinkedObject.cpp \
	src/scene/LoadLevel.cpp \
	src/scene/Object.cpp \
	src/scene/Scene.cpp

SCRIPT_SOURCES = \
	src/script/Script.cpp \
	src/script/ScriptedAnimation.cpp \
	src/script/ScriptedCamera.cpp \
	src/script/ScriptedControl.cpp \
	src/script/ScriptedConversation.cpp \
	src/script/ScriptedInterface.cpp \
	src/script/ScriptedInventory.cpp \
	src/script/ScriptedIOControl.cpp \
	src/script/ScriptedIOProperties.cpp \
	src/script/ScriptedItem.cpp \
	src/script/ScriptedLang.cpp \
	src/script/ScriptedNPC.cpp \
	src/script/ScriptedPlayer.cpp \
	src/script/ScriptedVariable.cpp \
	src/script/ScriptEvent.cpp \
	src/script/ScriptUtils.cpp

UTIL_SOURCES = \
	src/util/String.cpp \
	src/util/cmdline/Parser.cpp

WINDOW_SOURCES = \
	src/window/RenderWindow.cpp \
	src/window/Window.cpp \
	src/window/SDLWindow.cpp

SRC = \
	$(AI_SOURCES) \
	$(ANIMATION_SOURCES) \
	$(AUDIO_SOURCES) \
	$(CORE_SOURCES) \
	$(GAME_SOURCES) \
	$(GRAPHICS_SOURCES) \
	$(GUI_SOURCES) \
	$(INPUT_SOURCES) \
	$(IO_SOURCES) \
	$(IO_RESOURCE_SOURCES) \
	$(IO_FILESYSTEM_SOURCES) \
	$(IO_LOGGER_SOURCES) \
	$(IO_LOGGER_EXTRA_SOURCES) \
	$(MATH_SOURCES) \
	$(PHYSICS_SOURCES) \
	$(PLATFORM_SOURCES) \
	$(PLATFORM_EXTRA_SOURCES) \
	$(PLATFORM_CRASHHANDLER_SOURCES) \
	$(SCENE_SOURCES) \
	$(SCRIPT_SOURCES) \
	$(UTIL_SOURCES) \
	$(WINDOW_SOURCES)

OBJ = $(SRC:.cpp=.o)

UNPAK_SRC = \
	$(PLATFORM_SOURCES) \
	$(IO_FILESYSTEM_SOURCES) \
	$(IO_LOGGER_SOURCES) \
	$(IO_RESOURCE_SOURCES) \
	$(UTIL_SOURCES) \
	tools/unpak/UnPak.cpp

UNPAK_OBJ = $(UNPAK_SRC:.cpp=.o)

SAVETOOL_SRC = \
	$(PLATFORM_SOURCES) \
	$(IO_FILESYSTEM_SOURCES) \
	$(IO_LOGGER_SOURCES) \
	$(IO_RESOURCE_SOURCES) \
	$(UTIL_SOURCES) \
	src/core/Localisation.cpp \
	src/io/SaveBlock.cpp \
	src/io/IniReader.cpp \
	src/io/IniSection.cpp \
	tools/savetool/SaveFix.cpp \
	tools/savetool/SaveTool.cpp \
	tools/savetool/SaveView.cpp

SAVETOOL_OBJ = $(SAVETOOL_SRC:.cpp=.o)

all: arx arxunpak savetool

arx: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

arxunpak: $(UNPAK_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

savetool: $(SAVETOOL_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	-rm -f $(OBJ) $(UNPAK_OBJ) $(SAVETOOL_OBJ)

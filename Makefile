BINPREFIX ?=
CXX = $(BINPREFIX)g++
OSTYPE ?= $(shell uname -s)
DEBUG ?= 0
SDL_CFLAGS ?= $(shell sdl-config --cflags)
SDL_LDFLAGS ?= $(shell sdl-config --libs)
RM = rm -f
STRIPBIN = $(BINPREFIX)strip -K ___stack
ifeq ($(PLATFORM), AROS)
STRIPBIN += --strip-unneeded
endif

CXXFLAGS = -Wall -fno-rtti

ifeq ($(DEBUG),1)
	CXXFLAGS += -DARX_DEBUG=1
#	-g
else
	CXXFLAGS += -O2
endif

CXXFLAGS += $(SDL_CFLAGS) -Isrc -Itools -Iamiga

ifeq ($(OSTYPE),AROS)
	LDFLAGS += -lfreetype2 -lglew
else
	LDFLAGS += -lfreetype
endif
LDFLAGS += -lopenal -lz
LDFLAGS += $(SDL_LDFLAGS) -lGLU
ifeq ($(OSTYPE), AmigaOS)
CXXFLAGS += -D__USE_INLINE__ -D__USE_BASETYPE__ -Wno-deprecated-declarations -fsigned-char
# -fno-strict-aliasing
# optimizing this file breaks the saving/loading
src/scene/ChangeLevel.o: CXXFLAGS += -O0
endif
LDFLAGS += -lpthread


AI_SOURCES = \
	src/ai/Anchors.cpp \
	src/ai/PathFinder.cpp \
	src/ai/PathFinderManager.cpp \
	src/ai/Paths.cpp

ANIMATION_SOURCES = \
	src/animation/Animation.cpp \
	src/animation/AnimationRender.cpp \
	src/animation/Skeleton.cpp \
	src/animation/Intro.cpp

CINEMATIC_SOURCES = \
	src/cinematic/Cinematic.cpp \
	src/cinematic/CinematicController.cpp \
	src/cinematic/CinematicKeyframer.cpp \
	src/cinematic/CinematicLoad.cpp \
	src/cinematic/CinematicTexture.cpp \
	src/cinematic/CinematicEffects.cpp \
	src/cinematic/CinematicSound.cpp

AUDIO_OPENAL_SOURCES = \
	src/audio/openal/OpenALBackend.cpp \
	src/audio/openal/OpenALSource.cpp \
	src/audio/openal/OpenALUtils.cpp

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
	$(AUDIO_OPENAL_SOURCES)

CORE_SOURCES = \
	src/core/Application.cpp \
	src/core/ArxGame.cpp \
	src/core/Benchmark.cpp \
	src/core/Config.cpp \
	src/core/Core.cpp \
	src/core/GameTime.cpp \
	src/core/Localisation.cpp \
	src/core/SaveGame.cpp \
	src/core/Startup.cpp \

VERSION_FILE = \
	amiga/core/Version.cpp

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
	src/game/Missile.cpp \
	src/game/NPC.cpp \
	src/game/Player.cpp \
	src/game/Spells.cpp \
	src/game/effect/Quake.cpp \
	src/game/effect/ParticleSystems.cpp \
	src/game/magic/Precast.cpp \
	src/game/magic/Rune.cpp \
	src/game/magic/RuneDraw.cpp \
	src/game/magic/Spell.cpp \
	src/game/magic/SpellData.cpp \
	src/game/magic/SpellRecognition.cpp \
	src/game/magic/spells/SpellsLvl01.cpp \
	src/game/magic/spells/SpellsLvl02.cpp \
	src/game/magic/spells/SpellsLvl03.cpp \
	src/game/magic/spells/SpellsLvl04.cpp \
	src/game/magic/spells/SpellsLvl05.cpp \
	src/game/magic/spells/SpellsLvl06.cpp \
	src/game/magic/spells/SpellsLvl07.cpp \
	src/game/magic/spells/SpellsLvl08.cpp \
	src/game/magic/spells/SpellsLvl09.cpp \
	src/game/magic/spells/SpellsLvl10.cpp \
	src/game/npc/Dismemberment.cpp \
	src/game/spell/FlyingEye.cpp \
	src/game/spell/Cheat.cpp

GRAPHICS_OPENGL_SOURCES = \
	src/graphics/opengl/GLDebug.cpp \
	src/graphics/opengl/GLTexture.cpp \
	src/graphics/opengl/GLTextureStage.cpp \
	src/graphics/opengl/GLVertexBuffer.cpp \
	src/graphics/opengl/OpenGLRenderer.cpp

GRAPHICS_SOURCES = \
	src/graphics/Draw.cpp \
	src/graphics/DrawLine.cpp \
	src/graphics/DrawDebug.cpp \
	src/graphics/GlobalFog.cpp \
	src/graphics/Math.cpp \
	src/graphics/Raycast.cpp \
	src/graphics/Renderer.cpp \
	src/graphics/RenderBatcher.cpp \
	src/graphics/data/FTL.cpp \
	src/graphics/data/Mesh.cpp \
	src/graphics/data/MeshManipulation.cpp \
	src/graphics/data/TextureContainer.cpp \
	src/graphics/effects/BlobShadow.cpp \
	src/graphics/effects/Cabal.cpp \
	src/graphics/effects/Fade.cpp \
	src/graphics/effects/Field.cpp \
	src/graphics/effects/Fissure.cpp \
	src/graphics/effects/FloatingStones.cpp \
	src/graphics/effects/Fog.cpp \
	src/graphics/effects/SpellEffects.cpp \
	src/graphics/effects/Halo.cpp \
	src/graphics/effects/LightFlare.cpp \
	src/graphics/effects/Lightning.cpp \
	src/graphics/effects/MagicMissile.cpp \
	src/graphics/effects/PolyBoom.cpp \
	src/graphics/effects/RotatingCone.cpp \
	src/graphics/effects/Trail.cpp \
	src/graphics/font/Font.cpp \
	src/graphics/font/FontCache.cpp \
	src/graphics/image/Image.cpp \
	src/graphics/image/ImageColorKey.cpp \
	src/graphics/image/stb_image.cpp \
	src/graphics/image/stb_image_write.cpp \
	src/graphics/particle/Particle.cpp \
	src/graphics/particle/ParticleEffects.cpp \
	src/graphics/particle/ParticleManager.cpp \
	src/graphics/particle/ParticleSystem.cpp \
	src/graphics/particle/MagicFlare.cpp \
	src/graphics/particle/Spark.cpp \
	src/graphics/spells/Spells05.cpp \
	src/graphics/texture/PackedTexture.cpp \
	src/graphics/texture/Texture.cpp \
	src/graphics/texture/TextureStage.cpp \
	$(GRAPHICS_OPENGL_SOURCES)

GUI_SOURCES = \
	src/gui/CharacterCreation.cpp \
	src/gui/CinematicBorder.cpp \
	src/gui/Console.cpp \
	src/gui/Credits.cpp \
	src/gui/Cursor.cpp \
	src/gui/DebugHud.cpp \
	src/gui/DebugUtils.cpp \
	src/gui/Hud.cpp \
	src/gui/Interface.cpp \
	src/gui/LoadLevelScreen.cpp \
	src/gui/MainMenu.cpp \
	src/gui/Menu.cpp \
	src/gui/MenuPublic.cpp \
	src/gui/MenuWidgets.cpp \
	src/gui/MiniMap.cpp \
	src/gui/Note.cpp \
	src/gui/Speech.cpp \
	src/gui/Text.cpp \
	src/gui/TextManager.cpp \
	src/gui/book/Book.cpp \
	src/gui/book/Necklace.cpp \
	src/gui/hud/HudCommon.cpp \
	src/gui/hud/PlayerInventory.cpp \
	src/gui/hud/SecondaryInventory.cpp \
	src/gui/menu/MenuCursor.cpp \
	src/gui/menu/MenuFader.cpp \
	src/gui/widget/ButtonWidget.cpp \
	src/gui/widget/CheckboxWidget.cpp \
	src/gui/widget/CycleTextWidget.cpp \
	src/gui/widget/PanelWidget.cpp \
	src/gui/widget/SliderWidget.cpp \
	src/gui/widget/TextWidget.cpp \
	src/gui/widget/Widget.cpp \
	src/gui/widget/WidgetContainer.cpp

INPUT_SDL1_SOURCES = \
	src/input/SDL1InputBackend.cpp

INPUT_SOURCES = \
	src/input/Input.cpp \
	src/input/TextInput.cpp \
	$(INPUT_SDL1_SOURCES)

IO_SOURCES = \
	src/io/IniReader.cpp \
	src/io/IniSection.cpp \
	src/io/IniWriter.cpp \
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
	src/physics/Attractors.cpp \
	src/physics/Collisions.cpp \
	src/physics/CollisionShapes.cpp \
	src/physics/Projectile.cpp \
	src/physics/Physics.cpp


PLATFORM_SOURCES = \
	src/platform/Alignment.cpp \
	src/platform/Environment.cpp \
	src/platform/Lock.cpp \
	src/platform/OS.cpp \
	src/platform/Platform.cpp \
	src/platform/Process.cpp \
	src/platform/ProgramOptions.cpp \
	src/platform/Time.cpp

PLATFORM_EXTRA_SOURCES = \
	src/platform/Dialog.cpp \
	src/platform/Thread.cpp

PLATFORM_CRASHHANDLER_SOURCES = \
	src/platform/CrashHandler.cpp

PLATFORM_PROFILER_SOURCES = \
	src/platform/profiler/Profiler.cpp

SCENE_SOURCES = \
	src/scene/Background.cpp \
	src/scene/ChangeLevel.cpp \
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

UTIL_MD5_SOURCES = \
	src/util/MD5.cpp

UTIL_SOURCES = \
	src/util/String.cpp \
	src/util/cmdline/CommandLine.cpp \
	src/util/cmdline/Parser.cpp

WINDOW_SOURCES = \
	src/window/RenderWindow.cpp \
	src/window/Window.cpp \
	src/window/SDL1Window.cpp

SRC = \
	$(AI_SOURCES) \
	$(ANIMATION_SOURCES) \
	$(CINEMATIC_SOURCES) \
	$(AUDIO_SOURCES) \
	$(AUDIO_OPENAL_SOURCES) \
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
	$(PLATFORM_PROFILER_SOURCES) \
	$(SCENE_SOURCES) \
	$(SCRIPT_SOURCES) \
	$(UTIL_SOURCES) \
	$(VERSION_FILE) \
	$(WINDOW_SOURCES)

OBJ = $(SRC:.cpp=.o)

UNPAK_SRC = \
	$(PLATFORM_SOURCES) \
	$(IO_FILESYSTEM_SOURCES) \
	$(IO_LOGGER_SOURCES) \
	$(IO_RESOURCE_SOURCES) \
	$(UTIL_SOURCES) \
	$(UTIL_MD5_SOURCES) \
	$(VERSION_FILE) \
	tools/unpak/UnPak.cpp

UNPAK_OBJ = $(UNPAK_SRC:.cpp=.o)

SAVETOOL_SRC = \
	$(PLATFORM_SOURCES) \
	$(IO_FILESYSTEM_SOURCES) \
	$(IO_LOGGER_SOURCES) \
	$(IO_RESOURCE_SOURCES) \
	$(UTIL_SOURCES) \
	$(VERSION_FILE) \
	src/core/Localisation.cpp \
	src/io/SaveBlock.cpp \
	src/io/IniReader.cpp \
	src/io/IniSection.cpp \
	tools/savetool/SaveFix.cpp \
	tools/savetool/SaveRename.cpp \
	tools/savetool/SaveTool.cpp \
	tools/savetool/SaveView.cpp

SAVETOOL_OBJ = $(SAVETOOL_SRC:.cpp=.o)

all: arx
# arxunpak savetool

arx: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)
ifeq ($(DEBUG),0)
	$(STRIPBIN) $@
endif

arxunpak: $(UNPAK_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

savetool: $(SAVETOOL_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJ) $(UNPAK_OBJ) $(SAVETOOL_OBJ)

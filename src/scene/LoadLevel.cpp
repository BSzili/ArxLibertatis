/*
 * Copyright 2011-2017 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Based on:
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
// Code: Cyril Meynier
//
// Copyright (c) 1999-2000 ARKANE Studios SA. All rights reserved

#include "scene/LoadLevel.h"

#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <boost/algorithm/string/case_conv.hpp>

#if defined(__MORPHOS__) || defined(__amigaos4__)
#include <SDL_endian.h>
#endif

#include "ai/PathFinderManager.h"
#include "ai/Paths.h"

#include "core/Application.h"
#include "core/GameTime.h"
#include "core/Config.h"
#include "core/Core.h"

#include "game/EntityManager.h"
#include "game/Levels.h"
#include "game/Player.h"

#include "gui/LoadLevelScreen.h"
#include "gui/MiniMap.h"
#include "gui/Interface.h"

#include "graphics/Math.h"
#include "graphics/data/FTL.h"
#include "graphics/data/TextureContainer.h"
#include "graphics/effects/Fade.h"
#include "graphics/effects/Fog.h"
#include "graphics/particle/ParticleEffects.h"

#include "io/resource/ResourcePath.h"
#include "io/resource/PakReader.h"
#include "io/Blast.h"
#include "io/log/Logger.h"

#include "physics/CollisionShapes.h"

#include "scene/Object.h"
#include "scene/GameSound.h"
#include "scene/Interactive.h"
#include "scene/LevelFormat.h"
#include "scene/Light.h"

#include "util/String.h"


extern bool bGCroucheToggle;

#if defined(__MORPHOS__) || defined(__amigaos4__)
// SDL 2.x compatibility
typedef union
{
	float f;
	int i;
} floatint_t;

static inline float SDL_SwapFloatLE(float f)
{
	floatint_t out;
	out.f  = f;
	out.i = SDL_SwapLE32(out.i);
	return out.f;
}
#endif

Entity * LoadInter_Ex(const res::path & classPath, EntityInstance instance,
                      const Vec3f & pos, const Anglef & angle) {
	
	EntityHandle t = entities.getById(EntityId(classPath, instance));
	if(t != EntityHandle()) {
		return entities[t];
	}
	
	arx_assert(instance != 0);
	
	Entity * io = AddInteractive(classPath, instance, NO_MESH | NO_ON_LOAD);
	if(!io) {
		return NULL;
	}
	
	RestoreInitialIOStatusOfIO(io);
	ARX_INTERACTIVE_HideGore(io);
	
	io->lastpos = io->initpos = io->pos = pos;
	io->move = Vec3f_ZERO;
	io->initangle = io->angle = angle;
	
	if(PakDirectory * dir = g_resources->getDirectory(io->instancePath())) {
		loadScript(io->over_script, dir->getFile(io->className() + ".asl"));
	}
	
	if(SendIOScriptEvent(NULL, io, SM_LOAD) == ACCEPT && io->obj == NULL) {
		bool pbox = (io->ioflags & IO_ITEM) == IO_ITEM;
		io->obj = loadObject(io->classPath() + ".teo", pbox);
		if(io->ioflags & IO_NPC) {
			EERIE_COLLISION_Cylinder_Create(io);
		}
	}
	
	return io;
}

static ColorBGRA savedColorConversion(u32 bgra) {
#if defined(__MORPHOS__) || defined(__amigaos4__)
	bgra = SDL_SwapLE32(bgra);
#endif
	return ColorBGRA(bgra);
}

static long LastLoadedLightningNb = 0;
static ColorBGRA * LastLoadedLightning = NULL;

bool DanaeLoadLevel(const res::path & file, bool loadEntities) {
	
	LogInfo << "Loading level " << file;
	
	CURRENTLEVEL = GetLevelNumByName(file.string());
	
	res::path lightingFileName = res::path(file).set_ext("llf");

	LogDebug("fic2 " << lightingFileName);
	LogDebug("fileDlf " << file);

	size_t FileSize = 0;
	char * dat = g_resources->readAlloc(file, FileSize);
	if(!dat) {
		LogError << "Unable to find " << file;
		return false;
	}
	
	g_requestLevelInit = true;
	
	PakFile * lightingFile = g_resources->getFile(lightingFileName);
	
	progressBarAdvance();
	LoadLevelScreen();
	
	size_t pos = 0;
	
	DANAE_LS_HEADER dlh;
	memcpy(&dlh, dat + pos, sizeof(DANAE_LS_HEADER));
#if defined(__MORPHOS__) || defined(__amigaos4__)
	dlh.version = SDL_SwapFloatLE(dlh.version);
	dlh.time = SDL_SwapLE32(dlh.time);
	dlh.pos_edit.x = SDL_SwapFloatLE(dlh.pos_edit.x);
	dlh.pos_edit.y = SDL_SwapFloatLE(dlh.pos_edit.y);
	dlh.pos_edit.z = SDL_SwapFloatLE(dlh.pos_edit.z);
	dlh.angle_edit.a = SDL_SwapFloatLE(dlh.angle_edit.a);
	dlh.angle_edit.b = SDL_SwapFloatLE(dlh.angle_edit.b);
	dlh.angle_edit.g = SDL_SwapFloatLE(dlh.angle_edit.g);

	dlh.nb_scn = SDL_SwapLE32(dlh.nb_scn);
	dlh.nb_inter = SDL_SwapLE32(dlh.nb_inter);
	dlh.nb_nodes = SDL_SwapLE32(dlh.nb_nodes);
	dlh.nb_nodeslinks = SDL_SwapLE32(dlh.nb_nodeslinks);
	dlh.nb_zones = SDL_SwapLE32(dlh.nb_zones);
	dlh.lighting = SDL_SwapLE32(dlh.lighting);
	//s32 Bpad[256];
	dlh.nb_lights = SDL_SwapLE32(dlh.nb_lights);
	dlh.nb_fogs = SDL_SwapLE32(dlh.nb_fogs);
	
	dlh.nb_bkgpolys = SDL_SwapLE32(dlh.nb_bkgpolys);
	dlh.nb_ignoredpolys = SDL_SwapLE32(dlh.nb_ignoredpolys);
	dlh.nb_childpolys = SDL_SwapLE32(dlh.nb_childpolys);
	dlh.nb_paths = SDL_SwapLE32(dlh.nb_paths);
	//s32 pad[250];
	dlh.offset.x = SDL_SwapFloatLE(dlh.offset.x);
	dlh.offset.y = SDL_SwapFloatLE(dlh.offset.y);
	dlh.offset.z = SDL_SwapFloatLE(dlh.offset.z);
	//f32 fpad[253];
	//char cpad[4096];
	//s32 bpad[256];
#endif
	pos += sizeof(DANAE_LS_HEADER);
	
	LogDebug("dlh.version " << dlh.version << " header size " << sizeof(DANAE_LS_HEADER));
	
	if(dlh.version > DLH_CURRENT_VERSION) {
		LogError << "Unexpected level file version: " << dlh.version << " for " << file;
		free(dat);
		dat = NULL;
		return false;
	}
	
	// using compression
	if(dlh.version >= 1.44f) {
		char * torelease = dat;
		dat = blastMemAlloc(dat + pos, FileSize - pos, FileSize);
		free(torelease);
		pos = 0;
		if(!dat) {
			LogError << "Could not decompress level file " << file;
			return false;
		}
	}
	
	player.desiredangle = player.angle = dlh.angle_edit;
	
	if(strcmp(dlh.ident, "DANAE_FILE") != 0) {
		LogError << "Not a valid file " << file << ": \"" << util::loadString(dlh.ident) << '"';
		return false;
	}
	
	LogDebug("Loading Scene");
	
	Vec3f trans = Vec3f_ZERO;
	
	// Loading Scene
	if(dlh.nb_scn > 0) {
		
		const DANAE_LS_SCENE * dls = reinterpret_cast<const DANAE_LS_SCENE *>(dat + pos);
		pos += sizeof(DANAE_LS_SCENE);
		
		res::path scene = res::path::load(util::loadString(dls->name));
		
		if(FastSceneLoad(scene, trans)) {
			LogDebug("done loading scene");
		} else {
			LogError << "Fast loading scene failed";
		}
		
		EERIEPOLY_Compute_PolyIn();
		LastLoadedScene = scene;
	}
	
	player.pos = dlh.pos_edit.toVec3() + trans;
	
	float increment = 0;
	if(dlh.nb_inter > 0) {
		increment = (60.f / (float)dlh.nb_inter);
	} else {
		progressBarAdvance(60);
		LoadLevelScreen();
	}
	
	for(long i = 0 ; i < dlh.nb_inter ; i++) {
		
		progressBarAdvance(increment);
		LoadLevelScreen();
		
#if defined(__MORPHOS__) || defined(__amigaos4__)
		DANAE_LS_INTER *dli = reinterpret_cast<DANAE_LS_INTER *>(dat + pos);
		DANAE_LS_INTER dli_copy = *dli;
		dli = &dli_copy;
		
		dli->pos.x = SDL_SwapFloatLE(dli->pos.x);
		dli->pos.y = SDL_SwapFloatLE(dli->pos.y);
		dli->pos.z = SDL_SwapFloatLE(dli->pos.z);
		dli->angle.a = SDL_SwapFloatLE(dli->angle.a);
		dli->angle.b = SDL_SwapFloatLE(dli->angle.b);
		dli->angle.g = SDL_SwapFloatLE(dli->angle.g);
		dli->ident = SDL_SwapLE32(dli->ident);
		dli->flags = SDL_SwapLE32(dli->flags);
#else
		const DANAE_LS_INTER * dli = reinterpret_cast<const DANAE_LS_INTER *>(dat + pos);
#endif
		pos += sizeof(DANAE_LS_INTER);
		
		if(loadEntities) {
			
			std::string pathstr = boost::to_lower_copy(util::loadString(dli->name));
			
			size_t graphPos = pathstr.find("graph");
			if(graphPos != std::string::npos) {
				pathstr = pathstr.substr(graphPos);
			}
			
			res::path classPath = res::path::load(pathstr).remove_ext();
			LoadInter_Ex(classPath, dli->ident, dli->pos.toVec3() + trans, dli->angle);
		}
	}
	
	if(dlh.lighting) {
		
#if defined(__MORPHOS__) || defined(__amigaos4__)
		DANAE_LS_LIGHTINGHEADER *dll = reinterpret_cast<DANAE_LS_LIGHTINGHEADER *>(dat + pos);
		DANAE_LS_LIGHTINGHEADER dll_copy = *dll;
		dll = &dll_copy;
		
		dll->nb_values = SDL_SwapLE32(dll->nb_values);
		dll->ViewMode = SDL_SwapLE32(dll->ViewMode);
		dll->ModeLight = SDL_SwapLE32(dll->ModeLight);
#else
		const DANAE_LS_LIGHTINGHEADER * dll = reinterpret_cast<const DANAE_LS_LIGHTINGHEADER *>(dat + pos);
#endif
		pos += sizeof(DANAE_LS_LIGHTINGHEADER);
		long bcount = dll->nb_values;
		
		if(!lightingFile) {
			
			LastLoadedLightningNb = bcount;
			
			// DANAE_LS_VLIGHTING
			free(LastLoadedLightning);
			ColorBGRA * ll = LastLoadedLightning = (ColorBGRA *)malloc(sizeof(ColorBGRA) * bcount);
			
			if(dlh.version > 1.001f) {
				std::transform((u32 *)(dat + pos), (u32 *)(dat + pos) + bcount, LastLoadedLightning, savedColorConversion);
				pos += sizeof(u32) * bcount;
			} else {
				while(bcount) {
#if defined(__MORPHOS__) || defined(__amigaos4__)
					DANAE_LS_VLIGHTING *dlv = reinterpret_cast<DANAE_LS_VLIGHTING *>(dat + pos);
					DANAE_LS_VLIGHTING dlv_copy = *dlv;
					dlv = &dlv_copy;
					
					dlv->r = SDL_SwapLE32(dlv->r);
					dlv->g = SDL_SwapLE32(dlv->g);
					dlv->b = SDL_SwapLE32(dlv->b);
#else
					const DANAE_LS_VLIGHTING * dlv = reinterpret_cast<const DANAE_LS_VLIGHTING *>(dat + pos);
#endif
					pos += sizeof(DANAE_LS_VLIGHTING);
					*ll = Color((dlv->r & 255), (dlv->g & 255), (dlv->b & 255), 255).toBGRA();
					ll++;
					bcount--;
				}
			}
			
		} else {
			pos += sizeof(u32) * bcount;
		}
	}
	
	progressBarAdvance();
	LoadLevelScreen();
	
	long nb_lights = (dlh.version < 1.003f) ? 0 : dlh.nb_lights;
	
	if(!lightingFile) {
		
		if(nb_lights != 0) {
			EERIE_LIGHT_GlobalInit();
		}
		
		for(long i = 0; i < nb_lights; i++) {
			
#if defined(__MORPHOS__) || defined(__amigaos4__)
			DANAE_LS_LIGHT *dlight = reinterpret_cast<DANAE_LS_LIGHT *>(dat + pos);
			DANAE_LS_LIGHT dlight_copy = *dlight;
			dlight = &dlight_copy;
			
			dlight->pos.x = SDL_SwapFloatLE(dlight->pos.x);
			dlight->pos.y = SDL_SwapFloatLE(dlight->pos.y);
			dlight->pos.z = SDL_SwapFloatLE(dlight->pos.z);
			dlight->rgb.r = SDL_SwapFloatLE(dlight->rgb.r);
			dlight->rgb.g = SDL_SwapFloatLE(dlight->rgb.g);
			dlight->rgb.b = SDL_SwapFloatLE(dlight->rgb.b);

			dlight->fallstart = SDL_SwapFloatLE(dlight->fallstart);
			dlight->fallend = SDL_SwapFloatLE(dlight->fallend);
			dlight->intensity = SDL_SwapFloatLE(dlight->intensity);
			dlight->i = SDL_SwapFloatLE(dlight->i);
			dlight->ex_flicker.r = SDL_SwapFloatLE(dlight->ex_flicker.r);
			dlight->ex_flicker.g = SDL_SwapFloatLE(dlight->ex_flicker.g);
			dlight->ex_flicker.b = SDL_SwapFloatLE(dlight->ex_flicker.b);
			dlight->ex_radius = SDL_SwapFloatLE(dlight->ex_radius);
			dlight->ex_frequency = SDL_SwapFloatLE(dlight->ex_frequency);
			dlight->ex_size = SDL_SwapFloatLE(dlight->ex_size);
			dlight->ex_speed = SDL_SwapFloatLE(dlight->ex_speed);
			dlight->ex_flaresize = SDL_SwapFloatLE(dlight->ex_flaresize);
			//f32 fpadd[24];
			dlight->extras = SDL_SwapLE32(dlight->extras);
			//s32 lpadd[31];
#else
			const DANAE_LS_LIGHT * dlight = reinterpret_cast<const DANAE_LS_LIGHT *>(dat + pos);
#endif
			pos += sizeof(DANAE_LS_LIGHT);
			
			long j = EERIE_LIGHT_Create();
			if(j >= 0) {
				EERIE_LIGHT * el = g_staticLights[j];
				
				el->exist = 1;
				el->treat = 1;
				el->fallend = dlight->fallend;
				el->fallstart = dlight->fallstart;
				el->falldiffmul = 1.f / (el->fallend - el->fallstart);
				el->intensity = dlight->intensity;
				el->pos = dlight->pos.toVec3();
				el->rgb = dlight->rgb;
				
				el->extras = ExtrasType::load(dlight->extras);
				
				el->ex_flicker = dlight->ex_flicker;
				el->ex_radius = dlight->ex_radius;
				el->ex_frequency = dlight->ex_frequency;
				el->ex_size = dlight->ex_size;
				el->ex_speed = dlight->ex_speed;
				el->m_ignitionLightHandle = LightHandle();
				el->sample = audio::INVALID_ID;
				
				if((el->extras & EXTRAS_SPAWNFIRE)) {
					el->extras |= EXTRAS_FLARE;
					if(el->extras & EXTRAS_FIREPLACE) {
						el->ex_flaresize = 95.f;
					} else {
						el->ex_flaresize = 40.f;
					}
				}
			}
		}
		
	} else {
		pos += sizeof(DANAE_LS_LIGHT) * nb_lights;
	}
	
	LogDebug("Loading FOGS");
	ARX_FOGS_Clear();
	
	for(long i = 0; i < dlh.nb_fogs; i++) {
		
#if defined(__MORPHOS__) || defined(__amigaos4__)
		DANAE_LS_FOG *dlf = reinterpret_cast<DANAE_LS_FOG *>(dat + pos);
		DANAE_LS_FOG dlf_copy = *dlf;
		dlf = &dlf_copy;
		
		dlf->pos.x = SDL_SwapFloatLE(dlf->pos.x);
		dlf->pos.y = SDL_SwapFloatLE(dlf->pos.y);
		dlf->pos.z = SDL_SwapFloatLE(dlf->pos.z);
		dlf->rgb.r = SDL_SwapFloatLE(dlf->rgb.r);
		dlf->rgb.g = SDL_SwapFloatLE(dlf->rgb.g);
		dlf->rgb.b = SDL_SwapFloatLE(dlf->rgb.b);
		dlf->size = SDL_SwapFloatLE(dlf->size);
		dlf->special = SDL_SwapLE32(dlf->special);
		dlf->scale = SDL_SwapFloatLE(dlf->scale);
		dlf->move.x = SDL_SwapFloatLE(dlf->move.x);
		dlf->move.y = SDL_SwapFloatLE(dlf->move.y);
		dlf->move.z = SDL_SwapFloatLE(dlf->move.z);
		dlf->angle.a = SDL_SwapFloatLE(dlf->angle.a);
		dlf->angle.b = SDL_SwapFloatLE(dlf->angle.b);
		dlf->angle.g = SDL_SwapFloatLE(dlf->angle.g);
		dlf->speed = SDL_SwapFloatLE(dlf->speed);
		dlf->rotatespeed = SDL_SwapFloatLE(dlf->rotatespeed);
		dlf->tolive = SDL_SwapLE32(dlf->tolive);
		dlf->blend = SDL_SwapLE32(dlf->blend);
		dlf->frequency = SDL_SwapFloatLE(dlf->frequency);
#else
		const DANAE_LS_FOG * dlf = reinterpret_cast<const DANAE_LS_FOG *>(dat + pos);
#endif
		pos += sizeof(DANAE_LS_FOG);
		
		long n = ARX_FOGS_GetFree();
		if(n > -1) {
			
			FOG_DEF * fd = &fogs[n];
			fd->exist = true;
			fd->rgb = dlf->rgb;
			fd->angle = dlf->angle;
			fd->pos = dlf->pos.toVec3() + trans;
			fd->blend = dlf->blend;
			fd->frequency = dlf->frequency;
			fd->rotatespeed = dlf->rotatespeed;
			fd->scale = dlf->scale;
			fd->size = dlf->size;
			fd->special = dlf->special;
			fd->speed = dlf->speed;
			fd->tolive = dlf->tolive;
			fd->move.x = 1.f;
			fd->move.y = 0.f;
			fd->move.z = 0.f;
			Vec3f out;
			out = VRotateY(fd->move, MAKEANGLE(fd->angle.getYaw()));
			
			fd->move = VRotateX(out, MAKEANGLE(fd->angle.getPitch()));
		}
	}
	
	progressBarAdvance(2.f);
	LoadLevelScreen();
	
	// Skip nodes
	pos += (dlh.version < 1.001f) ? 0 : dlh.nb_nodes * (204 + dlh.nb_nodeslinks * 64);
	
	LogDebug("Loading Paths");
	ARX_PATH_ReleaseAllPath();
	
	if(dlh.nb_paths) {
		ARXpaths = (ARX_PATH **)malloc(sizeof(ARX_PATH *) * dlh.nb_paths);
		nbARXpaths = dlh.nb_paths;
	}
	
	for(long i = 0; i < dlh.nb_paths; i++) {
		
#if defined(__MORPHOS__) || defined(__amigaos4__)
		DANAE_LS_PATH *dlp = reinterpret_cast<DANAE_LS_PATH *>(dat + pos);
		DANAE_LS_PATH dlp_copy = *dlp;
		dlp = &dlp_copy;
		
		dlp->idx = SDL_SwapLE16(dlp->idx);
		dlp->flags = SDL_SwapLE16(dlp->flags);
		dlp->initpos.x = SDL_SwapFloatLE(dlp->initpos.x);
		dlp->initpos.y = SDL_SwapFloatLE(dlp->initpos.y);
		dlp->initpos.z = SDL_SwapFloatLE(dlp->initpos.z);
		dlp->pos.x = SDL_SwapFloatLE(dlp->pos.x);
		dlp->pos.y = SDL_SwapFloatLE(dlp->pos.y);
		dlp->pos.z = SDL_SwapFloatLE(dlp->pos.z);
		dlp->nb_pathways = SDL_SwapLE32(dlp->nb_pathways);
		dlp->rgb.r = SDL_SwapFloatLE(dlp->rgb.r);
		dlp->rgb.g = SDL_SwapFloatLE(dlp->rgb.g);
		dlp->rgb.b = SDL_SwapFloatLE(dlp->rgb.b);
		dlp->farclip = SDL_SwapFloatLE(dlp->farclip);
		dlp->reverb = SDL_SwapFloatLE(dlp->reverb);
		dlp->amb_max_vol = SDL_SwapFloatLE(dlp->amb_max_vol);
		dlp->height = SDL_SwapLE32(dlp->height);
#else
		const DANAE_LS_PATH * dlp = reinterpret_cast<const DANAE_LS_PATH *>(dat + pos);
#endif
		pos += sizeof(DANAE_LS_PATH);
		
		Vec3f ppos = dlp->initpos.toVec3() + trans;
		ARX_PATH * ap = ARXpaths[i] = new ARX_PATH(boost::to_lower_copy(util::loadString(dlp->name)), ppos);
		
		ap->flags = PathFlags::load(dlp->flags); // TODO save/load flags
		ap->pos = dlp->pos.toVec3() + trans;
		ap->height = dlp->height;
		ap->ambiance = res::path::load(util::loadString(dlp->ambiance));
		
		ap->amb_max_vol = dlp->amb_max_vol;
		if(ap->amb_max_vol <= 1.f) {
			ap->amb_max_vol = 100.f;
		}
		
		ap->farclip = dlp->farclip;
		ap->reverb = dlp->reverb;
		ap->rgb = dlp->rgb;
		
		ap->pathways.resize(dlp->nb_pathways);
		for(long j = 0; j < dlp->nb_pathways; j++) {
			
#if defined(__MORPHOS__) || defined(__amigaos4__)
			DANAE_LS_PATHWAYS *dlpw = reinterpret_cast<DANAE_LS_PATHWAYS *>(dat + pos);
			DANAE_LS_PATHWAYS dlpw_copy = *dlpw;
			dlpw = &dlpw_copy;
			
			dlpw->rpos.x = SDL_SwapFloatLE(dlpw->rpos.x);
			dlpw->rpos.y = SDL_SwapFloatLE(dlpw->rpos.y);
			dlpw->rpos.z = SDL_SwapFloatLE(dlpw->rpos.z);
			dlpw->flag = SDL_SwapLE32(dlpw->flag);
			dlpw->time = SDL_SwapLE32(dlpw->time);
#else
			const DANAE_LS_PATHWAYS * dlpw = reinterpret_cast<const DANAE_LS_PATHWAYS *>(dat + pos);
#endif
			pos += sizeof(DANAE_LS_PATHWAYS);
			
			ap->pathways[j].flag = (PathwayType)dlpw->flag; // TODO save/load enum
			ap->pathways[j].rpos = dlpw->rpos.toVec3();
			ap->pathways[j]._time = GameDurationMs(dlpw->time); // TODO save/load time
		}
	}
	
	ARX_PATH_ComputeAllBoundingBoxes();
	progressBarAdvance(5.f);
	LoadLevelScreen();
	
	
	//Now LOAD Separate LLF Lighting File
	
	free(dat);
	pos = 0;
	dat = NULL;
	
	if(lightingFile) {
		
		LogDebug("Loading LLF Info");
		
		// using compression
		if(dlh.version >= 1.44f) {
			char * compressed = lightingFile->readAlloc();
			dat = blastMemAlloc(compressed, lightingFile->size(), FileSize);
			free(compressed);
		} else {
			dat = lightingFile->readAlloc();
			FileSize = lightingFile->size();
		}
	}
	// TODO size ignored
	
	if(!dat) {
		USE_PLAYERCOLLISIONS = true;
		LogInfo << "Done loading level";
		return true;
	}
	
#if defined(__MORPHOS__) || defined(__amigaos4__)
	DANAE_LLF_HEADER *llh = reinterpret_cast<DANAE_LLF_HEADER *>(dat + pos);
	DANAE_LLF_HEADER llh_copy = *llh;
	llh = &llh_copy;
	
	llh->version = SDL_SwapFloatLE(llh->version);
	llh->time = SDL_SwapLE32(llh->time);
	llh->nb_lights = SDL_SwapLE32(llh->nb_lights);
	llh->nb_Shadow_Polys = SDL_SwapLE32(llh->nb_Shadow_Polys);
	llh->nb_IGNORED_Polys = SDL_SwapLE32(llh->nb_IGNORED_Polys);
	llh->nb_bkgpolys = SDL_SwapLE32(llh->nb_bkgpolys);
#else
	const DANAE_LLF_HEADER * llh = reinterpret_cast<DANAE_LLF_HEADER *>(dat + pos);
#endif
	pos += sizeof(DANAE_LLF_HEADER);
	
	progressBarAdvance(4.f);
	LoadLevelScreen();
	
	if(llh->nb_lights != 0) {
		EERIE_LIGHT_GlobalInit();
	}
	
	for(int i = 0; i < llh->nb_lights; i++) {
		
#if defined(__MORPHOS__) || defined(__amigaos4__)
		DANAE_LS_LIGHT *dlight = reinterpret_cast<DANAE_LS_LIGHT *>(dat + pos);
		DANAE_LS_LIGHT dlight_copy = *dlight;
		dlight = &dlight_copy;
		
		dlight->pos.x = SDL_SwapFloatLE(dlight->pos.x);
		dlight->pos.y = SDL_SwapFloatLE(dlight->pos.y);
		dlight->pos.z = SDL_SwapFloatLE(dlight->pos.z);
		dlight->rgb.r = SDL_SwapFloatLE(dlight->rgb.r);
		dlight->rgb.g = SDL_SwapFloatLE(dlight->rgb.g);
		dlight->rgb.b = SDL_SwapFloatLE(dlight->rgb.b);
		dlight->fallstart = SDL_SwapFloatLE(dlight->fallstart);
		dlight->fallend = SDL_SwapFloatLE(dlight->fallend);
		dlight->intensity = SDL_SwapFloatLE(dlight->intensity);
		dlight->i = SDL_SwapFloatLE(dlight->i);
		dlight->ex_flicker.r = SDL_SwapFloatLE(dlight->ex_flicker.r);
		dlight->ex_flicker.g = SDL_SwapFloatLE(dlight->ex_flicker.g);
		dlight->ex_flicker.b = SDL_SwapFloatLE(dlight->ex_flicker.b);
		dlight->ex_radius = SDL_SwapFloatLE(dlight->ex_radius);
		dlight->ex_frequency = SDL_SwapFloatLE(dlight->ex_frequency);
		dlight->ex_size = SDL_SwapFloatLE(dlight->ex_size);
		dlight->ex_speed = SDL_SwapFloatLE(dlight->ex_speed);
		dlight->ex_flaresize = SDL_SwapFloatLE(dlight->ex_flaresize);
		dlight->extras = SDL_SwapLE32(dlight->extras);
#else
		const DANAE_LS_LIGHT * dlight = reinterpret_cast<const DANAE_LS_LIGHT *>(dat + pos);
#endif
		pos += sizeof(DANAE_LS_LIGHT);
		
		long j = EERIE_LIGHT_Create();
		if(j >= 0) {
			EERIE_LIGHT * el = g_staticLights[j];
			
			el->exist = 1;
			el->treat = 1;
			el->fallend = dlight->fallend;
			el->fallstart = dlight->fallstart;
			el->falldiffmul = 1.f / (el->fallend - el->fallstart);
			el->intensity = dlight->intensity;
			
			el->pos = dlight->pos.toVec3() + trans;
			
			el->rgb = dlight->rgb;
			
			el->extras = ExtrasType::load(dlight->extras);
			
			el->ex_flicker = dlight->ex_flicker;
			el->ex_radius = dlight->ex_radius;
			el->ex_frequency = dlight->ex_frequency;
			el->ex_size = dlight->ex_size;
			el->ex_speed = dlight->ex_speed;
			el->ex_flaresize = dlight->ex_flaresize;
			
			el->m_ignitionStatus = !(el->extras & EXTRAS_STARTEXTINGUISHED);
			
			if((el->extras & EXTRAS_SPAWNFIRE) && (!(el->extras & EXTRAS_FLARE))) {
				el->extras |= EXTRAS_FLARE;
				if(el->extras & EXTRAS_FIREPLACE) {
					el->ex_flaresize = 95.f;
				} else {
					el->ex_flaresize = 80.f;
				}
			}
			
			el->m_ignitionLightHandle = LightHandle();
			el->sample = audio::INVALID_ID;
		}
	}
	
	progressBarAdvance(2.f);
	LoadLevelScreen();
	
#if defined(__MORPHOS__) || defined(__amigaos4__)
	DANAE_LS_LIGHTINGHEADER *dll = reinterpret_cast<DANAE_LS_LIGHTINGHEADER *>(dat + pos);
	DANAE_LS_LIGHTINGHEADER dll_copy = *dll;
	dll = &dll_copy;
	
	dll->nb_values = SDL_SwapLE32(dll->nb_values);
	dll->ViewMode = SDL_SwapLE32(dll->ViewMode);
	dll->ModeLight = SDL_SwapLE32(dll->ModeLight);
#else
	const DANAE_LS_LIGHTINGHEADER * dll = reinterpret_cast<const DANAE_LS_LIGHTINGHEADER *>(dat + pos);
#endif
	pos += sizeof(DANAE_LS_LIGHTINGHEADER);
	
	long bcount = dll->nb_values;
	LastLoadedLightningNb = bcount;
	
	//DANAE_LS_VLIGHTING
	free(LastLoadedLightning);
	ColorBGRA * ll = LastLoadedLightning = (ColorBGRA *)malloc(sizeof(ColorBGRA) * bcount);
	if(dlh.version > 1.001f) {
		std::transform((u32 *)(dat + pos), (u32 *)(dat + pos) + bcount, LastLoadedLightning, savedColorConversion);
		pos += sizeof(u32) * bcount;
	} else {
		while(bcount) {
#if defined(__MORPHOS__) || defined(__amigaos4__)
			DANAE_LS_VLIGHTING *dlv = reinterpret_cast<DANAE_LS_VLIGHTING *>(dat + pos);
			DANAE_LS_VLIGHTING dlv_copy = *dlv;
			dlv = &dlv_copy;
			
			dlv->r = SDL_SwapLE32(dlv->r);
			dlv->g = SDL_SwapLE32(dlv->g);
			dlv->b = SDL_SwapLE32(dlv->b);
#else
			const DANAE_LS_VLIGHTING * dlv = reinterpret_cast<const DANAE_LS_VLIGHTING *>(dat + pos);
#endif
			pos += sizeof(DANAE_LS_VLIGHTING);
			*ll = Color((dlv->r & 255), (dlv->g & 255), (dlv->b & 255), 255).toBGRA();
			ll++;
			bcount--;
		}
	}
	
	ARX_UNUSED(pos), ARX_UNUSED(FileSize);
	arx_assert(pos <= FileSize);
	
	free(dat);
	
	progressBarAdvance();
	LoadLevelScreen();
	
	USE_PLAYERCOLLISIONS = true;
	
	LogInfo << "Done loading level";
	
	return true;
	
}

long FAST_RELEASE = 0;

extern long JUST_RELOADED;

void DanaeClearLevel() {
	
	JUST_RELOADED = 0;
	g_miniMap.reset();

	fadeReset();
	LAST_JUMP_ENDTIME = 0;
	FAST_RELEASE = 1;
	MCache_ClearAll();
	ARX_GAME_Reset();
	FlyingOverIO = NULL;

	EERIE_PATHFINDER_Release();

	InitBkg(ACTIVEBKG);
	
	EERIE_LIGHT_GlobalInit();
	ARX_FOGS_Clear();
	
	culledStaticLightsReset();
	
	UnlinkAllLinkedObjects();
	
	entities.clear();
	
	TextureContainer::DeleteAll(TextureContainer::Level);
	g_miniMap.clearMarkerTexCont();
	
	bGCroucheToggle = false;
	
	resetDynLights();
	
	TREATZONE_Release();
	TREATZONE_Clear();
	
	FAST_RELEASE = 0;
	
	CURRENTLEVEL = -1;
}

void RestoreLastLoadedLightning(BackgroundData & eb)
{
	long pos = 0;
	long bcount = CountBkgVertex();

	if(LastLoadedLightningNb <= 0)
		return;

	if(LastLoadedLightning == NULL)
		return;

	if(bcount != LastLoadedLightningNb) {
		free(LastLoadedLightning);
		LastLoadedLightning = NULL;
		LastLoadedLightningNb = 0;
		return;
	}

	bcount = LastLoadedLightningNb;
	
	// TODO copy-paste poly iteration
	for(short z = 0; z < eb.m_size.y; z++)
	for(short x = 0; x < eb.m_size.x; x++) {
		BackgroundTileData & eg = eb.m_tileData[x][z];
		for(long l = 0; l < eg.nbpoly; l++) {
			EERIEPOLY & ep = eg.polydata[l];
			
			long nbvert = (ep.type & POLY_QUAD) ? 4 : 3;
			
			for(long k = 0; k < nbvert; k++) {
				Color dc = Color::fromBGRA(LastLoadedLightning[pos]);
				pos++;
				dc.a = 255;
				ep.color[k] = ep.v[k].color = dc.toRGB();
				bcount--;
				
				if(bcount <= 0)
					goto plusloin;
			}
		}
	}

plusloin:
	free(LastLoadedLightning);
	LastLoadedLightning = NULL;
	LastLoadedLightningNb = 0;
}

/*
 * Copyright 2011-2016 Arx Libertatis Team (see the AUTHORS file)
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

#include "core/Version.h"

/*!
 * This file is automatically processed by cmake if the version or commit id changes.
 * For the exact syntax see the documentation of the configure_file() cmake command.
 * For available variables see cmake/VersionString.cmake.
 */

#if 6 != 6 && 6 != 7
#error "Configure error - the VERSION file should specify 5 or 6 non-empty lines!"
#endif

const std::string arx_name = "Arx Libertatis";
const std::string arx_icon_name = "arx-libertatis";
const std::string arx_version = "1.2-dev + 349269e";
const u64 arx_version_number = (u64(1) << 48) | (u64(1) << 32)
                             | (u64(9999) << 16) | (u64(9999) << 0);
const std::string arx_release_codename = "";

// TODO move this to an external file once we ship our own resources
const std::string arx_credits = "~Thanks for playing Arx Libertatis!\n"
	"\n"
	"This version of Arx Fatalis has been created by various community members in their spare time.\n"
	"We welcome your criticism, suggestions and contributions!\n"
	"You can find us at http://arx-libertatis.org/\n"
	"\n"
	"\n"
	"~ARX LIBERTATIS CONTRIBUTORS\n"
	"\n"
	"Erik Lund (Akhilla)\n"
	"Sebastien Lussier (BobJelly)\n"
	"Fabian Roth (Eli2)\n"
	"Lubosz Sarnecki (lubosz)\n"
	"Daniel Scharrer (dscharrer)\n"
	"David Stejskal (bsxf47)\n"
	"\n"
	"~Additional code by:\n"
	"\n"
	"Philippe Cavalaria (Nuky)\n"
	"Gabriel Cuvillier\n"
	"Dimoks\n"
	"dreamer.dead\n"
	"Adrian Fuhrmann\n"
	"Chris Gray (chrismgray)\n"
	"guidoj\n"
	"HelenWong\n"
	"Tafferwocky\n"
	"Thomas Leroy (LordSk)\n"
	"Alex Marginean (Alex M.)\n"
	"nemyax\n"
	"Olli Numminen (Olzaq)\n"
	"Jonathan Powell (jfpowell)\n"
	"QJa42FMM\n"
	"Jean-Francois Richard\n"
	"Elliot Rigets (adejr)\n"
	"xanm\n"
	"XoD\n"
	"\n"
	"~Additional translations by:\n"
	"\n"
	"nemyax\n"
	"steinuil\n"
	"\n"
	"~Many thanks to:\n"
	"All our users, testers and packagers, including\n"
	"\n"
	"Akien\n"
	"amdmi3\n"
	"Andrey Bondrov\n"
	"barra\n"
	"DaveMachine\n"
	"Alexandre Detiste (a-detiste)\n"
	"gcala\n"
	"hasufell\n"
	"i30817\n"
	"Jan-Hendrik Peters\n"
	"smls\n"
	"Tafferwocky\n"
	"vytautas (aka. ProzacR)\n"
	"Dimitris Zlatanidis\n"
	"\n"
	"~Special thanks to\n"
	"\n"
	"Arnaud David\n"
	"\n"
	"As well as everyone else at Arkane Studios\n"
	"\n"
	"For making this project possible by creating Arx Fatalis, releasing the source code and answering our questions";
const std::string arx_copyright = "~Arx Libertatis is free software:\n"
	"You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version (see the LICENSE file for details). Some parts of the source code are available under more permissive licenses.\n"
	"Copyright (C) 2011-2017 Arx Libertatis Team and Contributors.\n"
	"\n"
	"Arx Libertatis is based on the Arx Fatalis GPLv3 source code available from http://www.arkane-studios.com/uk/arx_downloads.php\n"
	"Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.\n"
	"\n"
	"Arx Libertatis makes uses of DejaVu fonts, based on the Bitstream Vera fonts (c) Bitstream. DejaVu changes are in public domain. Glyphs imported from Arev fonts are (c) Tavmjong Bah. See the LICENSE.DejaVu file for details.";

const std::string cmake_version = "3.6.2";

const int tbg_version_id = 1;

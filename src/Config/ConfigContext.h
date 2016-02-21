/*
    Copyright (C) 2016 EdWard <ezeward4@gmail.com>
    ---
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301, USA.

 */

#ifndef SRC_CONFIG_CONFIGCONTEXT_H_
#define SRC_CONFIG_CONFIGCONTEXT_H_

// Project Includes
#include "PortGroup.h"
#include "SettingGroup.h"

// STD Includes
#include <map>
#include <string>

// Library Includes
#include "json.hpp"

namespace dman
{

/** Class which contains a context which can be shared by all systems
 */
class ConfigContext
{
public:
	ConfigContext() = default;

	/** Constructs ConfigContext preloading manifest at home_path
	 * If it fails to load this manifest, it will set the manifest to null,
	 * and \c has_no_manifest() will return \c true
	 */
	ConfigContext(std::string home_path);
	virtual ~ConfigContext() = default;

public:
	/** Loads configs from the files described in the \c manifest_
	 * @exception InvalidManifestError if the manifest file was missing values
	 * @return true if any sub config failed to load all data
	 */
	virtual bool LoadConfig();

	/** Assembles configs from the files described in the \c manifest_
	 * @exception InvalidManifestError if the manifest file was missing values
	 * @return true if any of the sub config assemblies failed
	 */
	virtual bool AssembleConfig();

	/** Stores the current configuration for each sub object into \\
	 * the file described in the manifest
	 * @exception InvalidManifestError if the manifest file was missing values
	 */
	virtual void SaveConfig() const;

	/** Stores the current schema for each sub object into the file \\
	 * described in the manifest
	 * @exception InvalidManifestError if the manifest file was missing values
	 */
	virtual void SaveSchema() const;

public:
	/// Returns root port group
	PortGroup& get_ports() {return ports_;}
	const PortGroup& get_ports() const {return ports_;}

	/// Returns root settings group
	SettingGroup& get_settings() {return settings_;}
	const SettingGroup& get_settings() const {return settings_;}

	/** Sets a primary sub node's port space and propogates it
	 * Creates the node if necessary
	 * @param space_name key of the sub node to create or set port space of
	 * @param port_space shared pointer to port space
	 * @return The root of this port space (primary sub node at \c space_name
	 */
	PortGroup RegisterPortSpace(const PortGroup::Key_t& space_name,
						   PortGroup::PortSpace_t port_space);

public:
	/// Return path to home directory
	std::string get_home_path() const {return home_;}

	/** Changes the path to the home directory and reloads the manifest file
	 * Manifest file is loaded from \c manifest.json from the home directory
	 * @exception InvalidManifestError if the manifest file could not be found
	 * State of the object will be as it was before the call if it throws
	 */
	void SetHomePath(std::string home);

	/// Returns the loaded manifest json object
	json get_manifest() const {return manifest_;}

	bool has_no_manifest() const {return manifest_.is_null();}

	/** Returns the json file corresponding to that json path in the manifest
	 * @return json tree corresponding to that path
	 * @exception InvalidManifestError if the manifest file lacks that path
	 */
	json GetConfigFile(const std::string &path) const;

	/** Places the json config at the file specified by the path
	 * @param path Path in manifest of file to store json at
	 * @param config Config to save
	 * @exception InvalidManifestError if the manifest file lacks that path
	 */
	void SaveConfigFile(const std::string &path, json config) const;

private:
	/// Gets the filename from the path in the manifest
	std::string getFilename(const std::string &path) const;

private:
	void assertValidManifest() const;

private:
	std::string home_;
	json manifest_;
	PortGroup ports_;
	SettingGroup settings_;
};


}  // namespace dman

#endif  // SRC_CONFIG_CONFIGCONTEXT_H_

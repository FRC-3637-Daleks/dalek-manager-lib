/*
    Copyright (C) 2016, EdWard <ezeward4@gmail.com>
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

// Project Includes
#include "ConfigContext.h"
#include "InvalidManifestError.h"

// STD Includes
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace dman
{

ConfigContext::ConfigContext(std::string home_path)
{
	try
	{
		SetHomePath(std::move(home_path));
	}
	catch(const InvalidManifestError& e)
	{
		manifest_ = json(json::value_t::null);
	}
}

void ConfigContext::assertValidManifest() const
{
	if(has_no_manifest())
		throw InvalidManifestError(home_ + "/manifest.json");
}

std::string ConfigContext::getFilename(const std::string &path) const
{
	assertValidManifest();

	auto manifest_node = get_manifest();

	for(int i = 0, j = path.find_first_of('/');
		i != std::string::npos;
		i = j+1, j = path.find_first_of('/', j+1))
	{
		try
		{
			if (j != std::string::npos)
				manifest_node = manifest_node.at(path.substr(i, j - i));
			else
			{
				manifest_node = manifest_node.at(path.substr(i, j));
				break;
			}
		}
		catch(const std::logic_error& e)
		{
			throw InvalidManifestError(home_ + "/manifest.json", path);
		}
	}

	std::string ret = manifest_node.get<std::string>();
	if(ret.empty())
		return ret;

	if(ret[0] != '/')  // Ensure it's an absolute path just to be safe
		ret = get_home_path() + ret;

	return ret;
}

json ConfigContext::GetConfigFile(const std::string &path) const
{
	assertValidManifest();
	auto filename = getFilename(path);
	if (filename == "")
		return json::value_t::null;
	std::ifstream config_file(filename);
	json ret;
	try
	{
		ret << config_file;
	}
	catch(const std::invalid_argument& e)
	{
		ret = nullptr;
	}
	return ret;
}

void ConfigContext::SaveConfigFile(const std::string &path, json config) const
{
	assertValidManifest();
	auto filename = getFilename(path);
	if(filename.empty())
		throw InvalidManifestError(home_ + "/manifest.json", path);
	std::ofstream config_file(getFilename(path));
	config_file << std::setw(4) << std::move(config);
}

bool ConfigContext::LoadConfig()
{
	assertValidManifest();
	InvalidManifestError ports_error, settings_error;
	InvalidManifestError *error = nullptr;

	bool ret = true;

	try
	{
		ret &= ports_.LoadConfig(GetConfigFile("runtime/configs/ports"));
	}
	catch(const InvalidManifestError& e)
	{
		ports_error = e;
		error = &ports_error;
	}

	try
	{
		ret |= settings_.LoadConfig(GetConfigFile("runtime/configs/settings"));
	}
	catch(const InvalidManifestError& e)
	{
		settings_error = e;
		error = &settings_error;
	}

	if(error != nullptr)
		throw *error;

	return ret;
}

bool ConfigContext::AssembleConfig()
{
	assertValidManifest();
	InvalidManifestError ports_error, settings_error;
	InvalidManifestError *error = nullptr;

	bool ret = true;

	try
	{
		ret &= ports_.AssembleConfig(GetConfigFile("runtime/configs/ports"));
	}
	catch(const InvalidManifestError& e)
	{
		ports_error = e;
		error = &ports_error;
	}

	try
	{
		ret |= settings_.AssembleConfig(GetConfigFile("runtime/configs/settings"));
	}
	catch(const InvalidManifestError& e)
	{
		settings_error = e;
		error = &settings_error;
	}

	if(error != nullptr)
		throw *error;

	return ret;
}

void ConfigContext::SaveConfig() const
{
	assertValidManifest();
	InvalidManifestError ports_error, settings_error;
	InvalidManifestError *error = nullptr;

	try
	{
		SaveConfigFile("runtime/configs/ports", ports_.GetConfig());
	}
	catch(const InvalidManifestError& e)
	{
		ports_error = e;
		error = &ports_error;
	}

	try
	{
		SaveConfigFile("runtime/configs/settings", settings_.GetConfig());
	}
	catch(const InvalidManifestError& e)
	{
		settings_error = e;
		error = &settings_error;
	}

	if(error != nullptr)
		throw *error;
}

void ConfigContext::SaveSchema() const
{
	InvalidManifestError ports_error, settings_error;
	InvalidManifestError *error = nullptr;

	try
	{
		SaveConfigFile("templates/configs/ports", ports_.GetSchema());
	}
	catch(const InvalidManifestError& e)
	{
		ports_error = e;
		error = &ports_error;
	}

	try
	{
		SaveConfigFile("templates/configs/settings", settings_.GetSchema());
	}
	catch(const InvalidManifestError& e)
	{
		settings_error = e;
		error = &settings_error;
	}

	if(error != nullptr)
		throw *error;
}

PortGroup& ConfigContext::RegisterPortSpace(const PortGroup::Key_t& space_name,
										   PortGroup::PortSpace_t port_space)
{
	auto& ret = ports_[space_name];
	ret.PropogatePortSpace(std::move(port_space));
	return ret;
}

void ConfigContext::SetHomePath(std::string home)
{
	std::ifstream manifest_file(home + "/manifest.json");
	if(manifest_file.fail())
	{
		throw InvalidManifestError(home + "/manifest.json");
	}

	manifest_ = json::parse(std::move(manifest_file));
	home_ = std::move(home);
}

ValueStore& ConfigContext::GetValueStore(const Key_t& name)
{
	return value_stores_[name];
}

UpdateStore& ConfigContext::GetUpdateStore(const Key_t& name)
{
	return update_stores_[name];
}

UpdateThread& ConfigContext::GetUpdateThread(const Key_t& name)
{
	return update_threads_[name];
}

}  // namespace dman

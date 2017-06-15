/*
 * Version.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "dlog.h"
#include "db/constant/Version.h"

using namespace std;
using namespace boost;

Version::Version(std::string version)
{
	vector<string> splitVersion;
	boost::split( splitVersion, version, boost::is_any_of( "\\." ));

	if (splitVersion.size() != 3) {
		dlog_print(DLOG_DEBUG,LOG_TAG,"version is not collect");
		return;
	}

	major = atoi(splitVersion[0].c_str());
	minor = atoi(splitVersion[1].c_str());
	revision = atoi(splitVersion[2].c_str());
}
Version::Version(int major, int minor, int revision)
{
    this->major = major;
    this->minor = minor;
    this->revision = revision;
}

Version::~Version(void)
{

}

int Version::getMajor(void) {
	return major;
}

int Version::getMinor(void) {
	return minor;
}

int Version::getRevision(void) {
	return revision;
}

void Version::setMajor(int major) {
	this->major = major;
}

void Version::setMinor(int minor) {
	this->minor = minor;
}

void Version::setRevision(int revision) {
	this->revision = revision;
}

bool Version::isBiggerThanMajor(Version ver) {
	if (this->major > ver.getMajor()) {
		return true;
	}
	else if (this->major < ver.getMajor()) {
		return false;
	}
	dlog_print(DLOG_DEBUG,LOG_TAG,"Major version is same!!");
	return false;
}

bool Version::isBiggerThanMinor(Version ver) {
	if (this->minor > ver.getMinor()) {
		return true;
	}
	else if (this->minor < ver.getMinor()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"Minor version is same!!");
	return false;
}

bool Version::isBiggerThanRevision(Version ver) {
	if (this->revision > ver.getRevision()) {
		return true;
	}
	else if (this->revision < ver.getRevision()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"Revision version is same!!");
	return false;
}


bool Version::isBiggerThan(Version ver) {
	if (this->major > ver.getMajor()) {
		return true;
	}
	else if (this->major < ver.getMajor()) {
		return false;
	}

	if (this->minor > ver.getMinor()) {
		return true;
	}
	else if (this->minor < ver.getMinor()) {
		return false;
	}

	if (this->revision > ver.getRevision()) {
		return true;
	}
	else if (this->revision < ver.getRevision()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"version is same!!");
	return false;
}


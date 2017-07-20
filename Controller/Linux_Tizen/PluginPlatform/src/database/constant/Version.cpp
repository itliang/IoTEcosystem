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
#include <database/constant/Version.h>
#include "dlog.h"

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

	mMajor = atoi(splitVersion[0].c_str());
	mMinor = atoi(splitVersion[1].c_str());
	mRevision = atoi(splitVersion[2].c_str());
}
Version::Version(int major, int minor, int revision)
{
    this->mMajor = major;
    this->mMinor = minor;
    this->mRevision = revision;
}

Version::~Version(void)
{

}

int Version::getMajor(void) {
	return mMajor;
}

int Version::getMinor(void) {
	return mMinor;
}

int Version::getRevision(void) {
	return mRevision;
}

void Version::setMajor(int major) {
	this->mMajor = major;
}

void Version::setMinor(int minor) {
	this->mMinor = minor;
}

void Version::setRevision(int revision) {
	this->mRevision = revision;
}

bool Version::isBiggerThanMajor(Version ver) {
	if (this->mMajor > ver.getMajor()) {
		return true;
	}
	else if (this->mMajor < ver.getMajor()) {
		return false;
	}
	dlog_print(DLOG_DEBUG,LOG_TAG,"Major version is same!!");
	return false;
}

bool Version::isBiggerThanMinor(Version ver) {
	if (this->mMinor > ver.getMinor()) {
		return true;
	}
	else if (this->mMinor < ver.getMinor()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"Minor version is same!!");
	return false;
}

bool Version::isBiggerThanRevision(Version ver) {
	if (this->mRevision > ver.getRevision()) {
		return true;
	}
	else if (this->mRevision < ver.getRevision()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"Revision version is same!!");
	return false;
}


bool Version::isBiggerThan(Version ver) {
	if (this->mMajor > ver.getMajor()) {
		return true;
	}
	else if (this->mMajor < ver.getMajor()) {
		return false;
	}

	if (this->mMinor > ver.getMinor()) {
		return true;
	}
	else if (this->mMinor < ver.getMinor()) {
		return false;
	}

	if (this->mRevision > ver.getRevision()) {
		return true;
	}
	else if (this->mRevision < ver.getRevision()) {
		return false;
	}

	dlog_print(DLOG_DEBUG,LOG_TAG,"version is same!!");
	return false;
}


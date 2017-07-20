/*
 * Version.h
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_CONSTANT_VERSION_H_
#define DATABASE_CONSTANT_VERSION_H_

#include <string>

class Version
{
public:
	Version(std::string version);
	Version(int major, int minor, int revision);
	virtual ~Version(void);

    int getMajor(void);
    int getMinor(void);
    int getRevision(void);
    void setMajor(int major);
    void setMinor(int minor);
    void setRevision(int revision);

    bool isBiggerThanMajor(Version ver);
    bool isBiggerThanMinor(Version ver);
    bool isBiggerThanRevision(Version ver);
    bool isBiggerThan(Version ver);

private:
    int mMajor;
    int mMinor;
    int mRevision;
};


#endif /* DATABASE_CONSTANT_VERSION_H_ */

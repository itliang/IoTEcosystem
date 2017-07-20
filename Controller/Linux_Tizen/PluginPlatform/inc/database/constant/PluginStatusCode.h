/*
 * PluginStatusCode.h
 *
 *  Created on: Jun 19, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_CONSTANT_PLUGINSTATUSCODE_H_
#define DATABASE_CONSTANT_PLUGINSTATUSCODE_H_

enum PluginStatusCode {
    STATUS_VALID = 0,
    STATUS_IS_UPDATE_AVAILABLE = 1,
    STATUS_TO_BE_DELETED = 2,
    STATUS_OUTDATED = 3,
};

#endif /* DATABASE_CONSTANT_PLUGINSTATUSCODE_H_ */

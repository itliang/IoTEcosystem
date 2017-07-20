#include "info/PluginInfo.hpp"
#include <libxml/parser.h>
#include <libxml/tree.h>

void xml_parse_type(int type);
void xml_parse_response(PluginInfo * info, char *body);

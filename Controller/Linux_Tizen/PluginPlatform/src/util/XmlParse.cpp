#include "util/XmlParse.h"
#include <string>
#include "util/Log.h"
#include <stdlib.h>
#include "galaxyapps/PluginFileRequest.h"

int parse_type;
void xml_parse_type(int type)
{
	parse_type = type;
}
void xml_parse_node(PluginInfo * info, xmlNode *Node,bool SaveFlag)
{

	xmlNode *cur_node = NULL;
	for (cur_node = Node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			
            
			string name = (char *)cur_node->name;
			string content_value = (char *)xmlNodeGetContent(cur_node);

			WDEBUG("node type: Element, name: %s : %d \n", name.c_str(),SaveFlag);

			if(SaveFlag){
							
				//save value into pluginInfo
				if(name == "productName"){
					
				}else if(name == "appId"){
					info->setId(content_value);
				}else if(name == "productIconImgURL"){
					
				}else if(name == "versionName"){
					info->setVersionName(content_value);
				}else if(name == "versionCode"){
					info->setVersionCode(content_value);
				}else if(name == "realContentSize"){
					info->setFileSize(atoi(content_value.c_str()));
				}else if(name == "sellerName"){
					
				}else if(name == "pluginDeviceType"){
					info->setDeviceType(content_value);
				}else if(name == "pluginDeviceSubType"){
					info->setDeviceSubType(content_value);
				}else if(name == "pluginMinVer"){
					info->setPlatformVersion(content_value);
				}else if (name == "downloadURI"){
					info->setUrl(content_value);				
				}			
			}

            if((name == "appInfo" && parse_type == MSG::START_FIND) || (name == "result" && parse_type == MSG::START_DOWNLOAD_URI)) {
				xml_parse_node(info,cur_node->children,true);
				
			}else{
				xml_parse_node(info,cur_node->children,false);
			}
			
        }
     }

}

void xml_parse_response(PluginInfo * info, char *body)
{

	 xmlNode *root_element = NULL;
	 xmlDoc *doc = NULL;
	 
	 doc = xmlReadMemory(body,strlen(body),"in_memory.xml",NULL,NULL);
	 if (doc == NULL) {
        WDEBUG("error: could not parse body");
     }
	
	 /*Get the root element node */
	 if(doc != NULL){
		root_element = xmlDocGetRootElement(doc);
	 }
	
	
	xml_parse_node(info,root_element,false);
     

	 
}


#include "entity.h"
#include <string>
#include <list>
#include <easy_logger/easy_logger.h>


EntityAnn::EntityAnn(string hdt_file_dir, string log_file_dir){
   m_logger = new EasyLogger(log_file_dir);
}


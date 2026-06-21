


#ifndef PARSER_H
#define PARSER_H

#include <pugixml.hpp>
#include <unordered_map>

#include "types.h"

void parse_blocks ( const pugi::xml_node&, std::vector<block>&, std::unordered_map<int, int>& );
void parse_lines ( const pugi::xml_node&, std::vector<block>&, std::unordered_map<int, int>& );


#endif /* PARSER_H */
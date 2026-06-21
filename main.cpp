#include <iostream>
#include <pugixml.hpp>
#include <unordered_map>

#include "types.h"
#include "parser.h"
#include "sort.h"
#include "generator.h"




int main ()
{
	std::vector<block> blocks;
	std::vector<int> order;				// Последовательность блоков
	std::unordered_map<int, int> sid_to_index;	// SID -> индекс в blocks

	pugi::xml_document doc;
	
	if ( !doc.load_file ( "source.xml" ) ){
		std::cerr << "Failed to load file\n";

	} else {
		pugi::xml_node system = doc.child( "System" );

		parse_blocks ( system, blocks, sid_to_index );
		parse_lines ( system, blocks, sid_to_index );

		// top_sort(blocks, order, sid_to_index);		// рекурсивная версия
		top_sort_kahn ( blocks, order, sid_to_index );		// алгоритм Кана

		gen_code ( blocks, order, sid_to_index );


		/*
		for ( const auto& b : blocks ) {
        		std::cout << "SID=" << b.sid << ", Type=" << b.type << ", Name=" << b.name << std::endl;
        		for ( const auto& p : b.params ) {
				std::cout << "  " << p.first << " => " << p.second << std::endl;
			}
			for ( const auto& p : b.dep ) {
				int i = 1;
				std::cout << i << " => " << p << std::endl;
				i++;
			}

			std::cout << "degree = " << b.degree << "\n" << std::endl;

		}
		
		for ( int o : order ) {
			std::cout << o << std::endl;
		}

		*/
	}

	return 0;
}




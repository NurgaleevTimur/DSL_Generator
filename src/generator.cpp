
#include <iostream>
#include <algorithm>
#include "generator.h"


typedef void (*func)(std::vector<block>&, block&, std::unordered_map<int, int>&);



void __ext_ports ( std::vector<block>& blocks, block& b, int i, std::unordered_map<int, int>& sid_to_index )
{
	std::cout << "\t{ " << "\"" << b.name << "\", &nwocg.";

	if ( i ) std::cout << b.name;
	else	std::cout << blocks[ sid_to_index [ b.dep[0] ] ].name;

	std::cout << ", " << i << " },\n";

}

void __add ( std::vector<block>& blocks, block& b, std::unordered_map<int, int>& sid_to_index )
{
	std::string sign = "+";
	auto it = b.params.find("Inputs");
	std::string first = blocks[ sid_to_index [ b.dep[0] ] ].name;
	std::string second = blocks[ sid_to_index [ b.dep[1] ] ].name;

	if ( it != b.params.end() )
		sign = it->second.substr( 1, 2 );

	std::cout << "nwocg." << first << " " << sign << " nwocg." << second << ";\n";

}



void __gain ( std::vector<block>& blocks, block& b, std::unordered_map<int, int>& sid_to_index )
{
	std::string dep = blocks[ sid_to_index [ b.dep[0] ] ].name;
	std::string g = ( b.params.find ( "Gain" ) )->second;

	std::cout << "nwocg." << dep << " * " << g << ";\n";
}



std::map<std::string, func> fbt = {
	{ "Sum", __add },
	{ "Gain", __gain }
};



void gen_code ( std::vector<block>& blocks, std::vector<int>& queue, std::unordered_map<int, int>& sid_to_index )
{
	std::cout << "#include <math.h>\n#include \"nwocg_run.h\"\n\n";

	std::cout << "static struct\n{\n";

	for ( block b : blocks ) {
		if ( b.type != "Outport" )
			std::cout << "\tdouble " << b.name << ";\n";
	}

	std::cout << "} nwocg;\n\n\n";

	std::cout << "void nwocg_generated_init()\n{\n";

	for ( block b : blocks ) {
		if ( b.type == "UnitDelay" )
			std::cout << "\tnwocg." << b.name << " = 0;\n";
	}

	std::cout << "}\n\n\n";

	std::cout << "void nwocg_generated_step()\n{\n";

	std::vector<int> udel;
	for ( int q : queue ) {
		block& b = blocks[ sid_to_index[ q ] ];

		if ( b.type != "UnitDelay" ) {
			if ( b.type != "Outport" && b.type != "Inport" ) {
				std::cout << "\tnwocg." << b.name << " = ";
				fbt[ b.type ] ( blocks, b, sid_to_index );
			}
		} else udel.push_back( b.sid );
	}

	for ( int u : udel ) {
		block& b = blocks[ sid_to_index[ u ] ];
		std::cout << "\tnwocg." << b.name << " = " 
			<< "nwocg." << blocks[ sid_to_index [ b.dep [0] ] ].name
			<< ";\n";
	}

	std::cout << "}\n\n\n";


	std::cout << "static const nwocg_ExtPort ext_ports[] =\n{\n";

	for ( block b : blocks ) {
		if ( b.type == "Inport" )
			__ext_ports ( blocks, b, 1, sid_to_index );
		else if ( b.type == "Outport" )
			__ext_ports ( blocks, b, 0, sid_to_index );
	}

	std::cout << "\t{ 0, 0, 0 }\n};\n\n\n";

	std::cout << "const nwocg_ExtPort * const nwocg_generated_ext_ports = ext_ports;\n\n\n";

	std::cout << "const size_t nwocg_generated_ext_ports_size = sizeof(ext_ports);\n";

}

